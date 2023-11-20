//=============================================================================
// 
//  �v���C���[���� [union_armtoarm.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "union_armtoarm.h"
#include "player_union.h"
#include "player.h"
#include "game.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "enemy.h"
#include "calculation.h"
#include "model.h"
#include "motion.h"
#include "objectChara.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"
#include "impactwave.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define LIFE_UNION			(60 * 200)	// ���̎���
#define DEADTIME			(120)		// ���S���̎���
#define MAX_ALTERNATELY		(4)			// ���ݍU���̉�
#define INTERVAL_ALTERNATELY	(60)			// ���ݍU���̃C���^�[�o��

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CUnion_ArntoArm::CUnion_ArntoArm(int nPriority) : CPlayerUnion(nPriority)
{
	// �l�̃N���A
	m_nInputSuperAtkIdx = 0;	// �K�E�Z���͂̃C���f�b�N�X
	m_nIntervalAlternately = 0;	// ���ݓ��͂̃C���^�[�o��
	m_nCntInputSuperATK = 0;	// �K�E�Z�̓��͉�
	m_bSuperATK = false;		// �K�E�Z�̔���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CUnion_ArntoArm::~CUnion_ArntoArm()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CUnion_ArntoArm::Init(void)
{
	// ������
	CPlayerUnion::Init();

	// ��ނ̐ݒ�
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;		// ���
	m_nUnionLife = LIFE_UNION;	// ���̎���

	SetPosition(D3DXVECTOR3(-600.0f, 0.0f, -1000.0f));
	return S_OK;
}

//==========================================================================
// �p�[�c�̐ݒ�
//==========================================================================
HRESULT CUnion_ArntoArm::CreateParts(void)
{
	CObjectChara *pObjChar = NULL;

	// �����L�����ǂݍ���
	ReadMultiCharacter("data\\TEXT\\multicharacter\\ArmtoArm.txt");

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CUnion_ArntoArm::Uninit(void)
{
	// �I������
	CPlayerUnion::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CUnion_ArntoArm::Update(void)
{
	// �X�V����
	CPlayerUnion::Update();

	// ���S����擾
	if (IsDeath())
	{
		return;
	}

	// ���̎������Z
	m_nUnionLife--;

	if (m_nUnionLife <= 0)
	{// ���̏I��

		// �v���C���[�擾
		CPlayer **ppPlayer = CManager::GetInstance()->GetScene()->GetPlayer();

		// �ʒu�擾
		D3DXVECTOR3 pos = GetPosition();

		// ���̉���
		ppPlayer[m_nPartsIdx[0]]->SetState(CPlayer::STATE_RELEASEUNION);
		ppPlayer[m_nPartsIdx[0]]->SetPosition(pos);
		ppPlayer[m_nPartsIdx[1]]->SetState(CPlayer::STATE_RELEASEUNION);
		ppPlayer[m_nPartsIdx[1]]->SetPosition(pos);

		// �I������
		Kill();
		Uninit();
		return;
	}


	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[�K�E�I�I�I�I�I�I�I]------------------\n"
		"���͎�t���̃C���f�b�N�X�F�y%d�z\n"
		"���͉񐔁F�y%d�z \n", m_nInputSuperAtkIdx, m_nCntInputSuperATK);

}

//==========================================================================
// �p�[�c�̃R���g���[������
//==========================================================================
void CUnion_ArntoArm::ControllParts(void)
{
	// �e���ʂ̑���	
	for (int i = 0; i < PARTS_MAX; i++)
	{
		int nPartsIdx = CManager::GetInstance()->GetByPlayerPartsType(i);

		if (i == m_nControllMoveIdx)
		{
			// �r�ړ�����
			ControllLeg(i);
		}

		// �U������
		ControllATK(i, m_nPartsIdx[i]);
	}


	// �U���̓��̓J�E���^�[���Z
	m_nCntInputAtk--;
	if (m_nCntInputAtk <= 0)
	{
		m_nCntInputAtk = 0;

		// �K�E�Z�̓��͉񐔃��Z�b�g
		m_nCntInputSuperATK = 0;
	}

	if (m_bSuperATK == true)
	{
		m_bSuperATK = false;
	}

	if (m_bSuperATK == false &&
		m_nCntInputSuperATK >= MAX_ALTERNATELY)
	{
		// �`���[�W����
		for (int i = 0; i < PARTS_MAX; i++)
		{
			m_sMotionFrag[i].bCharge = false;

			if (m_pMotion[i] != NULL)
			{
				m_pMotion[i]->ToggleFinish(true);
			}
		}

		// �K�E�Z���
		m_bSuperATK = true;
	}
}

//==========================================================================
// �r����
//==========================================================================
void CUnion_ArntoArm::ControllLeg(int nIdx)
{
	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̌����擾
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// �ړ��ʎ擾
	float fMove = 2.5f;

	int nLeftArmIdx = CManager::GetInstance()->GetByPlayerPartsType(PARTS_L_ARM);
	int nRightArmIdx = CManager::GetInstance()->GetByPlayerPartsType(PARTS_R_ARM);

	if (m_state != STATE_DEAD &&
		m_state != STATE_FADEOUT)
	{// �ړ��\���[�V�����̎�

		// �ړ�����
		if (ControllMove(nIdx) &&
			m_pMotion[nIdx] != NULL &&
			m_pMotion[nIdx]->GetType() != MOTION_SUPERATK)
		{
			// �ړ����ɂ���
			for (int i = 0; i < PARTS_MAX; i++)
			{
				m_sMotionFrag[i].bMove = true;

				if (m_pMotion[i] == NULL)
				{
					continue;
				}

				//if (m_nControllMoveIdx != i &&
				//	m_pMotion[i]->GetType() != MOTION_WALK &&
				//	m_pMotion[i]->GetType() != MOTION_DEF)
				//{// �ړ���S���Ă�p�[�c�ȊO && �ړ����ĂȂ� && �ҋ@�ł��Ȃ�
				//	m_sMotionFrag[i].bMove = false;
				//}
			}
		}
		else
		{
			for (int i = 0; i < PARTS_MAX; i++)
			{
				m_sMotionFrag[i].bMove = false;
			}
		}


		// �ړ��ʎ擾
		D3DXVECTOR3 move = GetMove();

		if (m_bJump == false &&
			pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LB, nIdx))
		{//SPACE�������ꂽ,�W�����v

			m_bJump = true;
			move.y += 17.0f;

			// �W�����v���ɂ���
			for (int i = 0; i < PARTS_MAX; i++)
			{
				m_sMotionFrag[i].bJump = true;
				if (m_sMotionFrag[nRightArmIdx].bCharge == true)
				{
					m_sMotionFrag[nRightArmIdx].bJump = false;
				}
				if (m_sMotionFrag[nLeftArmIdx].bCharge == true)
				{
					m_sMotionFrag[nLeftArmIdx].bJump = false;
				}
			}

			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_JUMP);
		}

		// �ړ��ʐݒ�
		SetMove(move);
	}

}

