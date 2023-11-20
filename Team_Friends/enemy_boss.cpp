//=============================================================================
// 
//  �{�X���� [enemy_boss.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "enemy_boss.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"
#include "input.h"
#include "calculation.h"
#include "bullet.h"
#include "bullet_obstacle.h"
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
#include "stage.h"
#include "objectX.h"
#include "debugproc.h"
#include "fade.h"
#include "enemybase.h"
#include "effect_enemyspawn.h"
#include "impactwave.h"
#include "blackframe.h"
#include "gamemanager.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WIDTH			(60.0f)							// ����
#define HEIGHT			(60.0f)							// �c��
#define MAX_LIFE		(5)								// �̗�
#define PLAYER_SERCH	(800.0f)	// �v���C���[�T���͈�
#define CHACE_DISTABCE	(50.0f)		// �ǂ��|���鎞�̊Ԋu
#define JUMP			(18.0f)					// �W�����v�͏����l
#define BASECHANGETIME	(120)	// ���_�؂�ւ�����
#define PLAYERCHASETIME	(60 * 6 + 120)	// �e�ǂ��|������
#define WAITTIME	(60 * 2)	// �ҋ@����
#define BULLETCOUNT		(3)		// �e�̉�
#define PIYOPIYOTIME	(60 * 5)	// �s���s���̎���
#define STUNKNOCKBACK_TIME	(90)	// ���_�؂�ւ�����

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEnemyBoss::CEnemyBoss(int nPriority) : CEnemy(nPriority)
{
	// �l�̃N���A
	memset(&m_sAct, 0, sizeof(m_sAct));
	m_sAct.nKnockBackCnt = 0;
	m_sAct.nBulletCnt = 0;		// �e�̃J�E���^�[
	m_sAct.nAssultAngle = 1;	// �ːi�̌���
	m_sAct.AtkType = ATKTYPE_BULLET;	// �U���̎��
	m_sAct.StunPosDest = mylib_const::DEFAULT_VECTOR3;	// �X�^�����̖ڕW�̈ʒu
	m_sAct.nKnockBackCnt = 0;
	m_BaseType = BASETYPE_ORIGIN;	// ���_�̎��
	m_BaseTypeDest = BASETYPE_ORIGIN;	// �ڕW�̋��_���
	m_nCntDamage = 0;			// �_���[�W�J�E���^�[
	m_nCntEmission = 0;			// �������̃J�E���^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEnemyBoss::~CEnemyBoss()
{

}


//==========================================================================
// ����������
//==========================================================================
HRESULT CEnemyBoss::Init(void)
{
	// ����������
	CEnemy::Init();

	m_state = STATE_NONE;	// �e�ǂ��|�����
	m_Oldstate = STATE_NONE;
	m_ActType = ACTTYPE_BOSS;

	// �ҋ@����
	m_nCntState = WAITTIME;
	m_state = STATE_ATTACK;
	m_sMotionFrag.bATK = false;
	m_sAct.AtkType = ATKTYPE_APPEARANCE;

	m_pMotion->Set(MOTION_APPEARANCE);

	// �z�o�����O��
	CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_BGM_HOBARING);
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_HOBARING);


	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEnemyBoss::Uninit(void)
{
	// �I������
	CEnemy::Uninit();

	// ���[�h�ݒ�
	if (CGame::IsEdit() == false)
	{
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);
		CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_BGM_HOBARING);
	}
}

//==========================================================================
// ���S����
//==========================================================================
void CEnemyBoss::Kill(void)
{
	// ���S����
	CEnemy::Kill();
	CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_BGM_HOBARING);
}

