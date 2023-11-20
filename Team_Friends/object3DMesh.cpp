//=============================================================================
// 
//  �I�u�W�F�N�g3D���� [object3DMesh.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object3DMesh.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "input.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define CAL_VTX		(1280)		// �v�Z�p�̒��_��
#define MAX_MOVE	(8.0f)		// �ړ���
#define MAX_RANGE	(200.0f)	// �͈�
#define MOVE_LEN	(1.0f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int nInit = 0;
int nUniit = 0;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject3DMesh::CObject3DMesh(int nPriority) : CObject3D(nPriority)
{
	m_pVtxPos = NULL;		// ���_���W
	m_pVtxNor = NULL;		// ���_�@��
	m_pVtxCol = NULL;		// ���_�J���[
	m_pVtxTex = NULL;		// ���_�e�N�X�`�����W
	m_nNumIndex = 0;		// �C���f�b�N�X��
	m_nNumVertex = 0;		// ���_��
	m_nWidth = 0;			// ��������
	m_nHeight = 0;			// �c������
	m_fWidthLen = 0.0f;		// ���̒���
	m_fHeightLen = 0.0f;	// �c�̒���
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_type = TYPE_FIELD;	// ���b�V���̃^�C�v
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject3DMesh::~CObject3DMesh()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObject3DMesh::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_nTexIdx = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CObject3DMesh *CObject3DMesh::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CObject3DMesh *pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULL��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3DMesh;

		if (pObject3D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObject3D->Init();
		}

		return pObject3D;
	}

	return NULL;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CObject3DMesh *CObject3DMesh::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CObject3DMesh *pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULL��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3DMesh(nPriority);

		if (pObject3D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObject3D->Init();

			// �ʒu�E����
			pObject3D->SetPosition(pos);
			pObject3D->SetOriginPosition(pos);
			pObject3D->SetRotation(rot);
		}

		return pObject3D;
	}

	return NULL;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CObject3DMesh *CObject3DMesh::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidthLen, float fHeightLen, int nWidth, int nHeight, TYPE type, const char *pFileName, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CObject3DMesh *pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULL��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3DMesh(nPriority);

		if (pObject3D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �����̏���n��
			pObject3D->m_fWidthLen = fWidthLen;
			pObject3D->m_fHeightLen = fHeightLen;
			pObject3D->m_nWidth = nWidth;
			pObject3D->m_nHeight = nHeight;
			pObject3D->m_type = type;

			// �e�N�X�`���̊��蓖��
			if (pFileName != NULL)
			{// NULL����Ȃ�������
				pObject3D->m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(pFileName);
			}

			// �ʒu�E����
			pObject3D->SetPosition(pos);
			pObject3D->SetOriginPosition(pos);
			pObject3D->SetRotation(rot);

			// ����������
			pObject3D->Init(type);
		}

		return pObject3D;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject3DMesh::Init(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumIndex = (m_nHeight * ((m_nWidth + 1) * 2)) + (2 * (m_nHeight - 1));	// �C���f�b�N�X��
	m_nNumVertex = (m_nHeight + 1) * (m_nWidth + 1);							// ���_��
	m_type = TYPE_FIELD;	// ��ސݒ�

	m_pVtxPos = DEBUG_NEW D3DXVECTOR3[m_nNumVertex];	// ���_���W����
	memset(m_pVtxPos, 0, sizeof(D3DXVECTOR3) * m_nNumVertex);

	m_pVtxNor = DEBUG_NEW D3DXVECTOR3[m_nNumVertex];	// ���_�@��
	memset(m_pVtxNor, 0, sizeof(D3DXVECTOR3) * m_nNumVertex);

	m_pVtxCol = DEBUG_NEW D3DXCOLOR[m_nNumVertex];	// ���_�J���[
	memset(m_pVtxCol, 0, sizeof(D3DXCOLOR) * m_nNumVertex);

	m_pVtxTex = DEBUG_NEW D3DXVECTOR2[m_nNumVertex];	// ���_�e�N�X�`�����W
	memset(m_pVtxCol, 0, sizeof(D3DXVECTOR2) * m_nNumVertex);

	nInit++;

	 // ���_���W�ݒ�
	hr = CreateVertex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �C���f�b�N�X����
	hr = CreateIndex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject3DMesh::Init(TYPE type)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumIndex = (m_nHeight * ((m_nWidth + 1) * 2)) + (2 * (m_nHeight - 1));	// �C���f�b�N�X��
	m_nNumVertex = (m_nHeight + 1) * (m_nWidth + 1);							// ���_��
	m_type = type;	// ��ސݒ�

	m_pVtxPos = DEBUG_NEW D3DXVECTOR3[m_nNumVertex];	// ���_���W
	memset(m_pVtxPos, 0, sizeof(D3DXVECTOR3) * m_nNumVertex);

	m_pVtxNor = DEBUG_NEW D3DXVECTOR3[m_nNumVertex];	// ���_�@��
	memset(m_pVtxNor, 0, sizeof(D3DXVECTOR3) * m_nNumVertex);

	m_pVtxCol = DEBUG_NEW D3DXCOLOR[m_nNumVertex];		// ���_�J���[
	memset(m_pVtxCol, 0, sizeof(D3DXCOLOR) * m_nNumVertex);

	m_pVtxTex = DEBUG_NEW D3DXVECTOR2[m_nNumVertex];	// ���_�e�N�X�`�����W
	memset(m_pVtxCol, 0, sizeof(D3DXVECTOR2) * m_nNumVertex);

	nInit++;

	// ���_���W�ݒ�
	hr = CreateVertex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �C���f�b�N�X����
	hr = CreateIndex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			int nNowVtx = nCntWidth + (nCntHeight * (m_nWidth + 1));

			// ���_���W���蓖��
			m_pVtxPos[nNowVtx] = pVtx[0].pos;

			// ��ޖ��ɒ��_���ݒ�
			switch (m_type)
			{
			case TYPE_FIELD:
				m_pVtxNor[nNowVtx] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_pVtxTex[nNowVtx] = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(m_nWidth + 1))) * (1.0f / (float)(m_nWidth + 1)),
					((float)nCntHeight / (1.0f / (float)(m_nHeight + 1))) * (1.0f / (float)(m_nHeight + 1))
				);
				break;

			case TYPE_WALL:
				m_pVtxNor[nNowVtx] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				m_pVtxTex[nNowVtx] = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(m_nWidth + 1))) * (1.0f / (float)(m_nWidth + 1)),
					((float)nCntHeight / (1.0f / (float)(m_nHeight + 1))) * (1.0f / (float)(m_nHeight + 1))
				);
				break;

			case TYPE_CYLINDER:
				m_pVtxNor[nNowVtx] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_pVtxTex[nNowVtx] = D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(m_nWidth / 2)),
					nCntHeight * (1.0f / (float)(m_nHeight))
				);
				break;

			case TYPE_DOME:
				m_pVtxNor[nNowVtx] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_pVtxTex[nNowVtx] = -D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(m_nWidth)),
					nCntHeight * (1.0f / (float)(m_nHeight))
				);
				break;

			case TYPE_DONUTS:
				m_pVtxNor[nNowVtx] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_pVtxTex[nNowVtx] = D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(m_nWidth / 4)),
					nCntHeight * (1.0f / (float)(m_nHeight))
				);
				break;

			case TYPE_SPHERE:
				m_pVtxNor[nNowVtx] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_pVtxTex[nNowVtx] = -D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(m_nWidth / 4)),
					nCntHeight * (1.0f / (float)(m_nHeight / 4))
				);
				break;
			}

			m_pVtxCol[nNowVtx] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	return S_OK;
}

