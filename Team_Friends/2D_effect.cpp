//=============================================================================
// 
// �G�t�F�N�g���� [2D_effect.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "2D_effect.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define DEF_RADIUS			(20.0f)
#define EFFECT_2D_LIFE		(30)
#define EFFECT_2DSIZE1		(0.97f)
#define EFFECT_2DSIZE2		(0.98f)
#define EFFECT_2DSIZE3		(0.99f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CEffect2D::m_apTextureFile[] =		// �t�@�C���ǂݍ���
{
	"data\\TEXTURE\\effect\\effect000.jpg",	   // �ʏ�G�t�F�N�g
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // ���G�t�F�N�g
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // ����
	"data\\TEXTURE\\effect\\effect000.png",	   // ���G�t�F�N�g
	"data\\TEXTURE\\effect\\effect001.png",	   // �\���G�t�F�N�g
	"data\\TEXTURE\\effect\\Star01.png",	   // �\���G�t�F�N�g
};
int CEffect2D::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEffect2D::CEffect2D(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F�̌�
	m_fRadius = 0.0f;							// ���a
	m_fMaxRadius = 0.0f;						// �ő唼�a
	m_nLife = 0;								// ����
	m_nMaxLife = 0;								// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_bAddAlpha = true;							// ���Z�����̔���
	
	// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{
		m_nTexIdx[nCntTex] = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	}

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEffect2D::~CEffect2D()
{
	// �������Z
	m_nNumAll--;
}

//==========================================================================
// ��������
//==========================================================================
CEffect2D *CEffect2D::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CEffect2D *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect2D;

		//if (pEffect->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pEffect;
		//	return NULL;
		//}

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init();
		}

		return pEffect;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CEffect2D *CEffect2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	// �����p�̃I�u�W�F�N�g
	CEffect2D *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect2D;

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init(pos, move, col, fRadius, nLife, moveType, type);
		}

		return pEffect;
	}

	return NULL;
}


//==========================================================================
// ��������
//==========================================================================
CEffect2D *CEffect2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 rot, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	// �����p�̃I�u�W�F�N�g
	CEffect2D *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect2D;

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init(pos, move, col, fRadius, nLife, moveType, type);
			pEffect->SetRotation(rot);
		}

		return pEffect;
	}

	return NULL;
}

//==================================================================================
// �G�t�F�N�g�̏���������
//==================================================================================
HRESULT CEffect2D::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F�̌�
	m_fRadius = DEF_RADIUS;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	m_nLife = EFFECT_2D_LIFE;					// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_bAddAlpha = true;							// ���Z�����̔���

	// ��ނ̐ݒ�
	SetType(TYPE_EFFECT2D);

	// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{
		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntTex] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntTex]);

		// �e�N�X�`���̊��蓖��
		BindTexture(m_nTexIdx[nCntTex]);
	}

	// ����������
	hr = CObject2D::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	return S_OK;
}

//==================================================================================
// �G�t�F�N�g�̏���������
//==================================================================================
HRESULT CEffect2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	SetPosition(pos);
	SetMove(move);
	m_colOrigin = col;							// �F�̌�
	SetColor(col);								// �F
	m_fRadius = fRadius;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));	// �T�C�Y�ݒ�
	m_nLife = nLife;							// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = moveType;						// �ړ��̎��
	m_nType = type;								// ���

	// ���Z�����̔���
	switch (m_nType)
	{
	case TYPE_NORMAL:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKE:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKEBLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_BLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_JUJI:
		m_bAddAlpha = true;
		break;

	case TYPE_JUJI2:
		m_bAddAlpha = true;
		break;

	default:
		m_bAddAlpha = true;
		break;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_EFFECT2D);

	// �e�N�X�`���̊��蓖��
	m_nTexIdx[m_nType] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_nType]);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx[m_nType]);

	// ����������
	hr = CObject2D::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	return S_OK;
}

//==================================================================================
// �G�t�F�N�g�̏I������
//==================================================================================
void CEffect2D::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//==================================================================================
// �G�t�F�N�g�̍X�V����
//==================================================================================
void CEffect2D::Update(void)
{

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �F�擾
	D3DXCOLOR col = GetColor();

	// �ʒu�X�V
	pos += move;

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// ���������Ă��������̏ꍇ
	if (m_moveType == MOVEEFFECT_SUB)
	{// �G�t�F�N�g�����������Ă���

		// �k������
		SubSize();
	}
	else if (m_moveType == MOVEEFFECT_SUPERSUB)
	{// �G�t�F�N�g�����������Ă���

		// ���k������
		SuperSubSize();
	}
	else if (m_moveType == MOVEEFFECT_ADD)
	{// �G�t�F�N�g��傫�����Ă���

		// �g�又��
		AddSize();
	}
	else if (m_moveType == MOVEEFFECT_GENSUI)
	{
		Gensui();
	}

	// �����̍X�V
	m_nLife--;

	// �s�����x�̍X�V
	col.a = m_colOrigin.a * ((float)m_nLife / (float)m_nMaxLife);

	// �T�C�Y�ݒ�
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

	// �F�ݒ�
	SetColor(col);

	if (m_nLife <= 0)
	{// �������s������

		// �G�t�F�N�g�̍폜
		Uninit();
		return;
	}

	// ���_���W�̐ݒ�
	SetVtx();

}

//==================================================================================
// �G�t�F�N�g�̏k������
//==================================================================================
void CEffect2D::SubSize(void)
{
	int nEffect_2DType = rand() % 3;

	if (nEffect_2DType == 0)
	{
		m_fRadius *= EFFECT_2DSIZE1;
	}
	else if (nEffect_2DType == 1)
	{
		m_fRadius *= EFFECT_2DSIZE2;
	}
	else if (nEffect_2DType == 2)
	{
		m_fRadius *= EFFECT_2DSIZE3;
	}
}

//==================================================================================
// �G�t�F�N�g�̏k������
//==================================================================================
void CEffect2D::SuperSubSize(void)
{
	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;
}

//==================================================================================
// �G�t�F�N�g�̊g�又��
//==================================================================================
void CEffect2D::AddSize(void)
{
	int nEffect_2DType = rand() % 3;

	if (nEffect_2DType == 0)
	{
		m_fRadius += 1.8f;
	}
	else if (nEffect_2DType == 1)
	{
		m_fRadius += 0.8f;
	}
	else if (nEffect_2DType == 2)
	{
		m_fRadius += 0.0f;
	}
}

//==================================================================================
// �G�t�F�N�g�̌�������
//==================================================================================
void CEffect2D::Gensui(void)
{
	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	move.x += (0.0f - move.x) * 0.15f;
	move.y += (0.0f - move.y) * 0.15f;

	// �ړ��ʐݒ�
	SetMove(move);

	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;

}

//==================================================================================
// �G�t�F�N�g�̕`�揈��
//==================================================================================
void CEffect2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	if (m_bAddAlpha == true)
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
	CObject2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CEffect2D::SetVtx(void)
{
	// ���_�ݒ�
	CObject2D::SetVtx();
}

//==========================================================================
// �����擾
//==========================================================================
int CEffect2D::GetNumAll(void)
{
	return m_nNumAll;
}