//==========================================================================
// �q�b�g����
//==========================================================================
bool CEnemyBoss::Hit(const int nValue)
{
	// �����擾
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 pos = GetPosition();

	// �̗͎擾
	int nLife = GetLife();

	if (m_nCntDamage <= 0 && m_sAct.AtkType != ATKTYPE_STUNKNOCKBACK)
	{// �Ȃɂ��Ȃ���Ԃ̎�

		// �̗͌��炷
		if (nValue == mylib_const::DMG_SLASH)
		{
			nLife--;
		}
		else
		{
			nLife -= nValue;
		}

		// �̗͐ݒ�
		SetLife(nLife);

		if (nLife > 0)
		{// �̗͂��Ȃ��Ȃ��ĂȂ�������

			// �_���[�W���Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DMG01);

			if (m_pHPGauge == NULL)
			{
				// �̗̓Q�[�W
				m_pHPGauge = CHP_Gauge::Create(640.0f, GetLifeOrigin(), 10.0f);

				if (m_pHPGauge == NULL)
				{// NULL��������
					m_pHPGauge = NULL;
				}
			}
		}

		if (nLife <= 0)
		{// �̗͂��Ȃ��Ȃ�����

			// �q�b�g�X�g�b�v
			CManager::GetInstance()->SetEnableHitStop(5);

			// �U��
			CManager::GetInstance()->GetCamera()->SetShake(5, 10.0f, 0.0f);

			// ���S��Ԃɂ���
			m_state = STATE_DEAD;

			// �J�ڃJ�E���^�[�ݒ�
			m_nCntState = 40;

			// �m�b�N�o�b�N�̈ʒu�X�V
			m_posKnokBack = GetPosition();

			// �m�b�N�o�b�N����ɂ���
			m_sMotionFrag.bKnockback = true;

			// ���ꃂ�[�V����
			//m_pMotion->Set(MOTION_KNOCKBACK);

			// ��������
			return true;
		}

		// �␳
		ValueNormalize(nLife, GetLifeOrigin(), 0);

		// �m�b�N�o�b�N�̈ʒu�X�V
		m_posKnokBack = GetPosition();

		// �J�ڃJ�E���^�[�ݒ�
		if (nValue == mylib_const::DMG_SLASH)
		{
			m_nCntDamage = 10;

			// �U��
			CManager::GetInstance()->GetCamera()->SetShake(5, 8.0f, 0.0f);

			if (m_pMotion->GetType() == MOTION_STUN)
			{
				// �q�b�g�X�g�b�v
				CManager::GetInstance()->SetEnableHitStop(5);

				// �U��
				CManager::GetInstance()->GetCamera()->SetShake(10, 15.0f, 0.0f);
			}

		}
		else
		{// �e�����Ԃ��̃_���[�W�̎�
			m_nCntDamage = 20;

			if (m_sAct.AtkType == ATKTYPE_ASSULT)
			{// �ːi���̎�

				// �X�^����Ԃɂ���
				m_state = STATE_ATTACK;
				m_nCntState = PIYOPIYOTIME;
				m_sAct.AtkType = ATKTYPE_STUN;
			}
			else if(m_sAct.AtkType == ATKTYPE_BULLET || m_sAct.AtkType == ATKTYPE_ENEMYSPAWN)
			{

				// �X�^���m�b�N�o�b�N��Ԃɂ���
				m_state = STATE_ATTACK;
				m_nCntState = STUNKNOCKBACK_TIME;
				m_sAct.AtkType = ATKTYPE_STUNKNOCKBACK;
				m_BaseType = BASETYPE_MAP;
				m_sAct.nKnockBackCnt = 0;

				// �X�^���̖ړI�n����
				m_sAct.StunPosDest = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

				D3DXVECTOR3 move = GetMove();
				move.y = 35.0f;
				SetMove(move);
				m_posKnokBack.y = m_sAct.StunPosDest.y;

			}

			// �q�b�g�X�g�b�v
			CManager::GetInstance()->SetEnableHitStop(5);

			// �U��
			CManager::GetInstance()->GetCamera()->SetShake(10, 15.0f, 0.0f);
		}

		// ��������
		return true;
	}

	// ����łȂ�
	return false;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEnemyBoss::Update(void)
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


	// �U���ʏ���
	//UpdateByAttack();

	// �������̃J�E���^�[�X�V
	m_nCntEmission = (m_nCntEmission + 1) % 14;
	if (m_nCntEmission == 0)
	{
		// �ʒu�擾
		D3DXVECTOR3 pos = GetPosition();

		// �Ռ��g����
		CImpactWave::Create
		(
			D3DXVECTOR3(pos.x, pos.y, pos.z),	// �ʒu
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ����
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),	// �F
			100.0f,								// ��
			100.0f,								// ����
			0.0f,								// ���S����̊Ԋu
			8,									// ����
			40.0f,								// ���̈ړ���
			CImpactWave::TYPE_SMOKE,			// �e�N�X�`���^�C�v
			false								// ���Z�������邩
		);
	}

	// �_���[�W�J�E���^�[
	m_nCntDamage--;

	if (m_nCntDamage <= 0)
	{
		m_nCntDamage = 0;
	}
	else
	{
		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, m_mMatcol.a);
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[�{�X�̏��]------------------\n"
		"�̗́F�y%d�z\n", GetLife());

}

