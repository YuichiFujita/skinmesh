//=============================================================================
// 
//  ���b�V���V�����_�[���� [impactwave.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "impactwave.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE		"data\\TEXTURE\\field002.png"
#define POS_IMPACTWAVE	(5000.0f)
#define POS_IMPACTWAVE_Y	(3000.0f)
#define WIDTH	(32)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CImpactWave::m_apFilename[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\GRADATION\\black_01.jpg",
	"data\\TEXTURE\\GRADATION\\black_02.jpg",
	"data\\TEXTURE\\GRADATION\\orange_01.jpg",
	"data\\TEXTURE\\GRADATION\\orange_02.jpg",
	"data\\TEXTURE\\GRADATION\\blue_01.jpg",
	"data\\TEXTURE\\GRADATION\\blue_02.jpg",
	"data\\TEXTURE\\GRADATION\\green_01.jpg",
	"data\\TEXTURE\\GRADATION\\green_02.jpg",
	"data\\TEXTURE\\GRADATION\\purple_01.jpg",
	"data\\TEXTURE\\GRADATION\\purple_02.jpg",
	"data\\TEXTURE\\GRADATION\\purple_03.jpg",
	"data\\TEXTURE\\GRADATION\\purple_04.jpg",
	"data\\TEXTURE\\GRADATION\\pink_01.jpg",
	"data\\TEXTURE\\effect\\effect003.tga",
	"data\\TEXTURE\\GRADATION\\giza_01.png",
	"data\\TEXTURE\\GRADATION\\giza_02.png",
	"data\\TEXTURE\\GRADATION\\giza_03.png",
	"data\\TEXTURE\\GRADATION\\giza_06.png",
	"data\\TEXTURE\\GRADATION\\giza_05.png",
	"data\\TEXTURE\\GRADATION\\wave_01.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CImpactWave::CImpactWave(int nPriority) : CObject3DMesh(nPriority)
{
	m_nTexIdx = 0;										// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nLife = 0;										// ����
	m_nMaxLife = 0;										// �ő����
	m_nTexDivision = 0;									// �e�N�X�`������
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���̐F
	m_fOutWidth = 0.0f;		// ����(�O)
	m_fInWidth = 0.0f;		// ��(��)
	m_fRotWidth = 0.0f;		// 1������������̊p�x����
	m_fHeight = 0.0f;		// ����
	m_fMove = 0.0f;			// �L���鑬�x
	m_bAddBlend = false;	// ���Z�����̔���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CImpactWave::~CImpactWave()
{

}

//==========================================================================
// ��������
//==========================================================================
CImpactWave *CImpactWave::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fCenterDistance, int nLife, float fMove, int nTexType, bool bAddBlend)
{
	// �����p�̃I�u�W�F�N�g
	CImpactWave *pObjMeshCylinder = NULL;

	if (pObjMeshCylinder == NULL)
	{// NULL��������

		// �������̊m��
		pObjMeshCylinder = DEBUG_NEW CImpactWave;

		if (pObjMeshCylinder != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu�E����
			pObjMeshCylinder->SetPosition(pos);
			pObjMeshCylinder->SetRotation(rot);
			pObjMeshCylinder->SetColor(col);
			pObjMeshCylinder->m_colOrigin = col;
			pObjMeshCylinder->SetWidthBlock(WIDTH);
			pObjMeshCylinder->SetHeightBlock(1);
			pObjMeshCylinder->SetWidthLen(POS_IMPACTWAVE);
			pObjMeshCylinder->SetHeightLen(POS_IMPACTWAVE_Y);

			pObjMeshCylinder->m_fOutWidth = fWidth + fCenterDistance;		// ����(�O)
			pObjMeshCylinder->m_fInWidth = fWidth;		// ��(��)
			pObjMeshCylinder->m_fHeight = fHeight;		// ����
			pObjMeshCylinder->m_nLife = nLife;			// ����
			pObjMeshCylinder->m_fMove = fMove;			// �L���鑬�x
			pObjMeshCylinder->m_bAddBlend = bAddBlend;	// ���Z�����̔���

			// �e�N�X�`���̊��蓖��
			pObjMeshCylinder->m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(m_apFilename[nTexType]);

			// �e�N�X�`���̊��蓖��
			pObjMeshCylinder->BindTexture(pObjMeshCylinder->m_nTexIdx);

			// ����������
			pObjMeshCylinder->Init();
		}

		return pObjMeshCylinder;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CImpactWave::Init(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ��ސݒ�
	SetType(TYPE_MESHDONUTS);

	m_fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1������������̊p�x����
	m_nMaxLife = m_nLife;	// �ő����
	m_nTexDivision = 4;		// �e�N�X�`������

	// �I�u�W�F�N�g3D���b�V���̏���������
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_DONUTS);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	D3DXVECTOR3 *pVtxPos = GetVtxPos();	// ���_���W�擾
	D3DXVECTOR2 *pVtxTex = GetVtxTex();	// �e�N�X�`�����W�擾

	// �������擾
	int nHeightBlock = GetHeightBlock();
	int nWidthBlock = GetWidthBlock();

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < nHeightBlock + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < nWidthBlock + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			float fLength = (m_fOutWidth - nCntHeight * m_fInWidth);
			if (fLength <= 0.0f)
			{
				fLength = 0.0f;
			}

			// ���_���W�̐ݒ�
			pVtxPos[nCntWidth + (nCntHeight * (nWidthBlock + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength,
				nCntHeight * m_fHeight,
				cosf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength
			);

			pVtxTex[nCntWidth + (nCntHeight * (nWidthBlock + 1))] = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(nWidthBlock / m_nTexDivision)),
				nCntHeight * (1.0f / (float)(nHeightBlock))
			);
		}
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CImpactWave::Uninit(void)
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CImpactWave::Update(void)
{
	// �F�擾
	D3DXCOLOR col = GetColor();

	// �L���Ă���
	m_fOutWidth += m_fMove;

	// �����X�V
	m_nLife--;

	// �s�����x�X�V
	col.a = m_colOrigin.a * (float)m_nLife / (float)m_nMaxLife;

	// �F�ݒ�
	SetColor(col);

	// ���_���ݒ�
	SetVtx();
	
	if (m_nLife <= 0)
	{// �������s����

		// �I������
		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CImpactWave::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();


	// �w�ʂ̃J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���u�����f�B���O�����Z�����ɐݒ�
	if (m_bAddBlend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObject3DMesh::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �J�����O�̃f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CImpactWave::SetVtx(void)
{
	D3DXVECTOR3 *pVtxPos = GetVtxPos();	// ���_���W�擾
	D3DXVECTOR2 *pVtxTex = GetVtxTex();	// �e�N�X�`�����W�擾

	// �������擾
	int nHeightBlock = GetHeightBlock();
	int nWidthBlock = GetWidthBlock();

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < nHeightBlock + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < nWidthBlock + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			float fLength = (m_fOutWidth - nCntHeight * m_fInWidth);
			if (fLength <= 0.0f)
			{
				fLength = 0.0f;
			}

			// ���_���W�̐ݒ�
			pVtxPos[nCntWidth + (nCntHeight * (nWidthBlock + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength,
				nCntHeight * m_fHeight,
				cosf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength
			);
		}
	}

	// ���_���W�ݒ�
	SetVtxPos(pVtxPos);

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CImpactWave::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//==========================================================================
// �����擾
//==========================================================================
int CImpactWave::GetLife(void) const
{
	return m_nLife;
}

//==========================================================================
// �ő�����ݒ�
//==========================================================================
void CImpactWave::SetMaxLife(const int nLife)
{
	m_nMaxLife = nLife;
}

//==========================================================================
// �ő�����擾
//==========================================================================
int CImpactWave::GetMaxLife(void) const
{
	return m_nMaxLife;
}

//==========================================================================
// ���̐F�ݒ�
//==========================================================================
void CImpactWave::SetColOrigin(const D3DXCOLOR col)
{
	m_colOrigin = col;
}

//==========================================================================
// ���̐F�擾
//==========================================================================
D3DXCOLOR CImpactWave::GetColOrigin(void) const
{
	return m_colOrigin;
}

//==========================================================================
// �O���ݒ�
//==========================================================================
void CImpactWave::SetOutWidth(const float fOutWidth)
{
	m_fOutWidth = fOutWidth;
}

//==========================================================================
// �O���擾
//==========================================================================
float CImpactWave::GetOutWidth(void) const
{
	return m_fOutWidth;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CImpactWave::SetInWidth(const float fInWidth)
{
	m_fInWidth = fInWidth;
}

//==========================================================================
// �����擾
//==========================================================================
float CImpactWave::GetInWidth(void) const
{
	return m_fInWidth;
}

//==========================================================================
// �ꕪ��������̊p�x�ݒ�
//==========================================================================
void CImpactWave::SetRotWidth(const float fRot)
{
	m_fRotWidth = fRot;
}

//==========================================================================
// �ꕪ��������̊p�x�擾
//==========================================================================
float CImpactWave::GetRotWidth(void) const
{
	return m_fRotWidth;
}

//==========================================================================
// �T�C�Y�̈ړ��ʐݒ�
//==========================================================================
void CImpactWave::SetMoveSize(const float fMoveSize)
{
	m_fMove = fMoveSize;
}

//==========================================================================
// �T�C�Y�̈ړ��ʎ擾
//==========================================================================
float CImpactWave::GetMoveSize(void) const
{
	return m_fMove;
}

//==========================================================================
// �e�N�X�`���������ݒ�
//==========================================================================
void CImpactWave::SetTexDivision(const int nDivision)
{
	m_nTexDivision = nDivision;
}

//==========================================================================
// �e�N�X�`���������擾
//==========================================================================
int CImpactWave::GetTexDivision(void) const
{
	return m_nTexDivision;
}

//==========================================================================
// ���Z�����̐ݒ�
//==========================================================================
void CImpactWave::SetIsAddBlend(bool bAddBlend)
{
	m_bAddBlend = bAddBlend;
}

//==========================================================================
// �t�@�C�����擾
//==========================================================================
const char *CImpactWave::GetFileName(int nTexType)
{
	return m_apFilename[nTexType];
}

//==========================================================================
// ���b�V���V�����_�[�I�u�W�F�N�g�̎擾
//==========================================================================
CImpactWave *CImpactWave::GetMyObject(void)
{
	return this;
}
