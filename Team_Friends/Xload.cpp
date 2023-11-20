//=============================================================================
// 
//  X�t�@�C������ [Xload.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "Xload.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "calculation.h"

// X�t�@�C���I�u�W�F�N�g��ǂݍ��ރt�@�C��
#include "map.h"
#include "ballast.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CXLoad::SXFile CXLoad::m_pXFile[mylib_const::MAX_OBJ] = {};	// X�t�@�C���̏��
int CXLoad::m_nNumAll = 0;				// �ǂݍ��񂾑���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CXLoad::CXLoad()
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		m_pXFile[nCntData].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���f���̍ŏ��l
		m_pXFile[nCntData].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���f���̍ő�l
		m_pXFile[nCntData].pVtxBuff = NULL;								// ���_�o�b�t�@�̃|�C���^
		m_pXFile[nCntData].pMesh = NULL;								// ���b�V��(���_���)�ւ̃|�C���^
		m_pXFile[nCntData].pBuffMat = NULL;								// �}�e���A���ւ̃|�C���^
		m_pXFile[nCntData].dwNumMat = 0;								// �}�e���A���̐�
		m_pXFile[nCntData].nIdxTexture = NULL;							// �e�N�X�`���̃C���f�b�N�X�ԍ�
		m_pXFile[nCntData].nFileNameLen = 0;							// �t�@�C�����̕�����

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			// �}�e���A���̃f�[�^
			ZeroMemory(&m_pXFile[nCntData].pMatData[nCntMat], sizeof(D3DXMATERIAL));
		}

		for (int nCntFile = 0; nCntFile < mylib_const::MAX_STRING; nCntFile++)
		{
			m_pXFile[nCntData].acFilename[nCntFile] = NULL;	// �t�@�C����
		}
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CXLoad::~CXLoad()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CXLoad::Init(void)
{
	// ���f���̓ǂݍ���
	if (FAILED(map::ReadXFile()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	//// ���I�̓ǂݍ���
	//if (FAILED(CBallast::Load()))
	//{// ���s�����ꍇ
	//	return E_FAIL;
	//}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CXLoad::Uninit(void)
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		if (m_pXFile[nCntData].nIdxTexture != NULL)
		{// NULL����Ȃ�������

			// �J������
			delete[] m_pXFile[nCntData].nIdxTexture;
			m_pXFile[nCntData].nIdxTexture = NULL;
		}

		// ���b�V���̔j��
		if (m_pXFile[nCntData].pMesh != NULL)
		{
			m_pXFile[nCntData].pMesh->Release();
			m_pXFile[nCntData].pMesh = NULL;
		}

		// �}�e���A���̔j��
		if (m_pXFile[nCntData].pBuffMat != NULL)
		{
			m_pXFile[nCntData].pBuffMat->Release();
			m_pXFile[nCntData].pBuffMat = NULL;
		}

		// ���_���W�̔j��
		if (m_pXFile[nCntData].pVtxPos != NULL)
		{
			delete[] m_pXFile[nCntData].pVtxPos;
			m_pXFile[nCntData].pVtxPos = NULL;
		}
	}

	m_nNumAll = 0;
}

//==========================================================================
// X�t�@�C���̔j��
//==========================================================================
void CXLoad::Unload(void)
{

}


//==========================================================================
// X�t�@�C���̓ǂݍ���
//==========================================================================
int CXLoad::XLoad(const char *pFileName)
{
	// �ő吔�擾
	int nIdx = 0;
	int nNumAll = GetNumAll() + 1;
	int nNowLen = 0;

	if (pFileName != NULL)
	{
		nNowLen = strlen(pFileName);
	}

	if (pFileName != NULL)
	{// NULL����Ȃ�������
		for (int nCntData = 0; nCntData < nNumAll; nCntData++)
		{
			if (m_pXFile[nCntData].nFileNameLen == nNowLen)
			{// �t�@�C�����̒�����������������

				// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
				if (strcmp(m_pXFile[nCntData].acFilename, pFileName) == 0)
				{// �t�@�C��������v���Ă���

					// �C���f�b�N�X�ԍ��ۑ�
					nIdx = nCntData;
					return nIdx;
				}
			}
		}
	}

	if (pFileName != NULL)
	{// NULL����Ȃ�������

		// �ǂݍ���
		HRESULT hr = Load(pFileName);

		if (FAILED(hr))
		{// ���s���Ă�����
			return E_FAIL;
		}

		// �C���f�b�N�X�ԍ��ۑ�
		nIdx = nNumAll - 1;
		return nIdx;
	}

	return -1;
}

//==========================================================================
// ���[�h����
//==========================================================================
HRESULT CXLoad::Load(const char *pFileName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	int nIdx = m_nNumAll;

	//X�t�@�C���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pXFile[nIdx].pBuffMat,
		NULL,
		&m_pXFile[nIdx].dwNumMat,
		&m_pXFile[nIdx].pMesh);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �t�@�C�����ƒ����ۑ�
	strcpy(m_pXFile[nIdx].acFilename, pFileName);
	m_pXFile[nIdx].nFileNameLen = strlen(&m_pXFile[nIdx].acFilename[0]);

	if (m_pXFile[nIdx].nIdxTexture == NULL)
	{
		// �e�N�X�`���̃C���f�b�N�X�ԍ�
		m_pXFile[nIdx].nIdxTexture = DEBUG_NEW int[(int)m_pXFile[nIdx].dwNumMat];
	}

	if (m_pXFile[nIdx].nIdxTexture == NULL)
	{// �m�ۂł��Ă��Ȃ��ꍇ
		return E_FAIL;
	}

	// ���_���擾
	m_pXFile[nIdx].nVtxNum = m_pXFile[nIdx].pMesh->GetNumVertices();

	// ���_�����Ń������m��
	m_pXFile[nIdx].pVtxPos = DEBUG_NEW D3DXVECTOR3[m_pXFile[nIdx].nVtxNum];

	// �ʂ̐��擾
	m_pXFile[nIdx].nFaceNum = m_pXFile[nIdx].pMesh->GetNumFaces();

	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pXFile[nIdx].pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)m_pXFile[nIdx].dwNumMat; nCntMat++)
	{
		m_pXFile[nIdx].nIdxTexture[nCntMat] = 0;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{// �t�@�C�������݂���

			// �e�N�X�`���̓ǂݍ���
			m_pXFile[nIdx].nIdxTexture[nCntMat] = CManager::GetInstance()->GetTexture()->Regist(pMat[nCntMat].pTextureFilename);

			if (FAILED(hr))
			{// ���s���Ă�����
				return E_FAIL;
			}
		}
	}

	BYTE* pVtxBuff;

	// ���_�o�b�t�@�����b�N
	m_pXFile[nIdx].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_pXFile[nIdx].pMesh->GetFVF());

	// ���_���W
	for (int nCntVtx = 0; nCntVtx < m_pXFile[nIdx].nVtxNum; nCntVtx++)
	{
		// ���_���W���
		m_pXFile[nIdx].pVtxPos[nCntVtx] = *(D3DXVECTOR3*)pVtxBuff;

		// �T�C�Y���|�C���^�ړ�
		pVtxBuff += dwSizeFVF;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pXFile[nIdx].pMesh->UnlockVertexBuffer();

	// �S���_�`�F�b�N
	CheckVtx(0.0f, &m_pXFile[nIdx].vtxMax, &m_pXFile[nIdx].vtxMin, m_pXFile[nIdx].pMesh, m_pXFile[nIdx].pVtxBuff);



	// �C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIndexBuff;
	m_pXFile[nIdx].pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	for (int nCntIdx = 0; nCntIdx < m_pXFile[nIdx].nFaceNum; nCntIdx++)
	{
		// �O�p�`���`������C���f�b�N�X, �ʖ�������3������
		int nIdx1 = (int)pIndexBuff[nCntIdx * 3];
		int nIdx2 = (int)pIndexBuff[nCntIdx * 3 + 1];
		int nIdx3 = (int)pIndexBuff[nCntIdx * 3 + 2];

		// �ꎞ���
		D3DXVECTOR3 pos1 = m_pXFile[nIdx].pVtxPos[nIdx1];
		D3DXVECTOR3 pos2 = m_pXFile[nIdx].pVtxPos[nIdx2];
		D3DXVECTOR3 pos3 = m_pXFile[nIdx].pVtxPos[nIdx3];

		// ���_�Ԃ̍ő勗��
		float fVtxDistance = 0.0f;

		// 2�_�̋���
		fVtxDistance = sqrtf(
			((pos1.x - pos2.x) * (pos1.x - pos2.x)) +
			((pos1.z - pos2.z) * (pos1.z - pos2.z)));

		if (fVtxDistance > m_pXFile[nIdx].fMaxVtxDistance)
		{
			// �ő勗���ۑ�
			m_pXFile[nIdx].fMaxVtxDistance = fVtxDistance;
		}

		// 2�_�̋���
		fVtxDistance = sqrtf(
			((pos2.x - pos3.x) * (pos2.x - pos3.x)) +
			((pos2.z - pos3.z) * (pos2.z - pos3.z)));
		if (fVtxDistance > m_pXFile[nIdx].fMaxVtxDistance)
		{
			// �ő勗���ۑ�
			m_pXFile[nIdx].fMaxVtxDistance = fVtxDistance;
		}

		// 2�_�̋���
		fVtxDistance = sqrtf(
			((pos3.x - pos1.x) * (pos3.x - pos1.x)) +
			((pos3.z - pos1.z) * (pos3.z - pos1.z)));
		if (fVtxDistance > m_pXFile[nIdx].fMaxVtxDistance)
		{
			// �ő勗���ۑ�
			m_pXFile[nIdx].fMaxVtxDistance = fVtxDistance;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pXFile[nIdx].pMesh->UnlockIndexBuffer();


	// �������Z
	m_nNumAll++;

	return S_OK;
}

//==========================================================================
// X�t�@�C���̃f�[�^�擾
//==========================================================================
CXLoad::SXFile *CXLoad::GetMyObject(int nIdx)
{
	return &m_pXFile[nIdx];
}

//==========================================================================
// X�t�@�C���̑����擾
//==========================================================================
int CXLoad::GetNumAll(void)
{
	return m_nNumAll;
}