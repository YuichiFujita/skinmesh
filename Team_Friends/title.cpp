//=============================================================================
// 
//  �^�C�g������ [title.cpp]
//  Author : ���n�Ή�
//  Added by �������
// 
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "sound.h"
#include "title_logo.h"
#include "fog.h"
#include "player_title.h"
#include "enemy.h"

//==========================================
//  �萔��` ����
//==========================================
namespace
{
	const D3DXCOLOR TARGET_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const float START_LENGTH = 300.0f; // ��������
	const float END_LENGTH = 3000.0f; // �ڕW����
	const float FUNCTION = 0.01f; //�{��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle::CTitle() :
m_col(D3DXCOLOR(0.0f, 0.0, 0.0f, 1.0f)),
m_fLength(START_LENGTH)
{
	// �l�̃N���A
	m_nCntSwitch = 0;		// �؂�ւ��̃J�E���^�[
	m_pLogo = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle::~CTitle()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle::Init(void)
{
	// BGM�Đ�
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_TITLE);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	//�^�C�g�����S�̕\��
	if (m_pLogo == nullptr)
	{
		m_pLogo = CTitleLogo::Create();
	}

	// ����������
	Fog::Set(true);

	// �t�H�O�̒l��ݒ肷��
	Fog::SetStart(START_LENGTH);
	Fog::SetEnd(m_fLength);
	Fog::SetCol(m_col);

	// �v���C���[��u���Ă݂�
	CPlayerTitle::Create(D3DXVECTOR3(0.0f, 100.0f, -3000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_UNION);
	CPlayerTitle::Create(D3DXVECTOR3(160.0f, 0.0f, -2900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_ARM);
	CPlayerTitle::Create(D3DXVECTOR3(-160.0f, 0.0f, -2900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_ARM);
	CPlayerTitle::Create(D3DXVECTOR3(240.0f, 0.0f, -3200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_LEG);
	CPlayerTitle::Create(D3DXVECTOR3(-240.0f, 0.0f, -3200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_BODY);

	// �G�u���Ă݂�
	CEnemy::Create(0, "data\\TEXT\\motion_set_player.txt", D3DXVECTOR3(0.0f, 0.0f, -3000.0f), CEnemy::TYPE_TEST);

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle::Uninit(void)
{
	//���𕥂�
	Fog::Set(false);

	// �^�C�g�����S��j��
	if (m_pLogo != nullptr)
	{
		m_pLogo = nullptr;
	}

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle::Update(void)
{
	CManager::GetInstance()->GetDebugProc()->Print(
		"���݂̃��[�h�F�y�^�C�g���z\n"
		"�؂�ւ��F�y F �z\n\n");

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// �؂�ւ��̃J�E���^�[���Z
	m_nCntSwitch++;

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	if (m_nCntSwitch <= 120)
	{
		return;
	}

	//�^�C�g�����S���������Ă��Ȃ��Ƃ��͔�����
	if (!m_pLogo->GetComplete())
	{
		return;
	}

	//�t�H�O������
	WhiteOut();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0) == true)
	{
		// ���[�h�ݒ�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_DECIDEPLAYER);
	}

	if (m_nCntSwitch >= 60 * 40)
	{// �����J��
		// ���[�h�ݒ�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RANKING);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitle::Draw(void)
{

}

//==========================================
//  �t�H�O����������
//==========================================
void CTitle::WhiteOut()
{
	// �ڕW�����܂ň����L�΂�
	m_fLength += (END_LENGTH - m_fLength) * FUNCTION;

	// �ڕW�F�܂ŕ␳����
	m_col += (TARGET_COLOR - m_col) * FUNCTION;

	// ������K�p����
	Fog::SetEnd(m_fLength);

	// �F��K�p����
	Fog::SetCol(m_col);

#ifdef _DEBUG
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	if (pInputKeyboard->GetTrigger(DIK_1))
	{
		// fog����
		Fog::Set(false);
	}
	if (pInputKeyboard->GetTrigger(DIK_2))
	{
		// fog����
		Fog::Set(true);
	}

	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		// ���[�h�ݒ�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_DECIDEPLAYER);
	}

	if (pInputKeyboard->GetTrigger(DIK_4))
	{// �����J��
		// ���[�h�ݒ�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RANKING);
	}
#endif
}