//==========================================================================
// �v���C���[�Ƃ̓����蔻��
//==========================================================================
void CEnemyBoss::CollisionPlayer(void)
{
	// �v���C���[�̎擾
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{
			return;
		}

		if (m_state == STATE_SPAWN || m_state == STATE_DEAD || m_state == STATE_FADEOUT)
		{
			return;
		}

		// �����̏��擾
		D3DXVECTOR3 pos = GetPosition();
		float fRadius = GetRadius();

		if (m_sAct.AtkType == ATKTYPE_STUN ||
			m_sAct.AtkType == ATKTYPE_STUNKNOCKBACK)
		{
			fRadius *= 0.5f;
		}

		// �v���C���[���擾
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
		float PlayerRadius = pPlayer->GetRadius();
		CPlayer::STATE PlayerState = (CPlayer::STATE)pPlayer->GetState();

		// ���̔���
		if (SphereRange(pos, PlayerPos, fRadius, PlayerRadius) &&
			PlayerState != CPlayer::STATE_DEAD &&
			PlayerState != CPlayer::STATE_DMG &&
			PlayerState != CPlayer::STATE_KNOCKBACK &&
			PlayerState != CPlayer::STATE_INVINCIBLE)
		{

			// �q�b�g����
			if (pPlayer->Hit(1) == false)
			{// ����łȂ�������

				// ������шړ��ʐݒ�
				pPlayer->SetMove(D3DXVECTOR3(8.0f, 0.0f, 0.0f));
			}
		}
	}
}

//==========================================================================
// ��ޕʍX�V����
//==========================================================================
void CEnemyBoss::UpdateByType(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// X�t�@�C���Ƃ̔���
	CStage *pStage = CGame::GetStage();
	if (pStage == NULL)
	{// NULL��������
		return;
	}

	bool bNowLand = false;
	for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
	{
		// �I�u�W�F�N�g�擾
		CObjectX *pObjX = pStage->GetObj(nCntStage);

		if (pObjX == NULL)
		{// NULL��������
			continue;
		}

		// �����擾
		bool bLand = false;
		float fHeight = pObjX->GetHeight(pos, bLand);

		if (bLand == true && fHeight > pos.y)
		{// �n�ʂ̕���������荂��������

			// �n�ʂ̍����ɕ␳
			pos.y = fHeight;

			if (bLand == true)
			{// ���n���Ă���

				// �W�����v�g�p�\�ɂ���
				move.y = 0.0f;
				bNowLand = true;
			}
		}
	}


	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[�{�X���]------------------\n"
		"�s���̎�ށF�y%d�z\n"
		"�ڕW�̋��_�F�y%d�z\n", m_ActType, m_BaseTypeDest);
}

//==========================================================================
// �U���ʏ���
//==========================================================================
void CEnemyBoss::UpdateByAttack(void)
{
	switch (m_sAct.AtkType)
	{
	case ATKTYPE_BULLET:
		UpdateAttackBullet();
		break;

	case ATKTYPE_ASSULT:
		UpdateAttackAssult();
		break;

	case ATKTYPE_ENEMYSPAWN:
		UpdateChildSpawn();
		break;

	case ATKTYPE_STUNKNOCKBACK:
		UpdateKnockBackStun();
		break;

	case ATKTYPE_STUN:
		UpdateStun();
		break;

	case ATKTYPE_APPEARANCE:
		UpdateAppearance();
		break;

	default:
		break;
	}
}

