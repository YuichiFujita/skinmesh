//=============================================================================
// 
//  �������� [multinumber.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "multinumber.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "objectBillboard.h"
#include "texture.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE			"data\\TEXTURE\\number_blackclover_01.png"	// �e�N�X�`���̃t�@�C��
#define TEX_U			(0.1f)							// U�̕���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMultiNumber::CMultiNumber(int nPriority)
{
	// �l�̃N���A
	m_nNum = 0;				// ����
	m_nNumNumber = 0;		// �����̐�
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nPriority = 0;		// �D�揇��
	m_ppMultiNumber = NULL;	// �����̃I�u�W�F�N�g
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_col =mylib_const::DEFAULT_COLOR;		// �F
	size = D3DXVECTOR2(0.0f, 0.0f);	// �����̃T�C�Y
	m_objType = CNumber::OBJECTTYPE_2D;	// �I�u�W�F�N�g�̎��
	m_bDigitDraw = false;				// �����`��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMultiNumber::~CMultiNumber()
{

}

//==========================================================================
// ��������
//==========================================================================
CMultiNumber *CMultiNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, bool bDigitDraw, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CMultiNumber *pNumber = NULL;

	if (pNumber == NULL)
	{// NULL��������

		// �������̊m��
		pNumber = DEBUG_NEW CMultiNumber;

		if (pNumber != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �I�u�W�F�N�g�̎��
			pNumber->m_objType = objtype;

			// �T�C�Y
			pNumber->size = size;

			// �ʒu
			pNumber->m_pos = pos;

			// �����̐�
			pNumber->m_nNumNumber = nNum;

			// �����`��
			pNumber->m_bDigitDraw = bDigitDraw;

			// �D�揇��
			pNumber->m_nPriority = nPriority;

			// ����������
			pNumber->Init();
		}
		else
		{
			delete pNumber;
			pNumber = NULL;
		}

		return pNumber;
	}

	return NULL;
}


//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CMultiNumber *CMultiNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, const char *pTextureFile, bool bDigitDraw, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CMultiNumber *pNumber = NULL;

	if (pNumber == NULL)
	{// NULL��������

		// �������̊m��
		pNumber = DEBUG_NEW CMultiNumber;

		if (pNumber != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �I�u�W�F�N�g�̎��
			pNumber->m_objType = objtype;

			// �T�C�Y
			pNumber->size = size;

			// �ʒu
			pNumber->m_pos = pos;

			// �����̐�
			pNumber->m_nNumNumber = nNum;

			// �����`��
			pNumber->m_bDigitDraw = bDigitDraw;

			// �D�揇��
			pNumber->m_nPriority = nPriority;

			// �e�N�X�`���ǂݍ���
			pNumber->m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(pTextureFile);

			// ����������
			pNumber->Init();
		}
		else
		{
			delete pNumber;
			pNumber = NULL;
		}

		return pNumber;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMultiNumber::Init(void)
{
	// �����������m��
	m_ppMultiNumber = DEBUG_NEW (CNumber*[m_nNumNumber]);

	// ��������
	if (m_nTexIdx == 0)
	{
		m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);
	}

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		// ��������
		m_ppMultiNumber[nCntNum] = CNumber::Create(m_objType, m_nPriority);

		// �e��ϐ��̏�����
		m_ppMultiNumber[nCntNum]->SetSize(size);	// �T�C�Y
		m_ppMultiNumber[nCntNum]->SetPosition(D3DXVECTOR3(m_pos.x + size.y * nCntNum, m_pos.y, m_pos.z));	// �ʒu

		// ��ނ̐ݒ�
		m_ppMultiNumber[nCntNum]->SetType(CObject::TYPE_SCORE);

		// �e�N�X�`���̊��蓖��
		m_ppMultiNumber[nCntNum]->BindTexture(m_nTexIdx);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMultiNumber::Uninit(void)
{
	// �I������
	if (m_ppMultiNumber != NULL)
	{// NULL����Ȃ�������

		// �e�����I�u�W�F�N�g�̔j��
		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != NULL)
			{
				m_ppMultiNumber[nCntNum]->Uninit();
				delete m_ppMultiNumber[nCntNum];
				m_ppMultiNumber[nCntNum] = NULL;
			}
		}
		delete[] m_ppMultiNumber;
		m_ppMultiNumber = NULL;
	}
}

