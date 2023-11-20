//=============================================================================
// 
//  �I�u�W�F�N�g3D���� [object3D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define POLYGON_TOP		(4)								// �|���S���̒��_��
#define WIDTH			(80.0f)							// ����
#define HEIGHT			(60.0f)							// �c��
#define ANIM_SPEED		(10)							// �ǂݍ��݊Ԋu
#define MAX_PATTERN_U	(5)								// U�̕�����
#define MAX_PATTERN_V	(2)								// V�̕�����
#define MAX_PATTERN		(MAX_PATTERN_U * MAX_PATTERN_V)
#define MOVE_U			(1.0f / (float)MAX_PATTERN_U)	// U���W�ړ���
#define MOVE_V			(1.0f / (float)MAX_PATTERN_V)	// V���W�ړ���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	D3DXMatrixIdentity(&m_mtxWorld);				// ���[���h�}�g���b�N�X
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���̈ʒu
	m_rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���̌���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_fSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_pVtxBuff = NULL;								// ���_�o�b�t�@
	m_nTexIdx = 0;									// �e�N�X�`���̃C���f�b�N�X�ԍ�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject3D::~CObject3D()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObject3D::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_nTexIdx = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CObject3D *CObject3D::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CObject3D *pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULL��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3D;

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
// ��������
//==========================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �����p�̃I�u�W�F�N�g
	CObject3D *pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULL��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3D;

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
// ����������
//==========================================================================
HRESULT CObject3D::Init(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != NULL)
	{// ���ɏ�񂪓����Ă�ꍇ

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObject3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObject3D::Update(void)
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObject3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// �v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxRot, mtxTrans, mtxRotOrigin;
	D3DXMatrixIdentity(&mtxRotOrigin);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���̌����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRotOrigin, m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotOrigin);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject3D::SetVtx(void)
{

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 size = GetSize();
	D3DXCOLOR col = GetColor();

		// �ʒu���X�V
		pVtx[0].pos = D3DXVECTOR3(-size.x, +size.y, +size.z);
		pVtx[1].pos = D3DXVECTOR3(+size.x, +size.y, +size.z);
		pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, -size.z);
		pVtx[3].pos = D3DXVECTOR3(+size.x, -size.y, -size.z);

	// ���E���̃x�N�g��
	D3DXVECTOR3 vecLine0 = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vecLine1 = pVtx[2].pos - pVtx[0].pos;

	// �O�ϋ��߂�
	D3DXVECTOR3 Nor[4];

	// �O��
	D3DXVec3Cross(&Nor[0], &vecLine0, &vecLine1);

	// ���K��
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	Nor[0] = D3DXVECTOR3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));
	
	// ���K��
	D3DXVec3Normalize(&Nor[0], &Nor[0]);


	// ���E���̃x�N�g��
	vecLine0 = pVtx[2].pos - pVtx[3].pos;
	vecLine1 = pVtx[1].pos - pVtx[3].pos;

	// �O�ϋ��߂�
	Nor[3] = D3DXVECTOR3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// �O��
	D3DXVec3Cross(&Nor[3], &vecLine0, &vecLine1);

	// ���K��
	D3DXVec3Normalize(&Nor[3], &Nor[3]);

	Nor[1] = (Nor[0] + Nor[3]) / 2.0f;
	Nor[2] = (Nor[0] + Nor[3]) / 2.0f;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = Nor[0];
	pVtx[1].nor = Nor[1];
	pVtx[2].nor = Nor[2];
	pVtx[3].nor = Nor[3];

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObject3D::SetWorldMtx(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
D3DXMATRIX CObject3D::GetWorldMtx(void) const
{
	return m_mtxWorld;
}

//==========================================================================
//	���̈ʒu�ݒ�
//==========================================================================
void CObject3D::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
//	���̈ʒu�擾
//==========================================================================
D3DXVECTOR3 CObject3D::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// ���̌����ݒ�
//==========================================================================
void CObject3D::SetOriginRotation(const D3DXVECTOR3 rot)
{
	m_rotOrigin = rot;
}

//==========================================================================
// ���̌����擾
//==========================================================================
D3DXVECTOR3 CObject3D::GetOriginRotation(void) const
{
	return m_rotOrigin;
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CObject3D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CObject3D::GetColor(void) const
{
	return m_col;
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CObject3D::SetSize(const D3DXVECTOR3 size)
{
	m_fSize = size;
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR3 CObject3D::GetSize(void) const
{
	return m_fSize;
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3D *CObject3D::GetObject3D(void)
{
	return this;
}

//==========================================================================
// �I�u�W�F�N�g3D���b�V���I�u�W�F�N�g�̎擾
//==========================================================================
CObject3DMesh *CObject3D::GetObject3DMesh(void)
{
	return NULL;
}