//==========================================================================
// �o��
//==========================================================================
void CEnemyBoss::UpdateAppearance(void)
{
	int nType = m_pMotion->GetType();
	if (nType == MOTION_APPEARANCE && m_pMotion->IsFinish() == true)
	{// �o�ꉉ�o���I����Ă���

		m_state = STATE_WAIT;	// �e�ǂ��|�����
		m_Oldstate = STATE_NONE;

		// �ҋ@����
		m_nCntState = WAITTIME;
		m_state = STATE_WAIT;
		m_sMotionFrag.bATK = false;
		m_sAct.nKnockBackCnt = 0;

		// ���t���[���J����
		CManager::GetInstance()->GetBlackFrame()->SetState(CBlackFrame::STATE_OUT);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_BOSSGAME);

		// �J�����擾
		CCamera *pCamera = CManager::GetInstance()->GetCamera();
		D3DXVECTOR3 CameraRotation = pCamera->GetDestRotation();

		pCamera->SetDestRotation(D3DXVECTOR3(CameraRotation.x, CameraRotation.y, -0.25f));

		// ����\��Ԃɂ���
		CGame::GetGameManager()->SetEnableControll(true);
		return;
	}

	// �J�����擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetLenDest(pCamera->GetOriginDistance() - 500.0f, 10, 2.0f, 0.01f);
	D3DXVECTOR3 CameraRotation = pCamera->GetDestRotation();
	pCamera->SetDestRotation(D3DXVECTOR3(CameraRotation.x, CameraRotation.y, -0.1f));

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

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
	rot.y += (fRotDest - rot.y) * 0.1f;
	SetRotation(rot);
	

	m_sAct.nKnockBackCnt++;

	if (m_sAct.nKnockBackCnt >= 100)
	{
		m_sAct.nKnockBackCnt = 100;
	}

	pos.x = Lerp(m_posKnokBack.x, 0.0f, (float)m_sAct.nKnockBackCnt / (float)100);
	pos.y = Lerp(m_posKnokBack.y, 1800.0f, (float)m_sAct.nKnockBackCnt / (float)100);
	pos.z = Lerp(m_posKnokBack.z, 0.0f, (float)m_sAct.nKnockBackCnt / (float)100);

	SetPosition(pos);


	// �o�ꃂ�[�V�����ݒ�
	m_pMotion->Set(MOTION_APPEARANCE);
}

//==========================================================================
// �e�U��
//==========================================================================
void CEnemyBoss::UpdateAttackBullet(void)
{
	int nType = m_pMotion->GetType();
	if (nType == MOTION_BULLETATK && m_pMotion->IsFinish() == true)
	{// �e�U�����I����Ă���

		// �e�̃J�E���^�[���Z
		m_sAct.nBulletCnt++;

		if (BULLETCOUNT <= m_sAct.nBulletCnt)
		{//�@�e�̉񐔂ɖ������Ă�����

			// �ҋ@����
			m_nCntState = WAITTIME;
			m_state = STATE_WAIT;
			m_sMotionFrag.bATK = false;

			// �ʏ탂�[�V�����ݒ�
			m_pMotion->Set(MOTION_DEF);
		}
		else
		{// �܂��e�̉񐔂��c���Ă�ꍇ
			m_state = STATE_ATTACK;

			// �ʏ탂�[�V�����ݒ�
			m_pMotion->Set(MOTION_DEF);
		}
		return;
	}
	
	if (nType != MOTION_BULLETATK)
	{// �e�U������Ȃ��ꍇ

		switch (m_sAct.AtkType)
		{
		case ATKTYPE_BULLET:
			// �e�U�����[�V�����ݒ�
			m_pMotion->Set(MOTION_BULLETATK);

			// �e����
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_BOSSBULLET_CHARGE);
			break;
		}
	}

	// �U�����ɂ���
	m_sMotionFrag.bATK = true;

}

//==========================================================================
// �ːi�U��
//==========================================================================
void CEnemyBoss::UpdateAttackAssult(void)
{
	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// �ʏ탂�[�V�����ݒ�
		m_pMotion->Set(MOTION_DEF);
		return;
	}

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());

	// �U�����̑����擾
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	// �Ռ��g����
	if (m_nCntState % 15 == 0 && m_nCntState <= PLAYERCHASETIME - 120)
	{
		// �ʒu�擾
		D3DXVECTOR3 weponpos = GetPosition();

		for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
		{
			if (aInfo.AttackInfo[nCntAttack] == NULL)
			{// NULL��������
				continue;
			}

			// ���f�����擾
			CModel **pModel = GetObjectChara()->GetModel();
			D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

			// ����̃}�g���b�N�X�擾
			D3DXMATRIX mtxWepon;
			D3DXMatrixIdentity(&mtxWepon);

			if (pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum] == NULL)
			{// NULL��������
				return;
			}

			// ���肷��p�[�c�̃}�g���b�N�X�擾
			mtxWepon = pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum]->GetWorldMtx();

			// �ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z);
			D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

			// ����̈ʒu
			weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);
		}

		// �����擾
		D3DXVECTOR3 rot = GetRotation();

		CImpactWave::Create
		(
			weponpos,	// �ʒu
			D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI + rot.y, 0.0f),		// ����
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f),	// �F
			150.0f,								// ��
			150.0f,								// ����
			0.0f,								// ���S����̊Ԋu
			15,									// ����
			25.0f,								// ���̈ړ���
			CImpactWave::TYPE_ORANGE2,			// �e�N�X�`���^�C�v
			true								// ���Z�������邩
		);
	}

	// �e�U�����[�V�����ݒ�
	m_pMotion->Set(MOTION_ASSULTATK);
}