//==========================================================================
// ���_�o�b�t�@����
//==========================================================================
HRESULT CObject3DMesh::CreateVertex(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != NULL)
	{// ���ɏ�񂪓����Ă�ꍇ
		return E_FAIL;
	}

	// ���_�o�b�t�@�̐���
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	if (m_pVtxBuff == NULL)
	{// ���ɏ�񂪓����Ă�ꍇ
		return E_FAIL;
	}

	// ��ނɒ��_���W�ݒ�
	switch (m_type)
	{
	case TYPE_FIELD:
	{
		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
			{// ���̒��_�����J��Ԃ�

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(
					(m_fWidthLen * nCntWidth) - ((m_fWidthLen * m_nWidth) * 0.5f),
					m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].y,
					-((m_fHeightLen * nCntHeight) - ((m_fHeightLen * m_nHeight) * 0.5f)));

				// ���_���W���蓖��
				m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))] = pVtx[0].pos;
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();

		SetVtxField();
	}
		break;

	case TYPE_WALL:
	{
		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
			{// ���̒��_�����J��Ԃ�

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				(
					(m_fWidthLen * nCntWidth) - ((m_fWidthLen * m_nWidth) * 0.5f),
					(m_fHeightLen * m_nHeight) - ((m_fHeightLen * nCntHeight)),
					0.0f
				);

				// ���_���W���蓖��
				m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))] = pVtx[0].pos;

				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
		break;

	case TYPE_CYLINDER:
		SetVtxCylinder();
		break;

	case TYPE_DOME:
		SetVtxDome();
		break;

	case TYPE_DONUTS:
		SetVtxDonuts();
		break;

	case TYPE_SPHERE:
		SetVtxSphere();
		break;
	}

	return S_OK;
}

