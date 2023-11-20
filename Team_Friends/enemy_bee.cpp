//=============================================================================
// 
// �n�`�G���� [enemy_bee.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "enemy_bee.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"
#include "input.h"
#include "calculation.h"
#include "bullet.h"
#include "explosion.h"
#include "particle.h"
#include "motion.h"
#include "model.h"
#include "elevation.h"
#include "player.h"
#include "enemymanager.h"
#include "camera.h"
#include "ballast.h"
#include "3D_Effect.h"
#include "hp_gauge.h"
#include "shadow.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WIDTH			(60.0f)							// ����
#define HEIGHT			(60.0f)							// �c��
#define MAX_LIFE		(5)								// �̗�
#define PLAYER_SERCH	(800.0f)	// �v���C���[�T���͈�
#define CHACE_DISTABCE	(50.0f)		// �ǂ��|���鎞�̊Ԋu
#define JUMP			(18.0f)					// �W�����v�͏����l

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEnemyBee::CEnemyBee(int nPriority) : CEnemy(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEnemyBee::~CEnemyBee()
{

}


//==========================================================================
// ����������
//==========================================================================
HRESULT CEnemyBee::Init(void)
{
	// ����������
	CEnemy::Init();

	m_state = STATE_NONE;	// �e�ǂ��|�����
	m_Oldstate = STATE_NONE;

	switch (m_colorType)
	{
	case COLORTYPE_NORMAL:
		break;

	case COLORTYPE_POISON:
		break;

	case COLORTYPE_LUCKY:
		break;

	case COLORTYPE_CONFUSION:
		break;

	case COLORTYPE_SWIFTFOOT:
		break;

	case COLORTYPE_TUTORIAL:
		m_Oldstate = STATE_WAIT;
		break;

	default:
		break;
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEnemyBee::Uninit(void)
{
	// �I������
	CEnemy::Uninit();
}

//==========================================================================
// ���S����
//==========================================================================
void CEnemyBee::Kill(void)
{
	// ���S����
	CEnemy::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEnemyBee::Update(void)
{
	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// �X�V����
	CEnemy::Update();

	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// �v���C���[���
	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);
	if (pPlayer == NULL)
	{
		return;
	}

	// �e�̈ʒu�擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �ڕW�̊p�x�����߂�
	fRotDest = atan2f((pos.x - posPlayer.x), (pos.z - posPlayer.z));

	// �ڕW�Ƃ̍���
	float fRotDiff = fRotDest - rot.y;

	//�p�x�̐��K��
	RotNormalize(fRotDiff);

	//�p�x�̕␳������
	rot.y += fRotDiff * 0.025f;

	// �p�x�̐��K��
	RotNormalize(rot.y);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);

}

//==========================================================================
// ���n���̏���
//==========================================================================
void CEnemyBee::ProcessLanding(void)
{
	// ���n���̏���
	CEnemy::ProcessLanding();
}

//==========================================================================
// �U����Ԉڍs����
//==========================================================================
void CEnemyBee::ChangeToAttackState(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �v���C���[���
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (pPlayer == NULL)
		{
			return;
		}

		// �e�̈ʒu�擾
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		float fRadius = 700.0f;

		if (m_pMotion->GetType() != MOTION_ATK && CircleRange3D(pos, posPlayer, fRadius, pPlayer->GetRadius()) == true)
		{// ��苗���ԂɃv���C���[����������

			// �U����Ԃɂ���
			m_sMotionFrag.bATK = true;

			// �ǂ��|����v���C���[�̔ԍ��ݒ�
			m_nTargetPlayerIndex = nCntPlayer;
			break;
		}
	}
}

//==========================================================================
// �o��
//==========================================================================
void CEnemyBee::Spawn(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState++;

	// �ʒu�X�V
	pos = (D3DXVECTOR3(0.0f, SPAWN_GRAVITY, 0.0f) * (float)(m_nCntState * m_nCntState) + move * (float)m_nCntState) + m_posOrigin;

	// �N���Ƃ̔���
	if (CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos))
	{// �n�ʂƓ������Ă�����
		m_state = m_Oldstate;	// �ߋ��̏��ۑ�
		m_nCntState = 0;	// �J�ڃJ�E���^�[�ݒ�
		move.y = 0.0f;			// �ړ��ʃ[��
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// �U������
//==========================================================================
void CEnemyBee::StateAttack(void)
{
	// �U������
	CEnemy::StateAttack();

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());

	// �U�����̑����擾
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	bool bAtkWait = true;	// �U���ҋ@��
	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == NULL)
		{// NULL��������
			continue;
		}

		// ���[�V�����J�E���^�[�擾
		if (m_pMotion->GetAllCount() > aInfo.AttackInfo[nCntAttack]->nMinCnt)
		{// �U�����蒆

			// �U�����蒆�ɂ���
			bAtkWait = false;
		}
	}

	if (bAtkWait == false)
	{// ���蒆�̎�
		return;
	}


	//// �v���C���[���
	//CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	//if (pPlayer == NULL)
	//{// NULL��������
	//	return;
	//}

	//// �e�̈ʒu�擾
	//D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	//// �ʒu�擾
	//D3DXVECTOR3 pos = GetPosition();

	//// �����擾
	//D3DXVECTOR3 rot = GetRotation();

	//// �ڕW�̌����擾
	//float fRotDest = GetRotDest();

	//// �ڕW�̊p�x�����߂�
	//fRotDest = atan2f((pos.x - posPlayer.x), (pos.z - posPlayer.z));

	//// �ڕW�Ƃ̍���
	//float fRotDiff = fRotDest - rot.y;

	////�p�x�̐��K��
	//RotNormalize(fRotDiff);

	////�p�x�̕␳������
	//rot.y += fRotDiff * 0.025f;

	//// �p�x�̐��K��
	//RotNormalize(rot.y);

	//// �����ݒ�
	//SetRotation(rot);
}