//==========================================================================
// �������
//==========================================================================
void CMultiNumber::Release(void)
{
	// �I������
	if (m_ppMultiNumber != NULL)
	{// NULL����Ȃ�������

		// �e�����I�u�W�F�N�g�̔j��
		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != NULL)
			{
				m_ppMultiNumber[nCntNum]->Release();
				delete m_ppMultiNumber[nCntNum];
				m_ppMultiNumber[nCntNum] = NULL;
			}
		}
		delete m_ppMultiNumber;
		m_ppMultiNumber = NULL;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CMultiNumber::Update(void)
{
	if (m_bDigitDraw == false)
	{// �����`�悶��Ȃ��ꍇ
		return;
	}

	int nNumberDigit = GetDigit(m_nNum);

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == NULL)
		{// NULL��������
			continue;
		}

		// �E�l�߂̕`����
		switch (m_objType)
		{
		case CNumber::OBJECTTYPE_2D:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// ����
				m_ppMultiNumber[nCntNum]->GetObject2D()->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->GetObject2D()->SetEnableDisp(false);
			}
			break;

		case CNumber::OBJECTTYPE_BILLBOARD:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// ����
				m_ppMultiNumber[nCntNum]->GetObjectBillboard()->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->GetObjectBillboard()->SetEnableDisp(false);
			}
			break;
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMultiNumber::Draw(void)
{
	int nNumNumber = m_nNumNumber;
	if (m_bDigitDraw == true)
	{// �����`�悾������
		nNumNumber = GetDigit(m_nNum);
	}

	for (int nCntNum = 0; nCntNum < nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == NULL)
		{// NULL��������
			continue;
		}
		m_ppMultiNumber[nCntNum]->Draw();
	}
}

//==========================================================================
// �l�̐ݒ菈��
//==========================================================================
void CMultiNumber::AddNumber(int nValue)
{
	m_nNum += nValue;

	// �l�̐ݒ菈��
	SetValue();
}

//==========================================================================
// �l�̐ݒ菈��
//==========================================================================
void CMultiNumber::SetValue(int nValue)
{
	m_nNum = nValue;

	// �l�̐ݒ菈��
	SetValue();
}

//==========================================================================
// �l�̎擾����
//==========================================================================
int CMultiNumber::GetValue(void)
{
	return m_nNum;
}

//==========================================================================
// �l�̐ݒ菈��
//==========================================================================
void CMultiNumber::SetValue(void)
{
	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == NULL)
		{// NULL��������
			continue;
		}

		int aTexU = m_nNum % (int)std::powf(10, m_nNumNumber + 1 - nCntNum) / ((int)std::powf(10, m_nNumNumber - nCntNum) / 10);

		if (aTexU < 0)
		{
			aTexU = 0;
		}

		// �e�N�X�`���|�C���^�擾
		D3DXVECTOR2 *pTex = m_ppMultiNumber[nCntNum]->GetTex();

		// �e�N�X�`�����W�̐ݒ�
		pTex[0] = D3DXVECTOR2(aTexU * TEX_U, 0.0f);
		pTex[1] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 0.0f);
		pTex[2] = D3DXVECTOR2(aTexU * TEX_U, 1.0f);
		pTex[3] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 1.0f);

		// ���_�ݒ�
		m_ppMultiNumber[nCntNum]->SetVtx();
	}
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CMultiNumber::SetPosition(const D3DXVECTOR3 pos)
{
	// �ʒu�ݒ�
	m_pos = pos;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != NULL)
		{
			m_ppMultiNumber[nCntNum]->SetPosition(D3DXVECTOR3(m_pos.x + size.y * nCntNum, m_pos.y, m_pos.z));	// �ʒu
		}
	}
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CMultiNumber::GetPosition(void) const
{
	return m_pos;
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CMultiNumber::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != NULL)
		{
			m_ppMultiNumber[nCntNum]->SetColor(m_col);	// �F
		}
	}
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CMultiNumber::GetColor(void) const
{
	return m_col;
}