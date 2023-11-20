//=============================================================================
// 
//  ����(�r���{�[�h)���� [number_Billboard.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "number_Billboard.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WIDTH			(640.0f)					// ����
#define HEIGHT			(360.0f)					// �c��
#define SCROLL_SPEED	(-0.005f)					// �X�N���[�����x

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CNumberBillboard::CNumberBillboard(int nPriority) : CNumber(nPriority)
{
	// �l�̃N���A
	m_pObjBillboard = NULL;		// �I�u�W�F�N�g�r���{�[�h�̃I�u�W�F�N�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CNumberBillboard::~CNumberBillboard()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CNumberBillboard::Init(int nPriority)
{
	// ��������
	m_pObjBillboard = CObjectBillboard::Create();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CNumberBillboard::Uninit(void)
{
	// �I������
	if (m_pObjBillboard != NULL)
	{// NULL����Ȃ�������
		m_pObjBillboard = NULL;
	}
}

//==========================================================================
// �������
//==========================================================================
void CNumberBillboard::Release(void)
{
	if (m_pObjBillboard != NULL)
	{// NULL����Ȃ�������
		m_pObjBillboard->Uninit();
		m_pObjBillboard = NULL;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CNumberBillboard::Update(void)
{
	// �X�V����
	if (m_pObjBillboard != NULL)
	{
		m_pObjBillboard->Update();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CNumberBillboard::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	if (m_pObjBillboard != NULL)
	{// NULL����Ȃ�������
		m_pObjBillboard->Draw();
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CNumberBillboard::SetVtx(void)
{
	m_pObjBillboard->SetVtx();
}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CNumberBillboard::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_pObjBillboard->BindTexture(nIdx);
}

//==========================================================================
// ��ސݒ�
//==========================================================================
void CNumberBillboard::SetType(const CObject::TYPE type)
{
	m_pObjBillboard->SetType(type);
}


//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CNumberBillboard::SetPosition(const D3DXVECTOR3 pos)
{
	m_pObjBillboard->SetPosition(pos);
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CNumberBillboard::GetPosition(void) const
{
	return m_pObjBillboard->GetPosition();
}

//==========================================================================
// �ړ��ʐݒ�
//==========================================================================
void CNumberBillboard::SetMove(const D3DXVECTOR3 move)
{
	m_pObjBillboard->SetMove(move);
}

//==========================================================================
// �ړ��ʎ擾
//==========================================================================
D3DXVECTOR3 CNumberBillboard::GetMove(void) const
{
	return m_pObjBillboard->GetMove();
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CNumberBillboard::SetColor(const D3DXCOLOR col)
{
	m_pObjBillboard->SetColor(col);
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CNumberBillboard::GetColor(void) const
{
	return m_pObjBillboard->GetColor();
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CNumberBillboard::SetSize(const D3DXVECTOR2 size)
{
	m_pObjBillboard->SetSize(size);		// �T�C�Y
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CNumberBillboard::GetSize(void) const
{
	return m_pObjBillboard->GetSize();
}

//==========================================================================
// ���̃T�C�Y�̐ݒ�
//==========================================================================
void CNumberBillboard::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_pObjBillboard->SetSizeOrigin(size);
}

//==========================================================================
// ���̃T�C�Y�̎擾
//==========================================================================
D3DXVECTOR2 CNumberBillboard::GetSizeOrigin(void) const
{
	return m_pObjBillboard->GetSizeOrigin();
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CNumberBillboard::SetTex(D3DXVECTOR2 *tex)
{
	m_pObjBillboard->SetTex(tex);
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CNumberBillboard::GetTex(void)
{
	return m_pObjBillboard->GetTex();
}

//==========================================================================
// �I�u�W�F�N�g�r���{�[�h�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectBillboard *CNumberBillboard::GetObjectBillboard(void)
{
	return m_pObjBillboard;
}