//==========================================================================
// �ǂ��|���ړ�
//==========================================================================
void CEnemyBee::ChaseMove(float fMove)
{
	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �ړ��ʉ��Z
	move.x += sinf(D3DX_PI + rot.y) * fMove;
	move.z += cosf(D3DX_PI + rot.y) * fMove;

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CEnemyBee::MotionSet(void)
{
	if (m_pMotion->IsFinish() == true)
	{// �I�����Ă�����

		// ���݂̎�ގ擾
		int nType = m_pMotion->GetType();

		if (m_sMotionFrag.bMove == true && m_sMotionFrag.bKnockback == false)
		{// �ړ����Ă�����

			m_sMotionFrag.bMove = false;	// �ړ�����OFF

			// �ړ����[�V����
			m_pMotion->Set(MOTION_WALK);
		}
		else if (m_sMotionFrag.bKnockback == true)
		{// ���ꒆ��������

			// ���ꃂ�[�V����
			m_pMotion->Set(MOTION_KNOCKBACK);
		}
		else if (m_sMotionFrag.bATK == true)
		{// �U�����Ă�����

			m_sMotionFrag.bATK = false;		// �U������OFF

			// �U�����[�V����
			m_pMotion->Set(MOTION_ATK);
		}
		else
		{
			// �j���[�g�������[�V����
			m_pMotion->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// �U��������
//==========================================================================
void CEnemyBee::AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo)
{
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// �U��������
	CEnemy::AttackAction(nModelNum, ATKInfo);

	if (GetObjectChara()->GetModel()[nModelNum] == NULL)
	{// NULL��������
		return;
	}

	// ����̈ʒu
	D3DXVECTOR3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);

	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);

	if (pPlayer == NULL)
	{// NULL��������
		return;
	}

	// �v���C���[�̈ʒu
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �v���C���[�ƓG�̃x�N�g��
	D3DXVECTOR3 vec = weponpos - posPlayer;

	// �ڕW�̊p�x�����߂�
	float fRotDest = atan2f(vec.x, vec.z);
	float fRotHeight = atan2f((weponpos.y - posPlayer.y - 100.0f), D3DXVec3Length(&vec));

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ʒu�擾
	D3DXVECTOR3 rot = GetRotation();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CEnemyBee::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();
}

//==========================================================================
// �G�̏��擾
//==========================================================================
CEnemyBee *CEnemyBee::GetEnemy(void)
{
	// �������g�̃|�C���^���擾
	return this;
}