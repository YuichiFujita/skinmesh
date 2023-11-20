//=============================================================================
// 
//  �V�[������ [scene.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "scene.h"
#include "renderer.h"
#include "calculation.h"
#include "Xload.h"
#include "map.h"
#include "fade.h"
#include "elevation.h"
#include "player.h"
#include "player_union.h"
#include "camera.h"

// �J�ڐ�
#include "game.h"
#include "decideplayer.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "ranking.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CXLoad *CScene::m_pXLoad = NULL;				// X�t�@�C���̃I�u�W�F�N�g
CElevation *CScene::m_pObject3DMesh = NULL;		// �I�u�W�F�N�g3D���b�V���̃I�u�W�F�N�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CScene::CScene()
{
	// �ϐ��̃N���A
	m_mode = MODE_TITLE;
	memset(&m_pPlayer[0], 0, sizeof(m_pPlayer));
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CScene::~CScene()
{

}

//==========================================================================
// ��������
//==========================================================================
CScene *CScene::Create(CScene::MODE mode)
{
	// �����p�̃I�u�W�F�N�g
	CScene *pScene = NULL;

	if (pScene == NULL)
	{// NULL��������

		// ��������
		switch (mode)
		{
		case CScene::MODE_TITLE:
			pScene = DEBUG_NEW CTitle;
			break;

		case CScene::MODE_DECIDEPLAYER:
			pScene = DEBUG_NEW CDecidePlayer;
			break;

		case CScene::MODE_TUTORIAL:
			pScene = DEBUG_NEW CTutorial;
			break;

		case CScene::MODE_GAME:
			pScene = DEBUG_NEW CGame;
			break;

		case CScene::MODE_RESULT:
			pScene = DEBUG_NEW CResult;
			break;

		case CScene::MODE_RANKING:
			pScene = DEBUG_NEW CRanking;
			break;
		}

		if (pScene != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ���[�h�̐ݒ�
			pScene->m_mode = mode;
		}

		return pScene;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CScene::Init(void)
{
	HRESULT hr;

	//**********************************
	// X�t�@�C��
	//**********************************
	if (m_pXLoad != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pXLoad = DEBUG_NEW CXLoad;

	if (m_pXLoad == NULL)
	{// �������̊m�ۂ��o���Ă��Ȃ�������
		return E_FAIL;
	}
	m_pXLoad->Init();

	//**********************************
	// �}�b�v�̐���
	//**********************************
	if (FAILED(map::Create("data\\TEXT\\map\\info.txt")))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	//**********************************
	// �N���̒n��
	//**********************************
	m_pObject3DMesh = CElevation::Create("data\\TEXT\\elevation\\field.txt");

	// ����������
	hr = m_pXLoad->Init();

	if (FAILED(hr))
	{// ���������������s�����ꍇ
		return E_FAIL;
	}

	// �L��������
	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetNumPlayer(); nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer);
		if (m_pPlayer[nCntPlayer] == NULL)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CScene::Uninit(void)
{
	// X�t�@�C���̔j��
	if (m_pXLoad != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pXLoad->Uninit();

		// �������̊J��
		delete m_pXLoad;
		m_pXLoad = NULL;
	}

	// �v���C���[�̔j��
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{// �������̊m�ۂ��o���Ă�����

			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}

}

//==========================================================================
// �X�V����
//==========================================================================
void CScene::Update(void)
{
	
}

//==========================================================================
// �`�揈��
//==========================================================================
void CScene::Draw(void)
{
	
}

//==========================================================================
// �V�[���̃��Z�b�g
//==========================================================================
void CScene::ResetScene(void)
{
	//**********************************
	// �j���t�F�[�Y
	//**********************************
	// �v���C���[�̔j��
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{// �������̊m�ۂ��o���Ă�����

			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}

	// �}�b�v
	map::Release();

	//**********************************
	// �����t�F�[�Y
	//**********************************
	// �}�b�v
	if (FAILED(map::Create("data\\TEXT\\map\\info_boss.txt")))
	{// ���s�����ꍇ
		return;
	}


	// ���̌�v���C���[����
	CPlayerUnion::Create(CPlayerUnion::TYPE_ALL);
}

//==========================================================================
// ���݂̃��[�h�擾
//==========================================================================
CScene::MODE CScene::GetMode(void)
{
	return m_mode;
}

//==========================================================================
// X�t�@�C���̎擾
//==========================================================================
CXLoad *CScene::GetXLoad(void)
{
	return m_pXLoad;
}

//==========================================================================
// 3D�I�u�W�F�N�g�̎擾
//==========================================================================
CElevation *CScene::GetElevation(void)
{
	return m_pObject3DMesh;
}

//==========================================================================
// �v���C���[�̎擾
//==========================================================================
CPlayer **CScene::GetPlayer(void)
{
	return &m_pPlayer[0];
}

//==========================================================================
// �v���C���[�̎擾
//==========================================================================
CPlayer *CScene::GetPlayer(int nIdx)
{
	return m_pPlayer[nIdx];
}

//==========================================================================
// �v���C���[�̏I��
//==========================================================================
void CScene::UninitPlayer(int nIdx)
{
	m_pPlayer[nIdx] = NULL;
}