//==========================================================================
// �C���f�b�N�X����
//==========================================================================
HRESULT CObject3DMesh::CreateIndex(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �C���f�b�N�X�o�b�t�@�̐���
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;

	// ���_�ԍ��f�[�^�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight; nCntHeight++)
	{// �����̕��������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			pIdx[nCntIdx + 0] = nCntWidth + (m_nWidth + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((m_nWidth + 1) * nCntHeight);
			nCntIdx += 2;	// 2�����Ă邩��
		}

		if (nCntHeight + 1 < m_nHeight)
		{// �Ō�̂����͑ł��Ȃ�

			// ��ł�2��
			pIdx[nCntIdx + 0] = ((m_nWidth + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (m_nWidth + 1) * (nCntHeight + 2);
			nCntIdx += 2;	// 2�����Ă邩��
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObject3DMesh::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	nUniit++;
	// ���_���W�̔j��
	if (m_pVtxPos != NULL)
	{
		delete[] m_pVtxPos;
		m_pVtxPos = NULL;
	}

	// ���_�@���̔j��
	if (m_pVtxNor != NULL)
	{
		delete[] m_pVtxNor;
		m_pVtxNor = NULL;
	}

	// ���_�@���̔j��
	if (m_pVtxCol != NULL)
	{
		delete[] m_pVtxCol;
		m_pVtxCol = NULL;
	}

	// ���_�e�N�X�`�����W�̔j��
	if (m_pVtxTex != NULL)
	{
		delete[] m_pVtxTex;
		m_pVtxTex = NULL;
	}

	// �I������
	CObject3D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObject3DMesh::Update(void)
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// ���_�グ����
//==========================================================================
void CObject3DMesh::UPVtxField(D3DXVECTOR3 pos)
{
	if (m_type != TYPE_FIELD)
	{
		return;
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"\n"
		"���_�グ�����F[5, 6]\n"
		"���g�k�F[7, 8] �y%f, %f�z\n", m_fWidthLen, m_fHeightLen);

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			float fNowLength = 
				sqrtf((pos.x - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].x) * (pos.x - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].x)
				+ (pos.z - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].z) * (pos.z - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].z));

			float Wariai = fNowLength / MAX_RANGE;

			if (Wariai >= 1.0f)
			{// ������1.0f�ȉ�����
				continue;
			}

			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (pInputKeyboard->GetPress(DIK_6))
			{// 6�ŏグ��
				move.y = MAX_MOVE - (MAX_MOVE * Wariai);
			}

			if (pInputKeyboard->GetPress(DIK_5))
			{// 5�ŉ�����
				move.y = -MAX_MOVE - (-MAX_MOVE * Wariai);
			}

			// �e���_���W�m�F
			m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].y += move.y;
		}
	}

	if (pInputKeyboard->GetPress(DIK_7))
	{// 7�ŏグ��

		m_fWidthLen -= MOVE_LEN;
		m_fHeightLen -= MOVE_LEN;
	}

	if (pInputKeyboard->GetPress(DIK_8))
	{// 8�ŉ�����
		m_fWidthLen += MOVE_LEN;
		m_fHeightLen += MOVE_LEN;
	}

	ValueNormalize(m_fWidthLen, 99999999.0f, 10.0f);
	ValueNormalize(m_fHeightLen, 99999999.0f, 10.0f);
}


