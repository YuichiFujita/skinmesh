//=============================================================================
// 
//  ���b�V���t�B�[���h���� [meshfield.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CMeshField::m_apFilename[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\glass.jpg",
	"data\\TEXTURE\\FIELD\\senter.png",
	"data\\TEXTURE\\FIELD\\senterlong.png",
	"data\\TEXTURE\\FIELD\\sideL.png",
	"data\\TEXTURE\\FIELD\\sideR.png",
	"data\\TEXTURE\\FIELD\\odan.png",
	"data\\TEXTURE\\FIELD\\walkroad.jpg",
	"data\\TEXTURE\\FIELD\\walkroad_01.jpg",
	"data\\TEXTURE\\FIELD\\tile_04.jpg",
	"data\\TEXTURE\\FIELD\\tile_03.jpg",
	"data\\TEXTURE\\FIELD\\water_01.jpg",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMeshField::CMeshField(int nPriority) : CObject3DMesh(nPriority)
{
	m_type = TYPE_CONCREAT;	// ���
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMeshField::~CMeshField()
{

}

//==========================================================================
// ��������
//==========================================================================
CMeshField *CMeshField::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CMeshField *pObjMeshField = NULL;

	if (pObjMeshField == NULL)
	{// NULL��������

		// �������̊m��
		pObjMeshField = DEBUG_NEW CMeshField;

		if (pObjMeshField != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu�E����
			pObjMeshField->SetWidthBlock(1);
			pObjMeshField->SetHeightBlock(1);
			pObjMeshField->SetWidthLen(100.0f);
			pObjMeshField->SetHeightLen(100.0f);

			// �e�N�X�`���̊��蓖��
			pObjMeshField->m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(m_apFilename[pObjMeshField->m_type]);

			// �e�N�X�`���̊��蓖��
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// ����������
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return NULL;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidthLen, float fHeightLen, int nWidth, int nHeight, TYPE type, const char *aFileName, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CMeshField *pObjMeshField = NULL;

	if (pObjMeshField == NULL)
	{// NULL��������

		// �������̊m��
		pObjMeshField = DEBUG_NEW CMeshField(nPriority);

		if (pObjMeshField != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu�E����
			pObjMeshField->SetPosition(pos);
			pObjMeshField->SetRotation(rot);
			pObjMeshField->SetWidthBlock(nWidth);
			pObjMeshField->SetHeightBlock(nHeight);
			pObjMeshField->SetWidthLen(fWidthLen);
			pObjMeshField->SetHeightLen(fHeightLen);
			pObjMeshField->m_type = type;	// ���

			if (aFileName == NULL)
			{// NULL��������

				// �e�N�X�`���̊��蓖��
				pObjMeshField->m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(m_apFilename[pObjMeshField->m_type]);
			}
			else
			{// �t�@�C�����������Ă�����

				// �e�N�X�`���̊��蓖��
				pObjMeshField->m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(aFileName);
			}

			// �e�N�X�`���̊��蓖��
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// ����������
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMeshField::Init(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ��ސݒ�
	SetType(TYPE_MESHFIELD);

	// �I�u�W�F�N�g3D���b�V���̏���������
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMeshField::Uninit(void)
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMeshField::Update(void)
{
	// ���_���ݒ�
	//SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMeshField::Draw(void)
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ���߂郂�[�h

	// �`�揈��
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ���߂郂�[�h
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CMeshField::SetVtx(void)
{
	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ���b�V���t�B�[���h�I�u�W�F�N�g�̎擾
//==========================================================================
CMeshField *CMeshField::GetMyObject(void)
{
	return this;
}
