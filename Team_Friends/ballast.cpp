//=============================================================================
// 
//  ���I���� [ballast.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "game.h"
#include "ballast.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "objectX.h"
#include "elevation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define BAI		(0.4f)
#define LIFE	(40)	// ����
#define MOVE	(2.0f)	// �ړ���
#define MOVE_Y	(30.0f * BAI)	// �ړ���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CBallast::m_pFileName[TYPE_MAX] = // �t�@�C����
{
	"data\\MODEL\\ballast_01.x",
	"data\\MODEL\\magic_ice.x"
};
int CBallast::m_nIdxXFile[TYPE_MAX] = {};			// X�t�@�C���̃C���f�b�N�X�ԍ�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBallast::CBallast(int nPriority) : CObject(nPriority)
{
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���_
	m_moveOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���̈ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���F
	m_nCntParabola = 0;	// ���I�̈ړ��J�E���g
	m_nNumAll = 0;		// ����
	m_nLife = 0;		// ����
	m_nMaxLife = 0;		// �ő����
	m_type =TYPE_STONE;	// ���
	memset(&m_pObjX[0], NULL, sizeof(m_pObjX));	// �I�u�W�F�N�gX�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBallast::~CBallast()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBallast *CBallast::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nNum, float fAlpha, TYPE type)
{
	// �����p�̃I�u�W�F�N�g
	CBallast *pBallast = NULL;

	if (pBallast == NULL)
	{// NULL��������

		// �������̊m��
		pBallast = DEBUG_NEW CBallast;

		if (pBallast != NULL)
		{// �������̊m�ۂ��o���Ă�����

			if (nNum >= MAX_BALLAST)
			{
				nNum = MAX_BALLAST;
			}

			// �����ݒ�
			pBallast->m_nNumAll = nNum;

			// ��ސݒ�
			pBallast->m_type = type;

			// �ʒu�ݒ�
			pBallast->m_posOrigin = pos;

			// �ړ��ʐݒ�
			pBallast->m_moveOrigin = move;

			// ���F�ݒ�
			pBallast->m_colOrigin.a = fAlpha;

			// ����������
			HRESULT hr = pBallast->Init();

			if (FAILED(hr))
			{
				return NULL;
			}
		}

		return pBallast;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBallast::Init(void)
{
	// ��ނ̐ݒ�
	SetType(TYPE_BALLAST);

	// �����ݒ�
	m_nLife = LIFE * m_nNumAll;
	m_nMaxLife = m_nLife;

	for (int nCntBallast = 0; nCntBallast < m_nNumAll; nCntBallast++)
	{
		// ��������
		m_pObjX[nCntBallast] = CObjectX::Create(
			m_pFileName[m_type],
			D3DXVECTOR3(m_posOrigin.x + Random(-20, 20), m_posOrigin.y, m_posOrigin.z + Random(-20, 20)),
			D3DXVECTOR3((float)Random(-314, 314) * 0.01f, (float)Random(-314, 314) * 0.01f, (float)Random(-314, 314) * 0.01f), false);

		if (m_pObjX[nCntBallast] == NULL)
		{// ���s�����Ƃ�

			// �I�u�W�F�N�gX�̏I������
			delete m_pObjX[nCntBallast];
			m_pObjX[nCntBallast] = NULL;
			return E_FAIL;
		}
		
		// �ړ��ʐݒ�
		m_pObjX[nCntBallast]->SetMove(D3DXVECTOR3(
			(float)Random(-314, 314) * 0.01f * m_moveOrigin.x,
			m_moveOrigin.y + ((float)Random(-20, 20) * 0.1f),
			(float)Random(-314, 314) * 0.01f * m_moveOrigin.z));

		// �X�P�[���ݒ�
		switch (m_type)
		{
		case TYPE_STONE:

			m_pObjX[nCntBallast]->SetScale(D3DXVECTOR3(
				(float)Random(-20, 20) * 0.05f,
				(float)Random(-20, 20) * 0.05f,
				(float)Random(-20, 20) * 0.05f));
			break;

		case TYPE_ICE:
			m_pObjX[nCntBallast]->SetScale(D3DXVECTOR3(
				(float)Random(-10, 10) * 0.1f,
				(float)Random(-10, 10) * 0.1f,
				(float)Random(-10, 10) * 0.1f));
			break;
		}
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBallast::Uninit(void)
{
	for (int nCntBallast = 0; nCntBallast < MAX_BALLAST; nCntBallast++)
	{
		if (m_pObjX[nCntBallast] != NULL)
		{// NULL����Ȃ�������

			// �I�u�W�F�N�gX�̏I������
			m_pObjX[nCntBallast]->Uninit();
			m_pObjX[nCntBallast] = NULL;
		}
	}

	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBallast::Update(void)
{
	// ���I�̈ړ��J�E���g���Z
	m_nCntParabola++;

	for (int nCntBallast = 0; nCntBallast < m_nNumAll; nCntBallast++)
	{
		if (m_pObjX[nCntBallast] == NULL)
		{// NULL��������

			// �����X�V
			m_nLife = 0;
			continue;
		}

		// �ʒu�擾
		D3DXVECTOR3 pos = m_pObjX[nCntBallast]->GetPosition();

		// �ړ��ʎ擾
		D3DXVECTOR3 move = m_pObjX[nCntBallast]->GetMove();

		// �����擾
		D3DXVECTOR3 rot = m_pObjX[nCntBallast]->GetRotation();

		if (m_type == TYPE_ICE)
		{
			// �����擾
			D3DXVECTOR3 scale = m_pObjX[nCntBallast]->GetScale();

			scale *= 0.98f;

			// �T�C�Y�ݒ�
			m_pObjX[nCntBallast]->SetScale(scale);

		}

		float PosY = pos.y;
		if (CGame::GetElevation()->IsHit(D3DXVECTOR3(pos.x, PosY, pos.z)) == true || GetPosition().y <= mylib_const::KILL_Y)
		{// �n�ʂ����������

			// �����X�V
			m_nLife--;

			pos.y = PosY;
		}
		else
		{
			// �ʒu�X�V
			pos = (D3DXVECTOR3(0.0f, -0.5f * BAI, 0.0f) * (float)(m_nCntParabola * m_nCntParabola) + move * (float)m_nCntParabola) + m_posOrigin;

			// �����X�V
			rot += D3DXVECTOR3(0.05f, 0.05f, -0.02f);
		}

		// �s�����x�X�V
		m_col.a = m_colOrigin.a * ((float)m_nLife / (float)m_nMaxLife);

		// �ʒu�ݒ�
		m_pObjX[nCntBallast]->SetPosition(pos);

		// �����ݒ�
		m_pObjX[nCntBallast]->SetRotation(rot);
	}

	if (m_nLife <= 0)
	{// �������s������

		// �I������
		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBallast::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntBallast = 0; nCntBallast < m_nNumAll; nCntBallast++)
	{
		if (m_pObjX[nCntBallast] != NULL)
		{// NULL����Ȃ�������44

			// �`�揈��
			m_pObjX[nCntBallast]->Draw(m_col.a);
		}
	}

}
