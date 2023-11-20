//=============================================================================
// 
// �Q�[���}�l�[�W������ [gamemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "instantfade.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "enemybase.h"
#include "enemymanager.h"
#include "stage.h"
#include "injectiontable.h"
#include "player_union.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameManager::CGameManager()
{
	// �l�̃N���A
	m_SceneType = SCENE_MAIN;	// �V�[���̎��
	m_bEndRush = false;			// ���b�V�����I��������
	m_bControll = false;		// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_nNumStage = 0;			// �X�e�[�W�̑���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CGameManager *CGameManager::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CGameManager *pManager = NULL;

	if (pManager == NULL)
	{// NULL��������

		// �������̊m��
		pManager = DEBUG_NEW CGameManager;

		if (pManager != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pManager->Init();

			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pManager;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGameManager::Init(void)
{
	m_bControll = true;		// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������

	//CPlayerUnion::Create();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGameManager::Uninit(void)
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CGameManager::Update(void)
{
	// ������
	switch (m_SceneType)
	{
	case CGameManager::SCENE_MAIN:
		m_bControll = true;
		break;

	case CGameManager::SCENE_MAINCLEAR:
		m_bControll = true;
		break;

	case CGameManager::SCENE_RUSH:
		m_bControll = true;
		break;

	case CGameManager::SCENE_BOSS:
		m_bControll = true;
		break;

	case CGameManager::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SCENE_TRANSITION:
		m_bControll = false;
		break;

	default:
		break;
	}


	if (m_SceneType == SCENE_TRANSITION)
	{// �J�ڒ�

		// �J�ڂȂ��t�F�[�h�̏�Ԏ擾
		CInstantFade::STATE fadestate = CManager::GetInstance()->GetInstantFade()->GetState();

		if (fadestate == CInstantFade::STATE_FADECOMPLETION)
		{// ���������u��

			// �J�����擾
			CCamera *pCamera = CManager::GetInstance()->GetCamera();
			pCamera->SetEnableFollow(true);

			// �ˏo��̈ʒu���Z�b�g
			CGame::GetStage()->GetInjectionTable()->SetPosition(CGame::GetStage()->GetInjectionTable()->GetOriginPosition());

			if (m_bEndNormalStage == false)
			{// �ʏ�X�e�[�W���I����Ă��Ȃ�������
				SetEnemy();
			}
			else
			{// �{�X�X�e�[�W
				SetBoss();
			}

		}
	}

}

//==========================================================================
// �{�X�ݒ�
//==========================================================================
void CGameManager::SetBoss(void)
{
	// BGM�X�g�b�v
	CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_BGM_GAME);

	// ��ސݒ�
	m_SceneType = SCENE_BOSS;

	// ���Z�b�g����
	CGame::Reset();

	// �V�[���̃��Z�b�g
	CManager::GetInstance()->GetScene()->ResetScene();

	// �v���C���[���
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{
			return;
		}

		// �ʒu�ݒ�
		pPlayer->SetPosition(D3DXVECTOR3(-500.0f + nCntPlayer * 250.0f, 5000.0f, -1000.0f));
	}

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	pCamera->ResetBoss();

	// ���t���[���N��
	CManager::GetInstance()->GetBlackFrame()->SetState(CBlackFrame::STATE_IN);
}

//==========================================================================
// �G�ݒ�
//==========================================================================
void CGameManager::SetEnemy(void)
{
	// �v���C���[���
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{
			continue;
		}

		// �ʒu�ݒ�
		pPlayer->SetPosition(D3DXVECTOR3(-500.0f + nCntPlayer * 250.0f, 0.0f, -1000.0f));
	}

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	pCamera->Reset(CScene::MODE_GAME);

	// �G�̍Ĕz�u
	CGame::GetEnemyManager()->SetStageEnemy();

	// �ύX������Ȃ�����
	CGame::GetEnemyManager()->SetEnableChangeStage(false);

	// ��ސݒ�
	m_SceneType = SCENE_MAIN;
}

//==========================================================================
// �X�e�[�W�̉��Z
//==========================================================================
void CGameManager::AddNowStage(void)
{
	// ���Z
	m_nNowStage++;

	if (CGame::GetEnemyBase()->GetNumStage() <= m_nNowStage)
	{// ���X�e�[�W���𒴂�����
		m_nNowStage = CGame::GetEnemyBase()->GetNumStage();

		// �ʏ�X�e�[�W���I������
		m_bEndNormalStage = true;
	}

}

//==========================================================================
// ���݂̃X�e�[�W�擾
//==========================================================================
int CGameManager::GetNowStage(void)
{
	return m_nNowStage;
}

//==========================================================================
// �V�[���̎�ސݒ�
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_SceneType = type;
}

//==========================================================================
// �V�[���̎�ގ擾
//==========================================================================
CGameManager::SceneType CGameManager::GetType(void)
{
	return m_SceneType;
}
