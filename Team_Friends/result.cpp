//=============================================================================
// 
//  ���U���g���� [result.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "resultscore.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CResultScore *CResult::m_pResultScore = NULL;	// ���U���g�X�N���[���̃I�u�W�F�N�g
bool CResult::m_bAllArrival = false;		// �S�ē�����������

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResult::CResult()
{
	// �l�̃N���A
	m_bAllArrival = false;	// �S�ē�����������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResult::~CResult()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResult::Init(void)
{

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// BGM�Đ�
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_RESULT);

	// ���U���g���
	m_pResultScore = CResultScore::Create();

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResult::Uninit(void)
{
	m_pResultScore = NULL;

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CResult::Update(void)
{
	CManager::GetInstance()->GetDebugProc()->Print(
		"���݂̃��[�h�F�y���U���g�z\n"
		"�؂�ւ��F�y F �z\n\n");

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	static int n = 0;
	n = (n + 1) % 5;
	if (n == 0)
	{
		// ���[�h�ݒ�
		//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0) == true)
	{
		if (m_bAllArrival == true)
		{
			// ���[�h�ݒ�
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RANKING);
		}

		if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE)
		{
			// �S�Ă̓�������
			if (m_pResultScore != NULL)
			{
				m_pResultScore->SetAllArrival();
			}
			m_bAllArrival = true;
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CResult::Draw(void)
{

}

//==========================================================================
// ���U���g�X�N���[���̏��擾
//==========================================================================
CResultScore *CResult::GetResultScore(void)
{
	return m_pResultScore;
}

//==========================================================================
// �����ݒ�ON
//==========================================================================
void CResult::SetEnableArrival(void)
{
	m_bAllArrival = true;
}