//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CObject3DMesh::CalWorldMtx(void)
{
	D3DXMATRIX m_mtxWorld = GetWorldMtx();			// �}�g���b�N�X�擾
	D3DXVECTOR3 m_rot = GetRotation();				// �����擾
	D3DXVECTOR3 m_rotOrigin = GetOriginRotation();	// ���̌����擾
	D3DXVECTOR3 m_pos = GetPosition();				// �ʒu�擾

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxRot, mtxTrans, mtxRotOrigin;
	D3DXMatrixIdentity(&mtxRotOrigin);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���̌����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRotOrigin, m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotOrigin);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	SetWorldMtx(m_mtxWorld);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObject3DMesh::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumVertex,
		0,
		m_nNumIndex - 2);

}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject3DMesh::SetVtx(void)
{
	// ��ނɒ��_���W�ݒ�
	switch (m_type)
	{
	case TYPE_FIELD:
		SetVtxField();
		break;

	case TYPE_WALL:
		SetVtxWall();
		break;

	case TYPE_CYLINDER:
		SetVtxCylinder();
		break;

	case TYPE_DOME:
		SetVtxDome();
		break;

	case TYPE_DONUTS:
		SetVtxDonuts();
		break;

	case TYPE_SPHERE:
		SetVtxSphere();
		break;
	}
}