//==========================================================================
// �U������
//==========================================================================
void CUnion_ArntoArm::ControllATK(int nIdx, int nLoop)
{
	if (m_pMotion[nIdx] != NULL &&
		m_pMotion[nIdx]->GetType() == MOTION_SUPERATK)
	{
		return;
	}

	if (nLoop < 0)
	{
		return;
	}

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	
	if ((pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, nLoop)))
	{// �U��

		// �`���[�W����
		m_sMotionFrag[nIdx].bCharge = true;

		if (m_nInputSuperAtkIdx == nLoop)
		{// ���͎�t���̃C���f�b�N�X�Ɠ�����������

			// �K�E�Z�̓��͉񐔉��Z
			m_nCntInputSuperATK++;

			// �U���̓��̓J�E���^�[���Z�b�g
			m_nCntInputAtk = INTERVAL_ALTERNATELY;

			// �K�E�Z���͂̃C���f�b�N�X�؂�ւ�
			m_nInputSuperAtkIdx = (m_nInputSuperAtkIdx == m_nPartsIdx[1]) ? m_nPartsIdx[0] : m_nPartsIdx[1];
		}
		else
		{
			// ���̓J�E���^�[���Z�b�g
			m_nCntInputAtk = 0;

			// �K�E�Z�̓��͉񐔃��Z�b�g
			m_nCntInputSuperATK = 0;
		}

	}

	if (m_sMotionFrag[nIdx].bCharge == true &&
		pInputGamepad->GetRelease(CInputGamepad::BUTTON_A, nLoop))
	{// �`���[�W���ɍU���{�^���𗣂�����

		// �U����
		m_sMotionFrag[nIdx].bCharge = false;
		m_sMotionFrag[nIdx].bATK = true;
	}
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CUnion_ArntoArm::MotionSet(int nIdx)
{
	if (m_pMotion[nIdx] == NULL)
	{// ���[�V������NULL��������
		return;
	}

	if (m_pMotion[nIdx]->IsFinish() == true)
	{// �I�����Ă�����

		// ���݂̎�ގ擾
		int nType = m_pMotion[nIdx]->GetType();
		int nOldType = m_pMotion[nIdx]->GetOldType();

		if (m_sMotionFrag[nIdx].bMove == true &&
			m_sMotionFrag[nIdx].bATK == false &&
			m_bKnockBack == false &&
			m_bDead == false &&
			m_sMotionFrag[nIdx].bCharge == false &&
			m_bJump == false)
		{// �ړ����Ă�����


			// �ړ����[�V����
			m_pMotion[nIdx]->Set(MOTION_WALK);
		}
		else if (m_sMotionFrag[nIdx].bJump == true &&
			m_sMotionFrag[nIdx].bATK == false &&
			m_sMotionFrag[nIdx].bCharge == false &&
			m_bKnockBack == false &&
			m_bDead == false)
		{// �W�����v��

			// �W�����v�̃t���OOFF
			m_sMotionFrag[nIdx].bJump = false;

			// �W�����v���[�V����
			m_pMotion[nIdx]->Set(MOTION_JUMP);
		}
		else if (m_bJump == true &&
			m_sMotionFrag[nIdx].bJump == false &&
			m_sMotionFrag[nIdx].bATK == false &&
			m_sMotionFrag[nIdx].bCharge == false &&
			m_bKnockBack == false &&
			m_bDead == false)
		{// �W�����v��&&�W�����v���[�V�������I����Ă鎞

			// �������[�V����
			m_pMotion[nIdx]->Set(MOTION_FALL);
		}
		else if (m_bSuperATK == true)
		{// �K�E�Z����������

			// �K�E�Z�̓��͉񐔃��Z�b�g
			m_nCntInputSuperATK = 0;

			// �K�E�Z���[�V����
			m_pMotion[nIdx]->Set(MOTION_SUPERATK);
		}
		else if (m_sMotionFrag[nIdx].bCharge == true)
		{// �`���[�W����������

			// �`���[�W���[�V����
			m_pMotion[nIdx]->Set(MOTION_CHARGE);
		}
		else if (m_bKnockBack == true)
		{// ���ꒆ��������

			// ���ꃂ�[�V����
			m_pMotion[nIdx]->Set(MOTION_KNOCKBACK);
		}
		else if (m_bDead == true)
		{// ���S����������

			// ���ꃂ�[�V����
			m_pMotion[nIdx]->Set(MOTION_DEAD);
		}
		else if (m_sMotionFrag[nIdx].bATK == true)
		{// �U�����Ă�����

			m_sMotionFrag[nIdx].bATK = false;		// �U������OFF
			m_pMotion[nIdx]->Set(MOTION_ATK, true);
		}
		else
		{
			// �j���[�g�������[�V����
			m_pMotion[nIdx]->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// �U��������
//==========================================================================
void CUnion_ArntoArm::AttackAction(int nIdx, int nModelNum, CMotion::AttackInfo ATKInfo)
{

	// ����̈ʒu
	D3DXVECTOR3 weponpos = m_pMotion[nIdx]->GetAttackPosition(m_pObjChara[nIdx]->GetModel(), ATKInfo);

	// ��ޕ�
	switch (m_pMotion[nIdx]->GetType())
	{
	case MOTION_ATK:
		
		break;

	case MOTION_SUPERATK:
		my_particle::Create(weponpos, my_particle::TYPE_MAGIC_EXPLOSION);
		break;
	}
}

//==========================================================================
// �q�b�g����
//==========================================================================
bool CUnion_ArntoArm::Hit(const int nValue)
{
	// �̗͎擾
	int nLife = 50;

	//if (nLife <= 0)
	//{
	//	// ����
	//	return true;
	//}

	if (m_state != STATE_DMG && m_state != STATE_KNOCKBACK && m_state != STATE_INVINCIBLE && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{// �_���[�W��t��Ԃ̎�

		// �̗͌��炷
		nLife -= nValue;


		// �Q�[���p�b�h���擾
		CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
		pInputGamepad->SetVibration(CInputGamepad::VIBRATION_STATE_DMG, 0);

		m_KnokBackMove.y += 18.0f;
		m_bHitStage = false;

		// �̗͐ݒ�
		//SetLife(nLife);

		if (nLife <= 0)
		{// �̗͂��Ȃ��Ȃ�����

			// �����
			m_state = STATE_DEAD;

			m_KnokBackMove.y = 8.0f;

			// �J�ڃJ�E���^�[�ݒ�
			m_nCntState = DEADTIME;

			// �̗͐ݒ�
			//SetLife(0);

			// �m�b�N�o�b�N����ɂ���
			m_bKnockBack = true;

			// ���ꃂ�[�V����
			m_pMotion[PARTS_BODY]->Set(MOTION_KNOCKBACK);

			// �m�b�N�o�b�N�̈ʒu�X�V
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 rot = GetRotation();
			m_posKnokBack = pos;

			// �Ռ��g����
			CImpactWave::Create
			(
				D3DXVECTOR3(pos.x, pos.y + 80.0f, pos.z),	// �ʒu
				D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// ����
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// �F
				80.0f,										// ��
				80.0f,										// ����
				0.0f,										// ���S����̊Ԋu
				20,											// ����
				10.0f,										// ���̈ړ���
				CImpactWave::TYPE_GIZAWHITE,				// �e�N�X�`���^�C�v
				false										// ���Z�������邩
			);

			CManager::GetInstance()->SetEnableHitStop(18);

			// �U��
			CManager::GetInstance()->GetCamera()->SetShake(21, 30.0f, 0.0f);

			// ����
			return true;
		}

		// �ߋ��̏�ԕۑ�
		m_Oldstate = m_state;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// �_���[�W��Ԃɂ���
		m_state = STATE_DMG;

		// �J�ڃJ�E���^�[�ݒ�
		m_nCntState = 0;

		// �m�b�N�o�b�N�̈ʒu�X�V
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 rot = GetRotation();
		m_posKnokBack = pos;

		// �m�b�N�o�b�N����ɂ���
		m_bKnockBack = true;

		// ���ꃂ�[�V����
		m_pMotion[PARTS_BODY]->Set(MOTION_KNOCKBACK);

		// �Ռ��g����
		CImpactWave::Create
		(
			D3DXVECTOR3(pos.x, pos.y + 80.0f, pos.z),	// �ʒu
			D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// ����
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// �F
			80.0f,										// ��
			80.0f,										// ����
			0.0f,										// ���S����̊Ԋu
			20,											// ����
			10.0f,										// ���̈ړ���
			CImpactWave::TYPE_GIZAWHITE,				// �e�N�X�`���^�C�v
			false										// ���Z�������邩
		);

		CManager::GetInstance()->SetEnableHitStop(12);

		// �U��
		CManager::GetInstance()->GetCamera()->SetShake(12, 25.0f, 0.0f);

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_PLAYERDMG);
	}

	// ����łȂ�
	return false;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CUnion_ArntoArm::Draw(void)
{
	// �`�揈��
	CPlayerUnion::Draw();
}