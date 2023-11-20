//=============================================================================
// 
//  �����L���O���� [ranking.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "rankingscore.h"
#include "enemymanager.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CRanking::m_nRandStage = 0;	// �X�e�[�W�̃����_���C���f�b�N�X�ԍ�
CRankingScore *CRanking::m_pRankingScore = NULL;	// �����L���O�X�R�A�̃I�u�W�F�N�g
bool CRanking::m_bAllArrival = false;		// �S�ē�����������

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRanking::CRanking()
{
	m_pRankingScore = NULL;	// �����L���O�X�R�A�̃I�u�W�F�N�g
	m_nCntSwitch = 0;		// �؂�ւ��̃J�E���^�[
	m_bAllArrival = false;	// �S�ē�����������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRanking::~CRanking()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRanking::Init(void)
{
	// �X�e�[�W�̃����_���C���f�b�N�X�ԍ�
	m_nRandStage = Random(1, 3);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// �����L���O�̃X�R�A����
	m_pRankingScore = CRankingScore::Create();

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRanking::Uninit(void)
{
	// �����L���O�X�R�A�̃I�u�W�F�N�g
	m_pRankingScore = NULL;

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRanking::Update(void)
{
	CManager::GetInstance()->GetDebugProc()->Print(
		"���݂̃��[�h�F�y�����L���O�z\n"
		"�؂�ւ��F�y F �z\n\n");

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// �؂�ւ��̃J�E���^�[���Z
	m_nCntSwitch++;

	if (m_nCntSwitch >= 60 * 60)
	{
		// ���[�h�ݒ�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0) == true)
	{
		if (m_bAllArrival == true)
		{
			// ���[�h�ݒ�
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}

		else
		{
			// �S�Ă̓�������
			//m_pRankingScore->SetAllArrival();
			m_bAllArrival = true;
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRanking::Draw(void)
{

}

//==========================================================================
// �����ݒ�ON
//==========================================================================
void CRanking::SetEnableArrival(void)
{
	m_bAllArrival = true;
}