//==========================================================================
// �t�B�[���h�̏����������_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetInitVtxField(void)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				(m_fWidthLen * nCntWidth) - ((m_fWidthLen * m_nWidth) * 0.5f),
				m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].y,
				-((m_fHeightLen * nCntHeight) - ((m_fHeightLen * m_nHeight) * 0.5f)));

			// ���_���W���蓖��
			m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))] = pVtx[0].pos;

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = m_pVtxNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = m_pVtxCol[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = m_pVtxTex[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �t�B�[���h�̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxField(void)
{

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			if (nCntWidth + (nCntHeight * (m_nWidth + 1)) >= (m_nWidth + 1) * (m_nHeight + 1))
			{// ���_����������

				int n = 0;
			}

			// ���_���W���蓖��
			pVtx[0].pos = m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = m_pVtxNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = m_pVtxCol[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = m_pVtxTex[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �E�H�[���̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxWall(void)
{

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_���W���蓖��
			m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))] = pVtx[0].pos;

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = m_pVtxNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = m_pVtxCol[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = m_pVtxTex[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �V�����_�[�̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxCylinder(void)
{
	D3DXCOLOR col = GetColor();			// �F
	D3DXVECTOR3 pos = GetPosition();	// �ʒu
	D3DXVECTOR3 posVtx[CAL_VTX];		// �v�Z�p�̍��W
	float fRot = (D3DX_PI * 2) / (float)(m_nWidth);	//1���Ƃ̊p�x�����߂�
	D3DXVECTOR3 NormalizeNor[CAL_VTX];	// ���K���p

	for (int nCntHeight = 0; nCntHeight < m_nHeight; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			// ���_���W���߂�
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % m_nWidth * fRot) * m_fWidthLen,
				(m_fHeightLen * m_nHeight) - ((m_fHeightLen * nCntHeight)),
				cosf(nCntWidth % m_nWidth * fRot) * m_fWidthLen
			);

			// ���_���W���߂�
			posVtx[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % m_nWidth * fRot) * m_fWidthLen,
				(m_fHeightLen * m_nHeight) - ((m_fHeightLen * (nCntHeight + 1))),
				cosf(nCntWidth % m_nWidth * fRot) * m_fWidthLen
			);

			// �e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] - pos;
			NormalizeNor[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))] - pos;

			// �o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))],				&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))],	&NormalizeNor[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))]);
		}
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = m_pVtxTex[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �h�[���̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxDome(void)
{
	D3DXCOLOR col = GetColor();			// �F
	D3DXVECTOR3 pos = GetPosition();	// �ʒu
	D3DXVECTOR3 posVtx[CAL_VTX];		// �v�Z�p�̍��W
	D3DXVECTOR3 NormalizeNor[CAL_VTX];	// �v�Z�p�̃x�N�g��
	float fRotWidth = (D3DX_PI * 2) / (float)(m_nWidth);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotHeight = (D3DX_PI * 0.5f) / (float)(m_nHeight);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotCalW = 0.0f;	// ���̍���̊p�x
	float fRotCalH = 0.0f;	// �c�̍���̊p�x

	// �Ō�̓_
	posVtx[m_nWidth + (m_nHeight * (m_nWidth + 1))] = D3DXVECTOR3(0.0f, m_fHeightLen, 0.0f);

	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ����̊p�x������o��
			fRotCalH = (float)(nCntHeight % (m_nHeight +1 )) * fRotHeight;
			fRotCalW = (fRotWidth * m_nWidth) - ((float)(nCntWidth % (m_nWidth + 1)) * fRotWidth);

			// �p�x�̐��K��
			RotNormalize(fRotCalW);
			RotNormalize(fRotCalH);

			// ���W����o��
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].x = cosf(fRotCalH) * sinf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].z = cosf(fRotCalH) * cosf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].y = sinf(fRotCalH) * m_fHeightLen;

			// �e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] - pos;

			// �o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))]);
		}
	}
	
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = m_pVtxTex[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �h�[�i�c�̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxDonuts(void)
{
	D3DXCOLOR col = GetColor();			// �F
	D3DXVECTOR3 pos = GetPosition();	// �ʒu
	D3DXVECTOR3 posVtx[CAL_VTX] = {};		// �v�Z�p�̍��W
	D3DXVECTOR3 NormalizeNor[CAL_VTX] = {};	// �v�Z�p�̃x�N�g��

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�ݒ�
			pVtx[0].pos = m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] - pos;

			// �o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))]);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = m_pVtxTex[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// ���̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxSphere(void)
{
	D3DXCOLOR col = GetColor();			// �F
	D3DXVECTOR3 pos = GetPosition();	// �ʒu
	D3DXVECTOR3 posVtx[CAL_VTX];		// �v�Z�p�̍��W
	D3DXVECTOR3 NormalizeNor[CAL_VTX];	// �v�Z�p�̃x�N�g��
	float fRotWidth = (D3DX_PI * 2) / (float)(m_nWidth);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotHeight = (D3DX_PI * 2) / (float)(m_nHeight);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotCalW = 0.0f;	// ���̍���̊p�x
	float fRotCalH = 0.0f;	// �c�̍���̊p�x

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �Ō�̓_
	pVtx[m_nWidth + (m_nHeight * (m_nWidth + 1))].pos = D3DXVECTOR3(0.0f, m_fHeightLen, 0.0f);

	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		// ���Z�b�g
		fRotCalW = 0.0f;

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ����̊p�x������o��
			fRotCalH = (float)nCntHeight * fRotHeight;
			fRotCalW = (float)nCntWidth * fRotWidth;

			// �p�x�̐��K��
			/*RotNormalize(fRotCalW);
			RotNormalize(fRotCalH);*/

			// ���W����o��
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].x = cosf(fRotCalH) * sinf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].z = cosf(fRotCalH) * cosf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].y = sinf(fRotCalH) * m_fHeightLen;

			// ���_���W�̐ݒ�
			pVtx[0].pos = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] - pos;

			// �o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))]);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = m_pVtxTex[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �C���f�b�N�X���擾
//==========================================================================
int CObject3DMesh::GetNumIndex(void)
{
	return m_nNumIndex;
}

//==========================================================================
// �C���f�b�N�X���ݒ�
//==========================================================================
void CObject3DMesh::SetNumIndex(int nWidth, int nHeight)
{
	m_nNumIndex = (nHeight * ((nWidth + 1) * 2)) + (2 * (nHeight - 1));	// �C���f�b�N�X��
}

//==========================================================================
// ���_���ݒ�
//==========================================================================
int CObject3DMesh::GetNumVertex(void)
{
	return m_nNumVertex;
}

//==========================================================================
// ���_���擾
//==========================================================================
void CObject3DMesh::SetNumVertex(int nWidth, int nHeight)
{
	m_nNumVertex = (nHeight + 1) * (nWidth + 1);							// ���_��
}

//==========================================================================
// ���_���W�擾
//==========================================================================
D3DXVECTOR3 *CObject3DMesh::GetVtxPos(void)
{
	return m_pVtxPos;
}

//==========================================================================
// ���_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxPos(D3DXVECTOR3 *pos)
{
	m_pVtxPos = pos;
}

//==========================================================================
// ���_�x�N�g���擾
//==========================================================================
D3DXVECTOR3 *CObject3DMesh::GetVtxNor(void)
{
	return m_pVtxNor;
}

//==========================================================================
// ���_�x�N�g���擾
//==========================================================================
void CObject3DMesh::SetVtxNor(D3DXVECTOR3 *nor)
{
	m_pVtxNor = nor;
}

//==========================================================================
// ���_�J���[�擾
//==========================================================================
D3DXCOLOR *CObject3DMesh::GetVtxCol(void)
{
	return m_pVtxCol;
}

//==========================================================================
// ���_�J���[�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxCol(D3DXCOLOR *col)
{
	m_pVtxCol = col;
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CObject3DMesh::GetVtxTex(void)
{
	return m_pVtxTex;
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxTex(D3DXVECTOR2 *tex)
{
	m_pVtxTex = tex;
}

//==========================================================================
// ���̕������擾
//==========================================================================
int CObject3DMesh::GetWidthBlock(void)
{
	return m_nWidth;
}

//==========================================================================
// ���̕������ݒ�
//==========================================================================
void CObject3DMesh::SetWidthBlock(int nWidth)
{
	m_nWidth = nWidth;
}

//==========================================================================
// �����̕������擾
//==========================================================================
int CObject3DMesh::GetHeightBlock(void)
{
	return m_nHeight;
}

//==========================================================================
// �����̕������ݒ�
//==========================================================================
void CObject3DMesh::SetHeightBlock(int nHeight)
{
	m_nHeight = nHeight;
}

//==========================================================================
// ���̒����擾
//==========================================================================
float CObject3DMesh::GetWidthLen(void)
{
	return m_fWidthLen;
}

//==========================================================================
// ���̒����ݒ�
//==========================================================================
void CObject3DMesh::SetWidthLen(float fLen)
{
	m_fWidthLen = fLen;
}

//==========================================================================
// �����̒����擾
//==========================================================================
float CObject3DMesh::GetHeightLen(void)
{
	return m_fHeightLen;
}

//==========================================================================
// �����̒����ݒ�
//==========================================================================
void CObject3DMesh::SetHeightLen(float fLen)
{
	m_fHeightLen = fLen;
}

//==========================================================================
// �e�N�X�`���̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObject3DMesh::GetIdxTex(void)
{
	return m_nTexIdx;
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3DMesh *CObject3DMesh::GetObject3DMesh(void)
{
	return this;
}

LPDIRECT3DVERTEXBUFFER9 CObject3DMesh::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