//==========================================================================
// �q���o���U��
//==========================================================================
void CEnemyBoss::UpdateChildSpawn(void)
{
	int nType = m_pMotion->GetType();
	if (nType == MOTION_CHILDSPAWN && m_pMotion->IsFinish() == true)
	{// �o���U�����I����Ă���

		// �ːi�Ɉڂ�
		m_state = STATE_BASECHANGE;
		m_nCntState = BASECHANGETIME;
		m_BaseTypeDest = BASETYPE_MAP;

		// �ʏ탂�[�V�����ݒ�
		m_pMotion->Set(MOTION_DEF);
		return;
	}
	
	if (nType != MOTION_CHILDSPAWN)
	{// �q���o������Ȃ��ꍇ

		// �q���o�����[�V�����ݒ�
		m_pMotion->Set(MOTION_CHILDSPAWN);
	}
}

//==========================================================================
// �s���s���U��(?)
//==========================================================================
void CEnemyBoss::UpdateStun(void)
{
	if (m_nCntState == PIYOPIYOTIME)
	{
		// �X�^����
		CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_STUN);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_STUN);
		CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_ASSULT);
	}

	m_nCntState--;
	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// �ҋ@����
		m_nCntState = WAITTIME;
		m_state = STATE_WAIT;
		m_sMotionFrag.bATK = false;

		// �X�^����
		CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_STUN);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_STUNHEAL);

		// �ʏ탂�[�V�����ݒ�
		m_pMotion->Set(MOTION_DEF);
		return;
	}
	
	// �X�^�����[�V�����ݒ�
	m_pMotion->Set(MOTION_STUN);
}

//==========================================================================
// �X�^���̃m�b�N�o�b�N
//==========================================================================
void CEnemyBoss::UpdateKnockBackStun(void)
{

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	if (m_nCntState <= 0)
	{
		// �X�^����Ԃɂ���
		m_state = STATE_ATTACK;
		m_nCntState = PIYOPIYOTIME;
		m_sAct.AtkType = ATKTYPE_STUN;
		return;
	}

	// �X�^�����[�V�����ݒ�
	m_pMotion->Set(MOTION_STUN);

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �m�b�N�o�b�N���ԉ��Z
	m_sAct.nKnockBackCnt++;

	// �������ړ�
	pos = GetParabola(m_posKnokBack, m_sAct.StunPosDest, 600.0f, (float)m_sAct.nKnockBackCnt / (float)STUNKNOCKBACK_TIME);

	// �ʒu�ݒ�
	SetPosition(pos);
}

//==========================================================================
// ���n���̏���
//==========================================================================
void CEnemyBoss::ProcessLanding(void)
{
	// ���n���̏���
	CEnemy::ProcessLanding();
}

//==========================================================================
// �U����Ԉڍs����
//==========================================================================
void CEnemyBoss::ChangeToAttackState(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	float fRadius = 500.0f;

	//if (CircleRange3D(pos, posPlayer, fRadius, pPlayer->GetRadius()) == true && m_sMotionFrag.bJump == false)
	{// ��苗���ԂɃv���C���[����������

		// �U����Ԃɂ���
		m_state = STATE_ATTACK;
		m_sMotionFrag.bATK = true;
	}
}

//==========================================================================
// �o��
//==========================================================================
void CEnemyBoss::Spawn(void)
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
// �ҋ@����
//==========================================================================
void CEnemyBoss::StateWait(void)
{

	// �v���C���[���
	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);
	if (pPlayer == NULL)
	{// NULL��������
		return;
	}

	// �e�̈ʒu�擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

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


	// �ҋ@����
	m_nCntState--;
	if (m_nCntState <= 0)
	{// �[���ɂȂ�����

		// �s�����I
		DrawingACT();
	}
}

