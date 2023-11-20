//=============================================================================
// 
//  �v���C���[���ߏ��� [decideplayer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "decideplayer.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "decideplayer_screen.h"
#include "sound.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDecidePlayer::CDecidePlayer()
{
	// �l�̃N���A

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDecidePlayer::~CDecidePlayer()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDecidePlayer::Init(void)
{

	// BGM�Đ�
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_TITLE);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// �^�C�g�����
	CDecidePlayerScreen::Create();

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDecidePlayer::Uninit(void)
{
	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CDecidePlayer::Update(void)
{
	CManager::GetInstance()->GetDebugProc()->Print(
		"���݂̃��[�h�F�y�l�����߁z\n");

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CDecidePlayer::Draw(void)
{

}
