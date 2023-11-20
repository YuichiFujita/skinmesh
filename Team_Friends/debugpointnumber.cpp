//=============================================================================
// 
//  �|�C���g�̐������� [debugpointnumber.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "debugpointnumber.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "input.h"
#include "3D_effect.h"
#include "multinumber.h"
#include "game.h"

//==========================================================================
// �}�N����`
//==========================================================================
DWORD dwGameStartsTime = 0;
//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDebugPointNumber::CDebugPointNumber(int nPriority) : CObject(nPriority)
{
	// �[���N���A
	m_nIdxParent = 0;		// �e�̔ԍ�
	m_pMultiNumber = NULL;	// �����̃I�u�W�F�N�g
	dwGameStartsTime = timeGetTime();
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDebugPointNumber::~CDebugPointNumber()
{

}

//==========================================================================
// ��������
//==========================================================================
CDebugPointNumber *CDebugPointNumber::Create(int nIdx)
{
	// �����p�̃I�u�W�F�N�g
	CDebugPointNumber *pObjectX = NULL;

	if (pObjectX == NULL)
	{// NULL��������

		// �������̊m��
		pObjectX = DEBUG_NEW CDebugPointNumber;

		if (pObjectX != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �e�̔ԍ�
			pObjectX->m_nIdxParent = nIdx;

			// ����������
			HRESULT hr = pObjectX->Init();
			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pObjectX;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDebugPointNumber::Init(void)
{
	//if (CGame::GetMapManager() == NULL)
	//{// NULL�Ŕ�����
	//	return E_FAIL;
	//}

	//// �}�b�v�}�l�[�W���擾
	//CMapManager *pMapManager = CGame::GetMapManager();

	// ��������
	m_pMultiNumber = CMultiNumber::Create(
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR2(50.0f, 60.0f),
		GetDigit(m_nIdxParent),
		CNumber::OBJECTTYPE_BILLBOARD);	// �����̃I�u�W�F�N�g

	// ��ސݒ�
	SetType(CObject::TYPE_NUMBER);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDebugPointNumber::Uninit(void)
{
	// �����̃I�u�W�F�N�g�̏I������
	if (m_pMultiNumber != NULL)
	{
		m_pMultiNumber->Uninit();
		delete m_pMultiNumber;
		m_pMultiNumber = NULL;
	}

	// �I������
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CDebugPointNumber::Update(void)
{

	// �l�̐ݒ菈��
	m_pMultiNumber->SetValue(m_nIdxParent);

	//// �ʒu�X�V
	//D3DXVECTOR3 pos = pMapManager->GetControlPoint(m_nIdxParent + 1);
	//pos.y += 50.0f;
	m_pMultiNumber->SetPosition(GetPosition());

	//D3DXCOLOR col = m_pMultiNumber->GetColor();

	//DWORD dwGameTime = timeGetTime();	// ���ݎ������擾
	//float fTime = (float)(dwGameTime - dwGameStartsTime);
	//fTime *= 0.001f;

	//if (fTime >= 1.0f)
	//{
	//	dwGameStartsTime = timeGetTime();	// ���ݎ������擾
	//}

	//col.a = Lerp(0.0f, 1.0f, fTime);

	//m_pMultiNumber->SetColor(col);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CDebugPointNumber::Draw(void)
{

}