//==========================================================================
// �v���C���[�Ǐ]
//==========================================================================
void CEnemyBoss::PlayerChase(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	// ���݂ƖڕW�̍���
	float fRotDiff = 0.0f;

	// �����̔���
	bool bLen = false;

	// �v���C���[���
	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);

	// �I�u�W�F�N�g���
	CObject *pObj = NULL;

	if (m_nCntState == PLAYERCHASETIME)
	{
		// �ːi�J�n��
		CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_ASSULTEND);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_ASSULT);
	}

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// �ҋ@����
		m_nCntState = WAITTIME;
		m_state = STATE_WAIT;

		// �ːi�I����
		CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_ASSULT);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_ASSULTEND);

		// �ʏ탂�[�V�����ݒ�
		m_pMotion->Set(MOTION_DEF);
		return;
	}

	// �v���C���[�̕������鏈��
	//RotPlayer();

	// �U���ʏ���
	UpdateByAttack();

	if (m_nCntState <= PLAYERCHASETIME - 120)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// �ǂ��|���ړ�����
		ChaseMove(fMove);
	}

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);
}

//==========================================================================
// �U������
//==========================================================================
void CEnemyBoss::StateAttack(void)
{
	
	// �U���ʏ���
	UpdateByAttack();

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
}

//==========================================================================
// �ǂ��|���ړ�
//==========================================================================
void CEnemyBoss::ChaseMove(float fMove)
{
	// �v���C���[���
	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);
	if (pPlayer == NULL)
	{
		return;
	}

	// �e�̈ʒu�擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	CObject *pMyObj = GetObject();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �v���C���[�̕�������
	RotPlayer();

	// �ړ������ʉ��Z
	move.x += fMove * m_sAct.nAssultAngle;

	// �ʒu�X�V
	pos += move;

	// �ړ��ʐݒ�
	SetMove(move);

	// �ʒu�ݒ�
	SetPosition(pos);
}

//==========================================================================
// �_���[�W
//==========================================================================
void CEnemyBoss::Damage(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	// ���݂ƖڕW�̍���
	float fRotDiff = 0.0f;

	// �����̔���
	bool bLen = false;

#if _DEBUG
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
#endif

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// �ߋ��̏�Ԃɂ���
		m_state = STATE_WAIT;
		m_nCntState = 60;
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);
}

//==========================================================================
// ���_�؂�ւ�
//==========================================================================
void CEnemyBoss::ChangeBase(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ڕW�̈ʒu
	D3DXVECTOR3 DestPosition = GetPosition();

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �ڕW�ʏ���
	switch (m_BaseTypeDest)
	{
	case CEnemy::BASETYPE_MAP:
		DestPosition = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
		pos.x = Lerp(0.0f, DestPosition.x, 1.0f - ((float)m_nCntState / (float)BASECHANGETIME));
		pos.z = Lerp(0.0f, DestPosition.z, 1.0f - ((float)m_nCntState / (float)BASECHANGETIME));
		break;

	case CEnemy::BASETYPE_ORIGIN:
		DestPosition = pos;
		pos.x = Lerp(DestPosition.x, 0.0f, 1.0f - ((float)m_nCntState / (float)BASECHANGETIME));
		pos.z = Lerp(DestPosition.z, 0.0f, 1.0f - ((float)m_nCntState / (float)BASECHANGETIME));
		break;
	}

	// �v���C���[�̕������鏈��
	RotPlayer();

	// �ʒu�ݒ�
	SetPosition(pos);

	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		switch (m_BaseTypeDest)
		{
		case CEnemy::BASETYPE_MAP:
			m_sAct.AtkType = ATKTYPE_ASSULT;
			m_state = STATE_PLAYERCHASE;
			m_nCntState = PLAYERCHASETIME;
			break;

		case CEnemy::BASETYPE_ORIGIN:
			m_sAct.AtkType = ATKTYPE_BULLET;
			m_state = STATE_ATTACK;
			break;
		}
		m_BaseType = m_BaseTypeDest;
		//m_state = STATE_WAIT;
	}
}

