//=============================================================================
// 
// �X�e�[�^�X�E�B���h�E�̉��n���� [statuswindow_base.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "statuswindow_base.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStatusWindow_Base::CStatusWindow_Base(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStatusWindow_Base::~CStatusWindow_Base()
{

}

//==========================================================================
// ��������
//==========================================================================
CStatusWindow_Base *CStatusWindow_Base::Create(const D3DXVECTOR3 pos)
{
	// �����p�̃I�u�W�F�N�g
	CStatusWindow_Base *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CStatusWindow_Base;

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init();
			pEffect->SetPosition(pos);
		}

		return pEffect;
	}

	return NULL;
}

//==================================================================================
// ����������
//==================================================================================
HRESULT CStatusWindow_Base::Init(void)
{
	HRESULT hr;

	// ����������
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̊��蓖��
	int nIdxTex = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\statuswindow\\statuswindow_base02.png");

	// �e�N�X�`���̊��蓖��
	BindTexture(nIdxTex);

	// �T�C�Y�擾
	D3DXVECTOR2 size = CManager::GetInstance()->GetTexture()->GetImageSize(nIdxTex) * 0.25f;
	size *= 0.5f;

	// �T�C�Y�ݒ�
	SetSize(size);

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	return S_OK;
}

//==================================================================================
// �I������
//==================================================================================
void CStatusWindow_Base::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//==================================================================================
// �X�V����
//==================================================================================
void CStatusWindow_Base::Update(void)
{
	// ���_���W�̐ݒ�
	SetVtx();
}

//==================================================================================
// �`�揈��
//==================================================================================
void CStatusWindow_Base::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObject2D::Draw();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

