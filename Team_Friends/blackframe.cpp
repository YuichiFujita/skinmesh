//=============================================================================
// 
//  ���t���[������ [blackframe.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "blackframe.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define SIZE		(D3DXVECTOR2(640.0f, 50.0f))
#define START_UP	(D3DXVECTOR3(640.0f, -SIZE.y, 0.0f))
#define START_DOWN	(D3DXVECTOR3(640.0f, SCREEN_HEIGHT + SIZE.y, 0.0f))
#define MOVETIME	(50)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const D3DXVECTOR3 CBlackFrame::m_DestPosition[VTX_MAX] =	// �ڕW�̈ʒu
{
	D3DXVECTOR3(640.0f, SIZE.y, 0.0f),
	D3DXVECTOR3(640.0f, SCREEN_HEIGHT - SIZE.y, 0.0f),
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBlackFrame::CBlackFrame(int nPriority)
{
	// �l�̃N���A
	memset(&m_pObj2D[0], NULL, sizeof(m_pObj2D));	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	m_state = STATE_NONE;	// ���
	m_nCntMove = 0;	// �ړ��J�E���g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBlackFrame::~CBlackFrame()
{

}

//==========================================================================
// ��������
//==========================================================================
CBlackFrame *CBlackFrame::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CBlackFrame *pTitleScreen = NULL;

	if (pTitleScreen == NULL)
	{// NULL��������

		// �������̊m��
		pTitleScreen = DEBUG_NEW CBlackFrame;

		if (pTitleScreen != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pTitleScreen->Init();
		}

		return pTitleScreen;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBlackFrame::Init(void)
{
	
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		// ��������
		m_pObj2D[nCntSelect] = CObject2D::Create(8);

		// ��ނ̐ݒ�
		//m_pObj2D[nCntSelect]->SetType(CObject::TYPE_OBJECT2D);

		// �e�N�X�`���̊��蓖��
		m_pObj2D[nCntSelect]->BindTexture(0);

		// �e��ϐ��̏�����
		switch (nCntSelect)
		{
		case VTX_UP:
			// �T�C�Y�擾
			m_pObj2D[nCntSelect]->SetSize(SIZE);	// �T�C�Y
			m_pObj2D[nCntSelect]->SetPosition(START_UP);	// �ʒu
			m_pObj2D[nCntSelect]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F
			break;

		case VTX_DOWN:
			// �T�C�Y�擾
			m_pObj2D[nCntSelect]->SetSize(SIZE);	// �T�C�Y
			m_pObj2D[nCntSelect]->SetPosition(START_DOWN);	// �ʒu
			m_pObj2D[nCntSelect]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F
			break;
		}
	}
	m_state = STATE_OUTCOMPLETION;	// ���

	return S_OK;
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CBlackFrame::Reset(void)
{
	m_state = STATE_OUTCOMPLETION;
	m_nCntMove = 0;

	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_pObj2D[nCntVtx] != NULL)
		{// NULL����Ȃ�������

			D3DXVECTOR3 pos = mylib_const::DEFAULT_VECTOR3;
			if (nCntVtx == VTX_UP)
			{
				pos = START_UP;
			}
			else if (nCntVtx == VTX_DOWN)
			{
				pos = START_DOWN;
			}

			// ���ݒ�
			m_pObj2D[nCntVtx]->SetPosition(pos);
		}
	}

}

//==========================================================================
// �I������
//==========================================================================
void CBlackFrame::Uninit(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_pObj2D[nCntSelect]->Uninit();
			delete m_pObj2D[nCntSelect];
			m_pObj2D[nCntSelect] = NULL;
		}
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CBlackFrame::Update(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] == NULL)
		{// NULL��������
			continue;
		}

		// ��ԍX�V����
		UpdateState(nCntSelect);

		// ���_���ݒ�
		m_pObj2D[nCntSelect]->SetVtx();
	}

	// �ړ��J�E���g���Z
	m_nCntMove++;
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CBlackFrame::UpdateState(int nCntVtx)
{
	switch (m_state)
	{
	case CBlackFrame::STATE_NONE:
		break;

	case CBlackFrame::STATE_IN:
		StateIn(nCntVtx);
		break;

	case CBlackFrame::STATE_OUT:
		StateOut(nCntVtx);
		break;

	case CBlackFrame::STATE_INCOMPLETION:
		break;

	case CBlackFrame::STATE_OUTCOMPLETION:
		break;
	}
}

//==========================================================================
// �N��
//==========================================================================
void CBlackFrame::StateIn(int nCntVtx)
{
	if (m_nCntMove > MOVETIME)
	{// �ړ����Ԃ��K��l��������
		m_nCntMove = MOVETIME;
		m_state = STATE_INCOMPLETION;
		return;
	}

	// ���擾
	D3DXVECTOR3 pos = m_pObj2D[nCntVtx]->GetPosition();

	// �����ʒu
	D3DXVECTOR3 start = mylib_const::DEFAULT_VECTOR3;
	if (nCntVtx == VTX_UP)
	{
		start = START_UP;
	}
	else if (nCntVtx == VTX_DOWN)
	{
		start = START_DOWN;
	}

	// �^�C������
	float fTime = (float)m_nCntMove / (float)MOVETIME;

	// �ʒu�X�V
	pos.x = Lerp(start.x, m_DestPosition[nCntVtx].x, fTime);
	pos.y = Lerp(start.y, m_DestPosition[nCntVtx].y, fTime);

	// ���ݒ�
	m_pObj2D[nCntVtx]->SetPosition(pos);
}

//==========================================================================
// �J��
//==========================================================================
void CBlackFrame::StateOut(int nCntVtx)
{
	if (m_nCntMove > MOVETIME)
	{// �ړ����Ԃ��K��l��������
		m_nCntMove = MOVETIME;
		m_state = STATE_OUTCOMPLETION;
		return;
	}

	// ���擾
	D3DXVECTOR3 pos = m_pObj2D[nCntVtx]->GetPosition();

	// �����ʒu
	D3DXVECTOR3 start = mylib_const::DEFAULT_VECTOR3;
	if (nCntVtx == VTX_UP)
	{
		start = START_UP;
	}
	else if (nCntVtx == VTX_DOWN)
	{
		start = START_DOWN;
	}

	// �^�C������
	float fTime = (float)m_nCntMove / (float)MOVETIME;

	// �ʒu�X�V
	pos.x = Lerp(m_DestPosition[nCntVtx].x, start.x, fTime);
	pos.y = Lerp(m_DestPosition[nCntVtx].y, start.y, fTime);

	// ���ݒ�
	m_pObj2D[nCntVtx]->SetPosition(pos);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBlackFrame::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] == NULL)
		{
			continue;
		}

		// �`�揈��
		m_pObj2D[nCntSelect]->Draw();
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CBlackFrame::SetState(STATE state)
{
	if (m_state == STATE_INCOMPLETION ||
		m_state == STATE_OUTCOMPLETION)
	{
		m_state = state;
		m_nCntMove = 0;
	}
}
