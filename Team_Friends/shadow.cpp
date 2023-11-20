//=============================================================================
// 
//  �e���� [shadow.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "elevation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define POS_SHADOW	(50.0f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CShadow::m_nNumAll = 0;						// �e�̑���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{

	m_pObject = NULL;	// �I�u�W�F�N�g�̃|�C���^
	m_nNumAll = 0;		// ����
	m_nTexIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CShadow::~CShadow()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CShadow *CShadow::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CShadow *pShadow = NULL;

	if (pShadow == NULL)
	{// NULL��������

		// �������̊m��
		pShadow = DEBUG_NEW CShadow;

		//if (pShadow->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pShadow;
		//	return NULL;
		//}

		if (pShadow != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pShadow->Init();
		}

		return pShadow;
	}

	return NULL;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, float size)
{
	// �����p�̃I�u�W�F�N�g
	CShadow *pShadow = NULL;

	if (pShadow == NULL)
	{// NULL��������

		// �������̊m��
		pShadow = DEBUG_NEW CShadow;

		//if (pShadow->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pShadow;
		//	return NULL;
		//}

		if (pShadow != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pShadow->SetPosition(pos);
			pShadow->SetSize(D3DXVECTOR3(size, 0.0f, size));	// �T�C�Y

			// ����������
			pShadow->Init();
		}
		return pShadow;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CShadow::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));	// �F

	// ��ނ̐ݒ�
	SetType(TYPE_SHADOW);

	// ����������
	hr = CObject3D::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg");

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CShadow::Uninit(void)
{
	// �I������
	CObject3D::Uninit();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CShadow::Update(void)
{
	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �����擾
	bool bLand = false;
	pos.y = CGame::GetElevation()->GetHeight(pos, bLand);

	// �ʒu�ݒ�
	SetPosition(pos);

	// ���_���ݒ�
	CObject3D::SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CShadow::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0); 

	// �`�揈��
	CObject3D::Draw();

	// ���Z���������Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// �����擾
//==========================================================================
int CShadow::GetNumAll(void)
{
	return m_nNumAll;
}
