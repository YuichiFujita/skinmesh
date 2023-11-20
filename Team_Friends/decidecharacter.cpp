//=============================================================================
// 
//  �L�����N�^�[���߉�ʏ��� [decidecharacter.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "decidecharacter.h"
#include "decideplayer_screen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "cursor.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define ALPHATIME	(60)		// �s�����x�X�V�̎���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CDecideCharacter::m_apTextureFile[VTX_MAX] =			// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\decideplayer_text.png",
};

const char *CDecideCharacter::m_apTextureFile_Select[VTXCHARACTER_MAX] =	// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\decidecharacter_01.png",
	"data\\TEXTURE\\decidecharacter_02.png",
	"data\\TEXTURE\\decidecharacter_03.png",
	"data\\TEXTURE\\decidecharacter_04.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDecideCharacter::CDecideCharacter(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	memset(&m_pObj2D[0], NULL, sizeof(m_pObj2D));				// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	memset(&m_pSelect2D[0], NULL, sizeof(m_pSelect2D));			// �I�����̃I�u�W�F�N�g
	memset(&m_apCursor[0], NULL, sizeof(m_apCursor));			// �J�[�\���̃I�u�W�F�N�g
	memset(&m_bDecide[0], false, sizeof(m_bDecide));			// ���肵����
	m_bAllDecide = false;										// �S�Č��肵����
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	memset(&m_nTexIdx_Select[0], 0, sizeof(m_nTexIdx_Select));	// �e�N�X�`���̃C���f�b�N�X�ԍ�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDecideCharacter::~CDecideCharacter()
{

}

//==========================================================================
// ��������
//==========================================================================
CDecideCharacter *CDecideCharacter::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CDecideCharacter *pTitleScreen = NULL;

	if (pTitleScreen == NULL)
	{// NULL��������

		// �������̊m��
		pTitleScreen = DEBUG_NEW CDecideCharacter;

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
HRESULT CDecideCharacter::Init(void)
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

	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
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
		D3DXVECTOR2 size = pTexture->GetImageSize(m_nTexIdx_Select[nCntSelect]) * 0.2f;

		m_pSelect2D[nCntSelect]->SetSize(size);			// �T�C�Y
		m_pSelect2D[nCntSelect]->SetSizeOrigin(size);	// ���̃T�C�Y

		size *= 1.5f;
		float fDistance = size.x * 2.0f;
		m_pSelect2D[nCntSelect]->SetPosition(D3DXVECTOR3(640.0f - (fDistance * 2.0f) + nCntSelect * fDistance + size.x, 600.0f, 0.0f));	// �ʒu
		m_pObj2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);	// �F
	}

	for (int i = 0; i < CManager::GetInstance()->GetNumPlayer(); i++)
	{
		float fDistance = 100.0f;
		D3DXVECTOR3 pos = D3DXVECTOR3(640.0f - (fDistance * 2.0f) + i * fDistance + (fDistance * 0.5f), 200.0f, 0.0f);

		// �J�[�\������
		m_apCursor[i] = CCursor::Create(pos, i);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDecideCharacter::Uninit(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_pObj2D[nCntSelect] = NULL;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
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
void CDecideCharacter::Delete(void)
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

	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
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
void CDecideCharacter::Update(void)
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

	if (m_bDecide[0] == false &&
		(pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_B, 0)))
	{// �L�����Z��

		for (int i = 0; i < CManager::GetInstance()->GetNumPlayer(); i++)
		{
			if (m_apCursor[i] == NULL)
			{
				continue;
			}
			m_apCursor[i]->Uninit();
			m_apCursor[i] = NULL;
		}

		// �폜
		Delete();

		// �v���C���[�l���I����ʐ���
		CDecidePlayerScreen::Create();
		return;
	}


	if (m_bAllDecide == true &&
		(pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_START, 0)))
	{// ���肪�����ꂽ

		// �Q�[���ɑJ�ڂ���
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
	}


	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] == NULL)
		{// NULL��������
			continue;
		}

		// �I�����Ƃ̓����蔻��
		if (CollisionSelect(nCntSelect))
		{
			//break;
		}

		// ���_���ݒ�
		m_pSelect2D[nCntSelect]->SetVtx();
	}
}

//==========================================================================
// �I�����̍X�V����
//==========================================================================
bool CDecideCharacter::CollisionSelect(int nCntSelect)
{
	// ���擾
	D3DXVECTOR3 pos = m_pSelect2D[nCntSelect]->GetPosition();
	D3DXVECTOR2 size = m_pSelect2D[nCntSelect]->GetSize();
	D3DXVECTOR2 sizeOrigin = m_pSelect2D[nCntSelect]->GetSizeOrigin();

	// �l�p�Ɖ~�̔���
	bool bHit = false;
	int nCursorIdx = -1;

	// �S�Č�����
	m_bAllDecide = true;

	for (int i = 0; i < CManager::GetInstance()->GetNumPlayer(); i++)
	{
		if (m_apCursor[i] == NULL)
		{
			continue;
		}

		// �J�[�\�����Ƃ̑I�����̃C���f�b�N�X�ԍ��擾
		int nDecideIdx = m_apCursor[i]->GetSelectIdx();

		if (nDecideIdx >= 0)
		{
			// �����Ԏ擾
			m_bDecide[nDecideIdx] = m_apCursor[i]->IsDecide();

			if (m_bDecide[nDecideIdx] == true)
			{
				continue;
			}
			else
			{
				// �S�Č��肵�ĂȂ��悤�ɂ���
				m_bAllDecide = false;
			}
		}
		else
		{
			// �S�Č��肵�ĂȂ��悤�ɂ���
			m_bAllDecide = false;
		}

		// ���擾
		D3DXVECTOR3 posCursor = m_apCursor[i]->GetPosition();
		D3DXVECTOR2 sizeCursor = m_apCursor[i]->GetSize();

		// �~�Ƌ�`�̔���
		bHit = CollisionCircleSquare2D(posCursor, pos, mylib_const::DEFAULT_VECTOR3, sizeCursor.x, size);

		if (bHit == true)
		{
			// �J�[�\���̃C���f�b�N�X�ԍ��ۑ�
			nCursorIdx = i;
			break;
		}
	}

	if (bHit && m_bDecide[nCntSelect] == false)
	{
		size.x += ((sizeOrigin.x * 1.2f) - size.x) * 0.25f;
		size.y += ((sizeOrigin.y * 1.2f) - size.y) * 0.25f;

		// �L�[�{�[�h���擾
		CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

		// �Q�[���p�b�h���擾
		CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, nCursorIdx))
		{
			// ���蔻��
			m_bDecide[nCntSelect] = true;
			m_apCursor[nCursorIdx]->SetEnbaleDicide(true);
			m_apCursor[nCursorIdx]->SetSelectIdx(nCntSelect);
			CManager::GetInstance()->SetByPlayerPartsType(nCursorIdx, nCntSelect);
		}

	}
	else
	{
		size.x += (sizeOrigin.x - size.x) * 0.25f;
		size.y += (sizeOrigin.y - size.y) * 0.25f;
	}

	if (m_bDecide[nCntSelect] == true)
	{
		m_pSelect2D[nCntSelect]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
	}
	else
	{
		m_pSelect2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);
	}

	// ���ݒ�
	m_pSelect2D[nCntSelect]->SetPosition(pos);
	m_pSelect2D[nCntSelect]->SetSize(size);

	return bHit;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CDecideCharacter::Draw(void)
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

