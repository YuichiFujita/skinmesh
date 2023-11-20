//=============================================================================
// 
//  �I�u�W�F�N�g2D���� [object2D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object2D.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "texture.h"
#include "pause.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE_NAME	"data\\TEXTURE\\titlelog_00.png"	// �e�N�X�`����
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
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_size = D3DXVECTOR2(0.0f, 0.0f);			// �T�C�Y
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);		// ���̃T�C�Y
	m_fLength = 0.0f;							// �Ίp���̒���
	m_fAngle = 0.0f;							// �Ίp���̌���
	m_pVtxBuff = NULL;							// ���_�o�b�t�@
	m_nTexIdx = 0;								// �e�N�X�`���̃C���f�b�N�X�ԍ�

	for (int nCntVtx = 0; nCntVtx < 32; nCntVtx++)
	{
		m_fTex[nCntVtx] = D3DXVECTOR2(0.0f, 0.0f);			// �e�N�X�`�����W
		m_VtxPos[nCntVtx] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���_���W
	}

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject2D::~CObject2D()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObject2D::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_nTexIdx = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CObject2D *CObject2D::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CObject2D *pObject2D = NULL;

	if (pObject2D == NULL)
	{// NULL��������

		// �������̊m��
		pObject2D = DEBUG_NEW CObject2D;

		if (pObject2D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObject2D->Init();
		}

		return pObject2D;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CObject2D *CObject2D::Create(int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CObject2D *pObject2D = NULL;

	if (pObject2D == NULL)
	{// NULL��������

		// �������̊m��
		pObject2D = DEBUG_NEW CObject2D(nPriority);

		if (pObject2D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObject2D->Init();
		}

		return pObject2D;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CObject2D *CObject2D::Create(int nPriority, int nNumVtx)
{
	// �����p�̃I�u�W�F�N�g
	CObject2D *pObject2D = NULL;

	if (pObject2D == NULL)
	{// NULL��������

		// �������̊m��
		pObject2D = DEBUG_NEW CObject2D(nPriority);

		if (pObject2D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObject2D->Init(nNumVtx);
		}

		return pObject2D;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject2D::Init(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_fTex[0] = D3DXVECTOR2(0.0f, 0.0f);	// �e�N�X�`�����W
	m_fTex[1] = D3DXVECTOR2(1.0f, 0.0f);	// �e�N�X�`�����W
	m_fTex[2] = D3DXVECTOR2(0.0f, 1.0f);	// �e�N�X�`�����W
	m_fTex[3] = D3DXVECTOR2(1.0f, 1.0f);	// �e�N�X�`�����W

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != NULL)
	{// ���ɏ�񂪓����Ă�ꍇ

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
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
// ����������
//==========================================================================
HRESULT CObject2D::Init(int nNumVertex)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_fTex[0] = D3DXVECTOR2(0.0f, 0.0f);	// �e�N�X�`�����W
	m_fTex[1] = D3DXVECTOR2(1.0f, 0.0f);	// �e�N�X�`�����W
	m_fTex[2] = D3DXVECTOR2(0.0f, 1.0f);	// �e�N�X�`�����W
	m_fTex[3] = D3DXVECTOR2(1.0f, 1.0f);	// �e�N�X�`�����W

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != NULL)
	{// ���ɏ�񂪓����Ă�ꍇ

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	// ���_���ݒ�
	SetVtx(nNumVertex);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObject2D::Draw(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

#if _DEBUG
	if (CManager::GetInstance()->GetPause()->IsPause() == false)
	{// �|�[�Y��

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(m_nTexIdx));

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
#else

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObject2D::Draw(int nNumVertex)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject2D::SetVtx(void)
{

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXCOLOR col = GetColor();
	float fAngle = GetAngle();
	float fLength = GetLength();

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z - D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = m_fTex[0];
	pVtx[1].tex = m_fTex[1];
	pVtx[2].tex = m_fTex[2];
	pVtx[3].tex = m_fTex[3];

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void  CObject2D::SetVtx(int nNumVertex)
{
	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXCOLOR col = GetColor();
	D3DXVECTOR3 *pVtxPos = GetVtxPos();

	// ���_���W�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = pos.x + pVtxPos[nCntVtx].x;
		pVtx[nCntVtx].pos.y = pos.y + pVtxPos[nCntVtx].y;
		pVtx[nCntVtx].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[nCntVtx].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CObject2D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CObject2D::GetColor(void) const
{
	return m_col;
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CObject2D::SetSize(const D3DXVECTOR2 size)
{
	m_size = size;		// �T�C�Y
	SetLength(sqrtf(m_size.x * m_size.x + m_size.y * m_size.y));	// �Ίp���̒���
	SetAngle(atan2f(m_size.x, m_size.y));								// �Ίp���̌���
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CObject2D::GetSize(void) const
{
	return m_size;
}

//==========================================================================
// ���̃T�C�Y�̐ݒ�
//==========================================================================
void CObject2D::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_sizeOrigin = size;
}

//==========================================================================
// ���̃T�C�Y�̎擾
//==========================================================================
D3DXVECTOR2 CObject2D::GetSizeOrigin(void) const
{
	return m_sizeOrigin;
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CObject2D::SetTex(D3DXVECTOR2 *tex)
{
	memcpy(&m_fTex[0], tex, sizeof(m_fTex));
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CObject2D::GetTex(void)
{
	return &m_fTex[0];
}

//==========================================================================
// �Ίp���̒����ݒ�
//==========================================================================
void CObject2D::SetLength(const float fLength)
{
	m_fLength = fLength;
}

//==========================================================================
// �Ίp���̒����擾
//==========================================================================
float CObject2D::GetLength(void) const
{
	return m_fLength;
}

//==========================================================================
// �Ίp���̌����擾
//==========================================================================
void CObject2D::SetAngle(const float fAngle)
{
	m_fAngle = fAngle;
}

//==========================================================================
// �Ίp���̌����擾
//==========================================================================
float CObject2D::GetAngle(void) const
{
	return m_fAngle;
}


//==========================================================================
// ���_���W�擾
//==========================================================================
D3DXVECTOR3 *CObject2D::GetVtxPos(void)
{
	return &m_VtxPos[0];
}

//==========================================================================
// ���_���W�ݒ�
//==========================================================================
void CObject2D::SetVtxPos(D3DXVECTOR3 *pos)
{
	//&m_VtxPos[0] = *pos;
	memcpy(&m_VtxPos[0], pos, sizeof(m_VtxPos));
}

//==========================================================================
// ���_�o�b�t�@�̎擾
//==========================================================================
LPDIRECT3DVERTEXBUFFER9 CObject2D::GetVtxBuff(void) const
{
	return m_pVtxBuff;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CObject2D::GetObject2D(void)
{
	return this;
}
