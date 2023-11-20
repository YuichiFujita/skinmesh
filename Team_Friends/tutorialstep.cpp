//=============================================================================
// 
//  �`���[�g���A���X�e�b�v���� [tutorialstep.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "tutorialstep.h"
#include "tutorialwindow.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define SPEEDTIME	(150)	// �X�s�[�h�ύX�̎���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTutorialStep::CTutorialStep()
{
	// �l�̃N���A
	m_nCntWait = 0;			// �ҋ@�̃J�E���^�[
	m_nCntSpeedUP = 0;		// �X�s�[�h�A�b�v�̃J�E���^�[
	m_nCntSpeedDOWN = 0;	// �X�s�[�h�_�E���̃J�E���^�[
	m_nCntImpactWave = 0;	// �~�`�g�̉�
	m_nCntDirectWave = 0;	// �����g�̉�
	m_bEndStep = false;		// �X�e�b�v�̏I������
	m_bSetOK = false;		// OK�̐ݒ蔻��
	m_step = STEP_WAIT;		// ���݂̃X�e�b�v
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTutorialStep::~CTutorialStep()
{

}

//==========================================================================
// ��������
//==========================================================================
CTutorialStep *CTutorialStep::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CTutorialStep *pScene = NULL;

	if (pScene == NULL)
	{// NULL��������

		// ��������
		pScene = DEBUG_NEW CTutorialStep;

		if (pScene != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			if (FAILED(pScene->Init()))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pScene;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTutorialStep::Init(void)
{
	// �e��ϐ��̏�����
	m_nCntWait = 0;			// �ҋ@�̃J�E���^�[
	m_nCntSpeedUP = 0;		// �X�s�[�h�A�b�v�̃J�E���^�[
	m_nCntSpeedDOWN = 0;	// �X�s�[�h�_�E���̃J�E���^�[
	m_nCntImpactWave = 0;	// �~�`�g�̉�
	m_nCntDirectWave = 0;	// �����g�̉�
	m_bEndStep = false;		// �X�e�b�v�̏I������
	m_bSetOK = false;		// OK�̐ݒ蔻��

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTutorialStep::Uninit(void)
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CTutorialStep::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	switch (m_step)
	{
	case CTutorialStep::STEP_WAIT:

		// �ҋ@���ԉ��Z
		m_nCntWait++;

		if (m_nCntWait >= 120)
		{// �ҋ@���I�����Ă�����

			// �X�e�b�v�̐ݒ菈��
			SetStep(STEP_WAIT);
		}
		break;

	case CTutorialStep::STEP_MAX:
		if (m_step >= CTutorialStep::STEP_MAX - 1 &&
			(pInputKeyboard->GetTrigger(DIK_BACKSPACE) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_START, 0) == true))
		{
			// ���[�h�ݒ�
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
		}
		break;
	}

	if (m_step > CTutorialStep::STEP_WAIT &&
		m_bEndStep == true && m_bSetOK == false)
	{// �X�e�b�v���I�����Ă����� && OK��ݒ肵�Ă��Ȃ�

		// OK�̐�������
		m_bSetOK = true;

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_STEPCLEAR);
	}

}

//==========================================================================
// �X�e�b�v�̐ݒ菈��
//==========================================================================
void CTutorialStep::SetStep(STEP step)
{
	if (m_step != step)
	{
		return;
	}

	switch (step)
	{
	case CTutorialStep::STEP_WAIT:
		if (m_bEndStep == false)
		{
			CTutorialWindow::Create(m_step);
			m_bEndStep = true;
		}
		break;

	case CTutorialStep::STEP_SPEEDUP:
		// �X�s�[�h�A�b�v�̃J�E���^�[
		m_nCntSpeedUP++;

		if (m_bEndStep == false && m_nCntSpeedUP >= SPEEDTIME)
		{
			m_bEndStep = true;
		}
		break;

	case CTutorialStep::STEP_SPEEDDOWN:
		// �X�s�[�h�_�E���̃J�E���^�[
		m_nCntSpeedDOWN++;

		if (m_bEndStep == false && m_nCntSpeedDOWN >= SPEEDTIME)
		{
			m_bEndStep = true;
		}
		break;

	case CTutorialStep::STEP_IMPACTWAVE:
		// �~�`�g�̉�
		m_nCntImpactWave++;

		if (m_bEndStep == false && m_nCntImpactWave >= 2)
		{
			m_bEndStep = true;
		}
		break;

	case CTutorialStep::STEP_DIRECTWAVE:
		// �����g�̉�
		m_nCntDirectWave++;

		if (m_bEndStep == false && m_nCntDirectWave >= 2)
		{
			m_bEndStep = true;
		}
		break;

	case CTutorialStep::STEP_MAX:
		break;

	default:
		break;
	}
}

//==========================================================================
// �X�e�b�v��i�߂�
//==========================================================================
void CTutorialStep::AddStep(void)
{
	if (m_step < CTutorialStep::STEP_MAX && m_bEndStep == true)
	{// �i�߂����Ԃ�������
		m_step = (STEP)(m_step + 1);
		m_bEndStep = false;
	}
}

//==========================================================================
// OK�T�C���̃t���OOFF�ɂ���
//==========================================================================
void CTutorialStep::SetDisableOKSign(void)
{
	m_bSetOK = false;
}

//==========================================================================
// ���݂̃X�e�b�v�擾
//==========================================================================
CTutorialStep::STEP CTutorialStep::GetNowStep(void)
{
	return m_step;
}

//==========================================================================
// �X�e�b�v�I���̔���擾
//==========================================================================
bool CTutorialStep::IsEndStep(void)
{
	return m_bEndStep;
}
