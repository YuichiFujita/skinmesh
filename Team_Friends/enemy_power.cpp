//=============================================================================
// 
//  �v���C���[���� [enemy.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "enemy_power.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"
#include "input.h"
#include "calculation.h"
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
CEnemyPower::CEnemyPower(int nPriority) : CEnemy(nPriority)
{
	// �l�̃N���A
	m_attackType = ATTACKTYPE_IMPACTWAVE;	// �U���̎��

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEnemyPower::~CEnemyPower()
{

}


//==========================================================================
// ����������
//==========================================================================
HRESULT CEnemyPower::Init(void)
{
	// ����������
	CEnemy::Init();

	// �̗͎擾
	int nLife = GetLife();

	// �̗̓Q�[�W
	m_pHPGauge = CHP_Gauge::Create(350.0f, nLife, 3.0f);

	if (m_pHPGauge == NULL)
	{// NULL��������
		m_pHPGauge = NULL;
	}

	// �U���̎��
	m_attackType = ATTACKTYPE_IMPACTWAVE;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEnemyPower::Uninit(void)
{
	// �I������
	CEnemy::Uninit();
}

//==========================================================================
// ���S����
//==========================================================================
void CEnemyPower::Kill(void)
{
	// ���S����
	CEnemy::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEnemyPower::Update(void)
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


	// ���݂̎�ގ擾
	int nType = m_pMotion->GetType();

	if (nType == MOTION_DIRECTATK)
	{
		// �ʒu�擾
		D3DXVECTOR3 pos = GetPosition();

		// �����擾
		D3DXVECTOR3 rot = GetRotation();

		// �ڕW�̌����擾
		float fRotDest = GetRotDest();

		// �v���C���[���
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);

		if (pPlayer == NULL)
		{// NULL��������
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
	}
}

//==========================================================================
// ���n���̏���
//==========================================================================
void CEnemyPower::ProcessLanding(void)
{
	// ���n���̏���
	CEnemy::ProcessLanding();
}

//==========================================================================
// �U����Ԉڍs����
//==========================================================================
void CEnemyPower::ChangeToAttackState(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �v���C���[���
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);

		if (pPlayer == NULL)
		{
			return;
		}

		// �e�̈ʒu�擾
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		// �E�O
		D3DXVECTOR3 posLeft;
		D3DXVECTOR3 posRight;

		float fSearchCircle = 400.0f;

		if (m_sMotionFrag.bATK == false &&
			CircleRange3D(pos, posPlayer, fSearchCircle, pPlayer->GetRadius()) == true &&
			m_sMotionFrag.bJump == false)
		{// ��苗���ԂɃv���C���[����������

			// �U����Ԃɂ���
			m_state = STATE_ATTACK;
			m_sMotionFrag.bATK = true;
			m_attackType = ATTACKTYPE_IMPACTWAVE;

			// �ǂ��|����v���C���[�̔ԍ��ݒ�
			m_nTargetPlayerIndex = nCntPlayer;
			return;
		}
	}
}

//==========================================================================
// �o��
//==========================================================================
void CEnemyPower::Spawn(void)
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
	if (CGame::GetElevation()->IsHit(pos))
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
// �ǂ��|���ړ�
//==========================================================================
void CEnemyPower::ChaseMove(float fMove)
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
void CEnemyPower::MotionSet(void)
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
			switch (m_attackType)
			{
			case CEnemyPower::ATTACKTYPE_IMPACTWAVE:
				m_pMotion->Set(MOTION_ATK);
				break;

			case CEnemyPower::ATTACKTYPE_DIRECTWAVE:
				m_pMotion->Set(MOTION_DIRECTATK);
				break;

			case CEnemyPower::ATTACKTYPE_MAX:
				break;

			default:
				break;
			}
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
void CEnemyPower::AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo)
{
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// �U��������
	CEnemy::AttackAction(nModelNum, ATKInfo);

	if (GetObjectChara()->GetModel()[nModelNum] == NULL)
	{// NULL��������
		return;
	}

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	D3DXMATRIX mtxWepon = GetObjectChara()->GetModel()[nModelNum]->GetWorldMtx();

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, ATKInfo.Offset.x, ATKInfo.Offset.y, ATKInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	// ����̈ʒu
	D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// ��ޕ�
	switch (m_pMotion->GetType())
	{
	case MOTION_WALK:
		break;

	case MOTION_ATK:

		// �p�[�e�B�N������
		my_particle::Create(weponpos, my_particle::TYPE_SUPERATTACK);

		// ���I����
		CBallast::Create(weponpos, DEFAULTMOVE_BALLAST, 5);

		// �U��
		CManager::GetInstance()->GetCamera()->SetShake(20, 5.0f, 0.0f);
		break;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CEnemyPower::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();
}

//==========================================================================
// �G�̏��擾
//==========================================================================
CEnemyPower *CEnemyPower::GetEnemy(void)
{
	// �������g�̃|�C���^���擾
	return this;
}