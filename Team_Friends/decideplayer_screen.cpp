//=============================================================================
// 
//  �^�C�g����ʏ��� [decideplayer_screen.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "decideplayer_screen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "decidecharacter.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define ALPHATIME	(60)		// �s�����x�X�V�̎���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CDecidePlayerScreen::m_apTextureFile[VTX_MAX] =			// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\decideplayer_text.png",
};

const char *CDecidePlayerScreen::m_apTextureFile_Select[VTXSELECT_MAX] =	// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\decideplayer_select01.png",
	"data\\TEXTURE\\decideplayer_select02.png",
	"data\\TEXTURE\\decideplayer_select03.png",
	"data\\TEXTURE\\decideplayer_select04.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDecidePlayerScreen::CDecidePlayerScreen(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	memset(&m_pObj2D[0], NULL, sizeof(m_pObj2D));				// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	memset(&m_pSelect2D[0], NULL, sizeof(m_pSelect2D));			// �I�����̃I�u�W�F�N�g
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	memset(&m_nTexIdx_Select[0], 0, sizeof(m_nTexIdx_Select));	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nCntAlpha = 0;		// �s�����x�̃J�E���^�[
	m_nNowSelect = 0;		// ���݂̑I����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDecidePlayerScreen::~CDecidePlayerScreen()
{

}

//==========================================================================
// ��������
//==========================================================================
CDecidePlayerScreen *CDecidePlayerScreen::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CDecidePlayerScreen *pTitleScreen = NULL;

	if (pTitleScreen == NULL)
	{// NULL��������

		// �������̊m��
		pTitleScreen = DEBUG_NEW CDecidePlayerScreen;

		if (pTitleScreen != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pTitleScreen->Init();
		}

		return pTitleScreen;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDecidePlayerScreen::Init(void)
{
	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̃I�u�W�F�N�g�擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		// ��������
		m_pObj2D[nCntSelect] = CObject2D::Create(8);

		// ��ނ̐ݒ�
		m_pObj2D[nCntSelect]->SetType(TYPE_OBJECT2D);

		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntSelect] = pTexture->Regist(m_apTextureFile[nCntSelect]);

		// �e�N�X�`���̊��蓖��
		m_pObj2D[nCntSelect]->BindTexture(m_nTexIdx[nCntSelect]);

		// �e��ϐ��̏�����
		D3DXVECTOR2 size;
		switch (nCntSelect)
		{
		case VTX_TEXT:
			// �T�C�Y�擾
			size = pTexture->GetImageSize(m_nTexIdx[nCntSelect]);
			m_pObj2D[nCntSelect]->SetSize(size * 0.4f);	// �T�C�Y
			m_pObj2D[nCntSelect]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));	// �ʒu
			m_pObj2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);	// �F
			break;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		// ��������
		m_pSelect2D[nCntSelect] = CObject2D::Create(8);

		// ��ނ̐ݒ�
		m_pSelect2D[nCntSelect]->SetType(TYPE_OBJECT2D);

		// �e�N�X�`���̊��蓖��
		m_nTexIdx_Select[nCntSelect] = pTexture->Regist(m_apTextureFile_Select[nCntSelect]);

		// �e�N�X�`���̊��蓖��
		m_pSelect2D[nCntSelect]->BindTexture(m_nTexIdx_Select[nCntSelect]);

		// �T�C�Y�擾
		D3DXVECTOR2 size = pTexture->GetImageSize(m_nTexIdx_Select[nCntSelect]) * 0.4f;
		float fDistance = size.x * 2.0f;

		m_pSelect2D[nCntSelect]->SetSize(size);								// �T�C�Y
		m_pSelect2D[nCntSelect]->SetPosition(D3DXVECTOR3(640.0f - (fDistance * 2.0f) + nCntSelect * fDistance + size.x, 550.0f, 0.0f));	// �ʒu
		m_pObj2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);	// �F
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDecidePlayerScreen::Uninit(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_pObj2D[nCntSelect] = NULL;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_pSelect2D[nCntSelect] = NULL;
		}
	}

	// ���폜
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CDecidePlayerScreen::Delete(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_pObj2D[nCntSelect]->Uninit();
			m_pObj2D[nCntSelect] = NULL;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_pSelect2D[nCntSelect]->Uninit();
			m_pSelect2D[nCntSelect] = NULL;
		}
	}

	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CDecidePlayerScreen::Update(void)
{

	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] == NULL)
		{// NULL��������
			continue;
		}

		// ���_���ݒ�
		m_pObj2D[nCntSelect]->SetVtx();
	}




	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// ���݂̑I�����X�V
	if ((pInputGamepad->GetStickSelect(CInputGamepad::STICK_X) == false && pInputGamepad->GetStickMoveL(0).x < 0) ||
		(pInputKeyboard->GetTrigger(DIK_A) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LEFT, 0)))
	{// ��

		// ���X�e�B�b�N�̔����n��
		pInputGamepad->SetEnableStickSelect(true, CInputGamepad::STICK_X);

		// �p�^�[��No.���X�V
		m_nNowSelect = (m_nNowSelect + (VTXSELECT_MAX - 1)) % VTXSELECT_MAX;

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);

		// �J�E���^�[���Z�b�g
		m_nCntAlpha = 0;
	}
	else if (pInputGamepad->GetStickSelect(CInputGamepad::STICK_X) == false && pInputGamepad->GetStickMoveL(0).x > 0 ||
		(pInputKeyboard->GetTrigger(DIK_D) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_RIGHT, 0)))
	{// �E

		// ���X�e�B�b�N�̔����n��
		pInputGamepad->SetEnableStickSelect(true, CInputGamepad::STICK_X);

		// �p�^�[��No.���X�V
		m_nNowSelect = (m_nNowSelect + 1) % VTXSELECT_MAX;

		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);

		// �J�E���^�[���Z�b�g
		m_nCntAlpha = 0;
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{// ���肪�����ꂽ
		CManager::GetInstance()->SetNumPlayer(m_nNowSelect + 1);
		//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);

		// �L�������߉�ʐ���
		CDecideCharacter::Create();

		Delete();
		return;
	}


	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] == NULL)
		{// NULL��������
			continue;
		}

		// �I�����̍X�V����
		UpdateSelect(nCntSelect);

		// ���_���ݒ�
		m_pSelect2D[nCntSelect]->SetVtx();
	}
}

//==========================================================================
// �I�����̍X�V����
//==========================================================================
void CDecidePlayerScreen::UpdateSelect(int nCntSelect)
{
	// �F�擾
	D3DXCOLOR col = m_pSelect2D[nCntSelect]->GetColor();

	// �s�����x�X�V
	if (m_nNowSelect == nCntSelect)
	{
		CuadricCurveComp(col.a, ALPHATIME, 0.3f, 1.0f, m_nCntAlpha);
	}
	else
	{
		col.a = 1.0f;
	}

	// �F�ݒ�
	m_pSelect2D[nCntSelect]->SetColor(col);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CDecidePlayerScreen::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	//{
	//	// �`�揈��
	//	m_pObj2D[nCntSelect]->Draw();
	//}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

