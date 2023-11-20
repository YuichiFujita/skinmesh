//=============================================================================
// 
//  �I�u�W�F�N�gX���� [objectX.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectX.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "texture.h"
#include "shadow.h"
#include "elevation.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CObjectX::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	D3DXMatrixIdentity(&m_mtxWorld);				// ���[���h�}�g���b�N�X
	SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));		// �X�P�[��
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �ʒu
	SetOldPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �O��̈ʒu
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// �ړ���
	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F
	SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// �T�C�Y
	m_bShadow = false;								// �e���g���Ă��邩�ǂ���
	m_nIdxTexure = NULL;							// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nIdxXFile = 0;								// X�t�@�C���̃C���f�b�N�X�ԍ�
	m_pShadow = NULL;								// �e�̏��
	m_nNumAll++;									// �������Z
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectX::~CObjectX()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObjectX::BindTexture(int *nIdx)
{
	// ���蓖�Ă�
	m_nIdxTexure = *nIdx;
}

//==========================================================================
// X�t�@�C���̃f�[�^���蓖��
//==========================================================================
void CObjectX::BindXData(int nIdxXFile)
{
	// ��񊄂蓖��
	m_nIdxXFile = nIdxXFile;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX *CObjectX::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CObjectX *pObjectX = NULL;

	if (pObjectX == NULL)
	{// NULL��������

		// �������̊m��
		pObjectX = DEBUG_NEW CObjectX;

		if (pObjectX != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObjectX->Init();
		}

		return pObjectX;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX *CObjectX::Create(const char *pFileName)
{
	// �����p�̃I�u�W�F�N�g
	CObjectX *pObjectX = NULL;

	if (pObjectX == NULL)
	{// NULL��������

		// �������̊m��
		pObjectX = DEBUG_NEW CObjectX;

		if (pObjectX != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjectX->Init(pFileName);

			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}

			// ��ސݒ�
			pObjectX->SetType(TYPE_XFILE);
		}

		return pObjectX;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX *CObjectX::Create(const char *pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bShadow)
{
	// �����p�̃I�u�W�F�N�g
	CObjectX *pObjectX = NULL;

	if (pObjectX == NULL)
	{// NULL��������

		// �������̊m��
		pObjectX = DEBUG_NEW CObjectX;

		if (pObjectX != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjectX->Init(pFileName);

			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}

			// ��ސݒ�
			//pObjectX->SetType(TYPE_XFILE);

			// �ʒu�E����
			pObjectX->SetPosition(pos);
			pObjectX->SetRotation(rot);
			pObjectX->m_bShadow = bShadow;

			if (bShadow == true)
			{
				// �e�ݒ�
				float f = pObjectX->GetVtxMax().x * 0.5f;
				pObjectX->m_pShadow = CShadow::Create(pObjectX->GetPosition(), f);
			}
		}

		return pObjectX;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX *CObjectX::Create(int nIdxXFile, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bShadow)
{
	// �����p�̃I�u�W�F�N�g
	CObjectX *pObjectX = NULL;

	if (pObjectX == NULL)
	{// NULL��������

		// �������̊m��
		pObjectX = DEBUG_NEW CObjectX;

		if (pObjectX != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjectX->Init(nIdxXFile);

			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}

			// ��ސݒ�
			//pObjectX->SetType(TYPE_XFILE);

			// �ʒu�E����
			pObjectX->SetPosition(pos);
			pObjectX->SetRotation(rot);
			pObjectX->m_bShadow = bShadow;

			if (bShadow == true)
			{
				// �e�ݒ�
				float f = pObjectX->GetVtxMax().x * 0.5f;
				pObjectX->m_pShadow = CShadow::Create(pObjectX->GetPosition(), f);
			}
		}

		return pObjectX;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectX::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CScene::GetXLoad();

	// X�t�@�C���̃��[�h
	m_nIdxXFile = pXLoad->XLoad("data\\MODEL\\radiokaikan_01.x");

	// X�t�@�C���̃f�[�^���蓖��
	BindXData(m_nIdxXFile);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectX::Init(const char *pFileName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CScene::GetXLoad();

	// X�t�@�C���̃��[�h
	m_nIdxXFile = pXLoad->XLoad(pFileName);

	// X�t�@�C���̃f�[�^���蓖��
	BindXData(m_nIdxXFile);

	if (m_nIdxXFile < 0)
	{
		return S_OK;
	}

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectX::Init(int nIdxXFile)
{
	// X�t�@�C���̃f�[�^���蓖��
	BindXData(nIdxXFile);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	return S_OK;
}


//==========================================================================
// �I������
//==========================================================================
void CObjectX::Uninit(void)
{
	// �e������
	if (m_pShadow != NULL)
	{
		//m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	m_nIdxTexure = NULL;

	// �I�u�W�F�N�g�̔j��
	Release();

	m_nNumAll--;	// �������Z
}

//==========================================================================
// ���S����
//==========================================================================
void CObjectX::Kill(void)
{
	// �e������
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// �I������
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectX::Update(void)
{
#if 1
	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CScene::GetXLoad();

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	D3DXVECTOR3 pos = GetPosition();
	/*bool bLand = false;
	float fHeight = CScene::GetElevation()->GetHeight(pos, bLand);
	if (bLand == true)
	{
		pos.y = fHeight;
	}

	SetPosition(pos);*/
#endif

	// �S���_�`�F�b�N
	//CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
}

//==========================================================================
// �����擾
//==========================================================================
float CObjectX::GetHeight(D3DXVECTOR3 pos, bool &bLand)
{
	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	if (pXData == NULL)
	{// NULL��������
		return 0.0f;
	}

	// �ő�̍���
	float fHeightMax = 0.0f;

	// �����̈ʒu
	D3DXVECTOR3 MyPosition = GetPosition();

	WORD* pIndexBuff;

	// �C���f�b�N�X�o�b�t�@�����b�N
	pXData->pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	for (int nCntTri = 0; nCntTri < pXData->nFaceNum; nCntTri++) 
	{
		
		int nIdx1 = (int)pIndexBuff[nCntTri * 3];
		int nIdx2 = (int)pIndexBuff[nCntTri * 3 + 1];
		int nIdx3 = (int)pIndexBuff[nCntTri * 3 + 2];

		// ���_�ʒu���
		D3DXVECTOR3 pos1 = pXData->pVtxPos[nIdx1] + MyPosition;
		D3DXVECTOR3 pos2 = pXData->pVtxPos[nIdx2] + MyPosition;
		D3DXVECTOR3 pos3 = pXData->pVtxPos[nIdx3] + MyPosition;

		// ��Βl(�}�C�i�X�����Ȃ�)�ƍő�̋�����r
		if (fabsf(pos1.x - pos.x) > pXData->fMaxVtxDistance ||
			fabsf(pos1.z - pos.z) > pXData->fMaxVtxDistance)
		{// ���_�����̕�������
			continue;
		}

		// �O�p�ō��������߂�
		
		bool bNowLand = false;
		float fNowHeight = GetVtxHeight(pos, pos1, pos3, pos2, bNowLand);

		if (bNowLand == true && fNowHeight > fHeightMax)
		{// ���n���Ă���

			// �ő卂���X�V
			fHeightMax = fNowHeight;
			bLand = true;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	pXData->pMesh->UnlockIndexBuffer();

	return fHeightMax;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectX::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f����
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K����ON�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K�����f�t�H���g�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}


//==========================================================================
// �`�揈��
//==========================================================================
void CObjectX::Draw(D3DXCOLOR col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	D3DXMATERIAL matNow;			// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = col;
	matNow.MatD3D.Ambient = col;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f����
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matNow.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K����ON�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K�����f�t�H���g�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}


//==========================================================================
// �`�揈��
//==========================================================================
void CObjectX::Draw(float fAlpha)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	D3DXMATERIAL matNow;			// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f����
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CScene::GetXLoad()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �s�����x�ݒ�
		matNow.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, fAlpha);
		matNow.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, fAlpha);
		matNow.MatD3D.Emissive = pMat[nCntMat].MatD3D.Emissive;
		matNow.MatD3D.Power = pMat[nCntMat].MatD3D.Power;
		matNow.MatD3D.Specular = pMat[nCntMat].MatD3D.Specular;

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&matNow.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K����ON�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K�����f�t�H���g�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObjectX::SetWorldMtx(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
D3DXMATRIX CObjectX::GetWorldMtx(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// �X�P�[���ݒ�
//==========================================================================
void CObjectX::SetScale(const D3DXVECTOR3 scale)
{
	m_scale = scale;
}

//==========================================================================
// �X�P�[���擾
//==========================================================================
D3DXVECTOR3 CObjectX::GetScale(void) const
{
	return m_scale;
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CObjectX::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CObjectX::GetColor(void) const
{
	return m_col;
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CObjectX::SetSize(const D3DXVECTOR3 size)
{
	m_fSize = size;
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR3 CObjectX::GetSize(void) const
{
	return m_fSize;
}

//==========================================================================
// ���_�̍ő�l�擾
//==========================================================================
D3DXVECTOR3 CObjectX::GetVtxMax(void) const
{
	// X�t�@�C���̃f�[�^�擾
	return CScene::GetXLoad()->GetMyObject(m_nIdxXFile)->vtxMax;
}

//==========================================================================
// ���_�̍ŏ��l�擾
//==========================================================================
D3DXVECTOR3 CObjectX::GetVtxMin(void) const
{
	// X�t�@�C���̃f�[�^�擾
	return CScene::GetXLoad()->GetMyObject(m_nIdxXFile)->vtxMin;
}

//==========================================================================
// X�t�@�C���̃C���f�b�N�X�擾
//==========================================================================
int CObjectX::GetIdxXFile(void) const
{
	return m_nIdxXFile;
}

//==========================================================================
// �e���g���Ă��邩�ǂ���
//==========================================================================
bool CObjectX::GetUseShadow(void) const
{
	return m_bShadow;
}

//==========================================================================
// �I�u�W�F�N�gX�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectX *CObjectX::GetObjectX(void)
{
	return this;
}

//==========================================================================
// �����擾
//==========================================================================
int CObjectX::GetNumAll(void)
{
	return m_nNumAll;
}
