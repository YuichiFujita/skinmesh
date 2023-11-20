//=============================================================================
// 
//  �`���[�g���A���E�B���h�E���� [tutorialwindow.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "tutorialwindow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "input.h"
#include "fade.h"
#include "tutorial.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WINDOW_POSITION	(D3DXVECTOR3(1920.0f, 360.0f, 0.0f))
#define WAITTIME	(15)	// �ҋ@����

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CTutorialWindow::m_apTextureFile[CTutorialStep::STEP_MAX] = // �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\tutorialwindow\\window_01.png",
	"data\\TEXTURE\\tutorialwindow\\window_02.png",
	"data\\TEXTURE\\tutorialwindow\\window_03.png",
	"data\\TEXTURE\\tutorialwindow\\window_04.png",
	"data\\TEXTURE\\tutorialwindow\\window_05.png",
};
int CTutorialWindow::m_nTexIdx[CTutorialStep::STEP_MAX] = {};	// �e�N�X�`���̃C���f�b�N�X�ԍ�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTutorialWindow::CTutorialWindow(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_NowStep = CTutorialStep::STEP_WAIT;	// ����̃X�e�b�v
	m_nCntWait = 0;		// �ҋ@����

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTutorialWindow::~CTutorialWindow()
{

}

//==========================================================================
// ��������
//==========================================================================
CTutorialWindow *CTutorialWindow::Create(CTutorialStep::STEP step)
{
	// �����p�̃I�u�W�F�N�g
	CTutorialWindow *pWindow = NULL;

	if (pWindow == NULL)
	{// NULL��������

		// �������̊m��
		pWindow = DEBUG_NEW CTutorialWindow;

		if (pWindow != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �����̏���n��
			pWindow->m_NowStep = step;

			// ����������
			pWindow->Init();
		}

		return pWindow;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTutorialWindow::Init(void)
{

	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init();

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e��ϐ��̏�����
	m_nCntWait = WAITTIME;		// �ҋ@����


	// �e�N�X�`���̊��蓖��
	if (m_nTexIdx[m_NowStep] == 0)
	{// �܂��ǂݍ���łȂ�������
		m_nTexIdx[m_NowStep] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_NowStep]);
	}

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx[m_NowStep]);

	// �T�C�Y�擾
	SetSize(D3DXVECTOR2(640.0f, 360.0f));	// �T�C�Y
	SetPosition(WINDOW_POSITION);			// �ʒu


	// �T�E���h�Đ�
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_TUTORIALWINDOW);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTutorialWindow::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTutorialWindow::Update(void)
{
	// ���_���X�V
	SetVtx();

	// �ҋ@���Ԍ��Z
	m_nCntWait--;

	if (m_nCntWait >= 0)
	{// �ҋ@���Ԃ��I�������

		D3DXVECTOR3 pos = GetPosition();			// �ʒu

		pos.x += (640.0f - WINDOW_POSITION.x) / (float)WAITTIME;

		SetPosition(pos);			// �ʒu
		return;
	}

	// ���Z�b�g
	m_nCntWait = -1;

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0) == true)
	{// ���肪������Ă�����

		// �I������
		Uninit();
		return;
	}

}
