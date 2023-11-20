//=============================================================================
// 
//  �t�F�[�h���� [fade.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define ALPHAMOVE	(0.025f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CFade::CFade()
{
	// �l�̃N���A
	m_aObject2D = NULL;					// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	m_ModeNext = CScene::MODE_TITLE;	// ���̃��[�h
	m_state = STATE_NONE;				// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CFade::~CFade()
{

}

//==========================================================================
// ��������
//==========================================================================
CFade *CFade::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CFade *pFade = NULL;

	if (pFade == NULL)
	{// NULL��������

		// �������̊m��
		pFade = DEBUG_NEW CFade;

		if (pFade != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			if (FAILED(pFade->Init()))
			{// ���s���Ă�����
				return NULL;
			}
		}
		else
		{
			delete pFade;
			pFade = NULL;
		}

		return pFade;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CFade::Init(void)
{
	// ��������
	m_aObject2D = CObject2D::Create(8);

	if (m_aObject2D == NULL)
	{// ���s���Ă�����
		return E_FAIL;
	}

	m_state = STATE_FADEIN;			// ���
	
#if _DEBUG
	m_ModeNext = CScene::MODE_GAME;	// ���̃��[�h
	m_ModeNext = CScene::MODE_TITLE;	// ���̃��[�h
#else
	m_ModeNext = CScene::MODE_TITLE;	// ���̃��[�h
#endif
	
	m_aObject2D->SetSize(D3DXVECTOR2(640.0f, 360.0f));	// �T�C�Y
	m_aObject2D->SetPosition(D3DXVECTOR3(640.0f, 360.0f, 0.0f));	// �ʒu
	m_aObject2D->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F�ݒ�

	// �ŏ��������̉�ʂ�
	CManager::GetInstance()->SetMode(m_ModeNext);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CFade::Uninit(void)
{
	if (m_aObject2D != NULL)
	{// NULL����Ȃ�������

		// �I������
		m_aObject2D->Uninit();
		delete m_aObject2D;
		m_aObject2D = NULL;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CFade::Update(void)
{
	// �F�擾
	D3DXCOLOR col = m_aObject2D->GetColor();

	switch (m_state)
	{
	case STATE_NONE:
		break;

	case STATE_FADEOUT:

		// �s�����x����
		col.a += ALPHAMOVE;

		if (col.a >= 1.0f)
		{
			col.a = 1.0f;
			m_state = STATE_FADEIN;

			// ���̉�ʂ�
			CManager::GetInstance()->SetMode(m_ModeNext);
		}
		break;

	case STATE_FADEIN:

		// �s�����x����
		col.a -= ALPHAMOVE;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;
			m_state = STATE_NONE;
		}
		break;
	}

	// �F�ݒ�
	m_aObject2D->SetColor(col);

	// �X�V����
	m_aObject2D->Update();

	m_aObject2D->SetVtx();

}

//==========================================================================
// �`�揈��
//==========================================================================
void CFade::Draw(void)
{
	// �`�揈��
	m_aObject2D->Draw();
}

//==========================================================================
// ���̃��[�h�ݒ�
//==========================================================================
void CFade::SetFade(CScene::MODE mode)
{
	if (m_state == STATE_NONE)
	{// �������Ă��Ȃ��Ƃ�

		// ���̃��[�h�ݒ�
		m_ModeNext = mode;
		m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g��Ԃɐݒ�

		// �F�ݒ�
		m_aObject2D->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CFade::STATE CFade::GetState(void)
{
	return m_state;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CFade::GetMyObject(void)
{
	return m_aObject2D;
}