//==========================================================================
// �s�����I
//==========================================================================
void CEnemyBoss::DrawingACT(void)
{
	while (1)
	{
		// �s�����I
		m_sAct.AtkType = (ATKTYPE)(rand() % ATKTYPE_MAX);

		if (m_sAct.AtkType != ATKTYPE_ENEMYSPAWN &&
			m_sAct.AtkType != ATKTYPE_STUN)
		{// �G�o�� && �X�^���ȊO
			break;
		}
	}

	// �e�̉񐔃��Z�b�g
	m_sAct.nBulletCnt = 0;

	// ���̍s����
	switch (m_sAct.AtkType)
	{
	case ATKTYPE_BULLET:
		if (m_BaseType == BASETYPE_MAP)
		{// �}�b�v�ňړ����Ă�ꍇ
			m_state = STATE_BASECHANGE;
			m_nCntState = BASECHANGETIME;
			m_BaseTypeDest = BASETYPE_ORIGIN;
		}
		else
		{// ���S�ɂ���ꍇ
			m_state = STATE_ATTACK;
		}
		break;

	case ATKTYPE_ASSULT:	// �ːi

		if (m_BaseType == BASETYPE_MAP)
		{// �}�b�v�ňړ����Ă�ꍇ
			m_state = STATE_PLAYERCHASE;
			m_nCntState = PLAYERCHASETIME;
		}
		else
		{// ���S�ɂ���ꍇ

			m_state = STATE_ATTACK;
			m_sAct.AtkType = ATKTYPE_ENEMYSPAWN;

			/*m_state = STATE_BASECHANGE;
			m_nCntState = BASECHANGETIME;
			m_BaseTypeDest = BASETYPE_MAP;*/
		}
		m_sAct.nAssultAngle = 1;	// �ːi�̌���

		if (Random(0, 1) == 0)
		{
			m_sAct.nAssultAngle = -1;	// �ːi�̌���
		}

		break;

	case ATKTYPE_ENEMYSPAWN:
		break;

	default:
		break;
	}
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CEnemyBoss::MotionSet(void)
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
	}
}

//==========================================================================
// �U��������
//==========================================================================
void CEnemyBoss::AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo)
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

	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);

	if (pPlayer == NULL)
	{// NULL��������
		return;
	}

	// �v���C���[�̈ʒu
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();


	// �ڕW�̊p�x�����߂�
	float fRotDest = atan2f((weponpos.x - posPlayer.x), (weponpos.z - posPlayer.z));

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ʒu�擾
	D3DXVECTOR3 rot = GetRotation();

	int nType = m_pMotion->GetType();

	CObject *pBullet = NULL;
	CObject *pBulletObstacle = NULL;
	D3DXVECTOR3 spawnpos = D3DXVECTOR3(pos.x, pos.y + 300.0f, pos.z);
	switch (nType)
	{
	case MOTION_BULLETATK:
	{
		// �e����
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_BOSSBULLET);

		int nCircleDivision = 15;
		for (int i = 0; i < nCircleDivision + 1; i++)
		{
			float fRot = ((D3DX_PI * 2.0f) / (float)nCircleDivision) * i;

			// �ǂ̎�ނ̒e���o�������I
			if (Random(0, 3) == 0)
			{
				// �e�̐���
				pBullet = CBullet::Create(
					CBullet::TYPE_ENEMY,
					CBullet::MOVETYPE_NORMAL,
					spawnpos,
					rot,
					D3DXVECTOR3(sinf(D3DX_PI + fRot) * 10.0f, 0.0f, cosf(D3DX_PI + fRot) * 10.0f),
					80.0f);
			}
			else
			{
				// ��Q���e�̐���
				pBulletObstacle = CBulletObstacle::Create(
					spawnpos,
					rot,
					D3DXVECTOR3(sinf(D3DX_PI + fRot) * 10.0f, 0.0f, cosf(D3DX_PI + fRot) * 10.0f),
					80.0f);
			}
		}
	}
		break;

	case MOTION_CHILDSPAWN:

		// �{�X�g�[�N
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_BOSSTALK);
		break;
	}
	
}

//==========================================================================
// �v���C���[�̕�������
//==========================================================================
void CEnemyBoss::RotPlayer(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// ���݂ƖڕW�̍���
	float fRotDiff = 0.0f;

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
	fRotDiff = fRotDest - rot.y;

	//�p�x�̐��K��
	RotNormalize(fRotDiff);

	//�p�x�̕␳������
	rot.y += fRotDiff * 0.025f;

	// �p�x�̐��K��
	RotNormalize(rot.y);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);

	// �����ݒ�
	SetRotation(rot);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CEnemyBoss::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();
}

//==========================================================================
// �G�̏��擾
//==========================================================================
CEnemyBoss *CEnemyBoss::GetEnemy(void)
{
	// �������g�̃|�C���^���擾
	return this;
}