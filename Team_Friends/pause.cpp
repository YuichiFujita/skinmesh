//=============================================================================
// 
//  �|�[�Y���� [pause.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "input.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define DIS_POSY	(130.0f)	// �I�����̊Ԋu

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CPause::m_apTextureFile[CPause::VTX_MAX] =		// �e�N�X�`���̃t�@�C��
{
	NULL,
	"data\\TEXTURE\\pause_menu.png",
	"data\\TEXTURE\\pause_game.png",
	"data\\TEXTURE\\pause_retry.png",
	"data\\TEXTURE\\pause_title.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPause::CPause()
{
	// �l�̃N���A
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		m_aObject2D[nCntVtx] = NULL;	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
		m_nTexIdx[nCntVtx] = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	}
	m_bPause = false;	// �|�[�Y�̔���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �|�[�Y�̃J���[
	m_nSelect = 0;									// �I����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPause::~CPause()
{

}

//==========================================================================
// ��������
//==========================================================================
CPause *CPause::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CPause *pFade = NULL;

	if (pFade == NULL)
	{// NULL��������

		// �������̊m��
		pFade = DEBUG_NEW CPause;

		if (pFade != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			if (FAILED(pFade->Init()))
			{// ���s���Ă�����
				return NULL;
			}
		}
		else
		{
			delete pFade;
			pFade = NULL;
		}

		return pFade;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPause::Init(void)
{
	// ��������
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		// ��������
		m_aObject2D[nCntVtx] = CObject2D::Create(7);

		if (m_aObject2D[nCntVtx] == NULL)
		{// ���s���Ă�����
			return E_FAIL;
		}

		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntVtx] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntVtx]);

		// �e�N�X�`���̊��蓖��
		m_aObject2D[nCntVtx]->BindTexture(m_nTexIdx[nCntVtx]);

		if (nCntVtx == VTX_FADE)
		{// �����̎�
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));				// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(D3DXVECTOR3(640.0f, 360.0f, 0.0f));	// �ʒu
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f));		// �F�ݒ�
		}
		else if (nCntVtx == VTX_WINDOW)
		{// �E�B���h�E�̎�
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));				// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(D3DXVECTOR3(640.0f, 360.0f, 0.0f));	// �ʒu
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �F�ݒ�
		}
		else
		{// �I����
			m_aObject2D[nCntVtx]->SetSize(CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.45f);	// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(D3DXVECTOR3(640.0f, 430.0f + ((nCntVtx - VTX_RETRY) * DIS_POSY), 0.0f));	// �ʒu
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�ݒ�
		}
	}

	// ��ނ̐ݒ�
	//m_aObject2D->SetType(CObject::TYPE_FADE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPause::Uninit(void)
{
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_aObject2D[nCntVtx]->Uninit();
			delete m_aObject2D[nCntVtx];
			m_aObject2D[nCntVtx] = NULL;
		}
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CPause::Update(void)
{
	if (m_bPause == false)
	{// �|�[�Y������Ȃ�������
		return;
	}

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h������Ȃ���
		return;
	}

	// �I�����̍X�V����
	UpdateSelect(m_nSelect);

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	if (pInputKeyboard->GetTrigger(DIK_W) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_UP, 0))
	{// ��n�������ꂽ

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + (MENU_MAX - 1)) % MENU_MAX;

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_DOWN, 0))
	{// ���n�������ꂽ

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + 1) % MENU_MAX;

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);
	}


	if (pInputGamepad->GetStickSelect(CInputGamepad::STICK_Y) == false && pInputGamepad->GetStickMoveL(0).y > 0)
	{// ��ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

		// ���X�e�B�b�N�̔����n��
		pInputGamepad->SetEnableStickSelect(true, CInputGamepad::STICK_Y);

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + (MENU_MAX - 1)) % MENU_MAX;

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);
	}
	else if (pInputGamepad->GetStickSelect(CInputGamepad::STICK_Y) == false && pInputGamepad->GetStickMoveL(0).y < 0)
	{// ���ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

		// ���X�e�B�b�N�̔����n��
		pInputGamepad->SetEnableStickSelect(true, CInputGamepad::STICK_Y);

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + 1) % MENU_MAX;

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);
	}


	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{// ���肪�����ꂽ

		switch (m_nSelect)
		{
		case MENU_RETURNGAME:
			SetPause();
			break;

		case MENU_RETRY:

			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DONE);

			// �Q�[�������g���C����
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
			break;

		case MENU_RETURNTITLE:

			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DONE);

			// �^�C�g���ɖ߂�
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
			break;
		}
	}



	// �X�V����
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != NULL)
		{// NULL����Ȃ�������

			// �X�V����
			m_aObject2D[nCntVtx]->Update();

			// ���_���W�X�V
			m_aObject2D[nCntVtx]->SetVtx();
		}
	}
}


//==================================================================================
//�|�[�Y���̑I�����X�V
//==================================================================================
void CPause::UpdateSelect(int nSelect)
{
	static float fMoveAlpha = 0.008f;

	if (m_col.a > 1.0f)
	{//�s�����x100%
		m_col.a = 1.0f;
		fMoveAlpha *= -1;
	}
	else if (m_col.a < 0.4f)
	{//�s�����x20%
		m_col.a = 0.4f;
		fMoveAlpha *= -1;
	}

	//�s�����x�̍X�V
	m_col.a += fMoveAlpha;

	//�I��������n�߂�
	for (int nCount = VTX_CONTINUE; nCount < VTX_MAX; nCount++)
	{
		// �F�擾
		D3DXCOLOR col = m_aObject2D[nCount]->GetColor();

		if (nCount != nSelect + VTX_CONTINUE)
		{// ���̑I��������Ȃ��Ƃ�

			// ���_�J���[�̐ݒ�
			col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			// ���_�J���[�̐ݒ�
			col = m_col;
		}

		// �F�ݒ�
		m_aObject2D[nCount]->SetColor(col);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPause::Draw(void)
{
	if (m_bPause == false)
	{// �|�[�Y������Ȃ�������
		return;
	}

	// �`�揈��
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != NULL)
		{// NULL����Ȃ�������

			// �`�揈��
			m_aObject2D[nCntVtx]->Draw();
		}
	}
}

//==========================================================================
// �|�[�Y�̐ݒ�
//==========================================================================
void CPause::SetPause(void)
{
	// �g�p�󋵕ύX
	m_bPause = m_bPause ? false : true;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �|�[�Y�̃J���[
	m_nSelect = 0;									// �I����
}

//==========================================================================
// �g�p�󋵎擾
//==========================================================================
bool CPause::IsPause(void)
{
	return m_bPause;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D **CPause::GetMyObject(void)
{
	return &m_aObject2D[0];
}