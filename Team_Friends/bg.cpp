//=============================================================================
// 
//  �w�i���� [bg.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WIDTH			(640.0f)					// ����
#define HEIGHT			(360.0f)					// �c��

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CBG::m_apTextureFile[] =				// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png",
};
const CBG::SBgInfo CBG::m_aBgInfo[] =				// �w�i�̏��̏��
{
	{ 0.001f, 0.0f },	// �ړ���
	{ 0.003f, 0.0f },
	{ 0.005f, 0.0f },
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBG::CBG(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	for (int nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		m_aBg[nCntBg].pObject2D = NULL;
		m_aBg[nCntBg].Info.fTexU = 0.0f;
		m_aBg[nCntBg].Info.fTexV = 0.0f;
		m_aBg[nCntBg].Info.fMoveU = 0.0f;
		m_aBg[nCntBg].Info.fMoveV = 0.0f;
	}

	// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{
		m_nTexIdx[nCntTex] = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBG::~CBG()
{

}

//==========================================================================
// ��������
//==========================================================================
CBG *CBG::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CBG *pBG = NULL;

	if (pBG == NULL)
	{// NULL��������

		// �������̊m��
		pBG = DEBUG_NEW CBG;

		//if (pBG->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pBG;
		//	return NULL;
		//}

		if (pBG != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pBG->Init();

			// ��ނ̐ݒ�
			pBG->SetType(TYPE_BG);
		}

		return pBG;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBG::Init(void)
{
	for (int nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		// ��������
		m_aBg[nCntBg].pObject2D = m_aBg[nCntBg].pObject2D->Create(0);

		// �e��ϐ��̏�����
		m_aBg[nCntBg].pObject2D->SetSize(D3DXVECTOR2(WIDTH, HEIGHT));	// �T�C�Y
		m_aBg[nCntBg].pObject2D->SetPosition(D3DXVECTOR3(640.0f, 360.0f, 0.0f));	// �ʒu
		m_aBg[nCntBg].Info = m_aBgInfo[nCntBg];	// ���n��

		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntBg] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntBg]);

		// �e�N�X�`���̊��蓖��
		m_aBg[nCntBg].pObject2D->BindTexture(m_nTexIdx[nCntBg]);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBG::Uninit(void)
{
	for (int nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		if (m_aBg[nCntBg].pObject2D != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_aBg[nCntBg].pObject2D->Uninit();
			m_aBg[nCntBg].pObject2D = NULL;
		}
	}

	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBG::Update(void)
{
	for (int nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		if (m_aBg[nCntBg].pObject2D != NULL)
		{// NULL����Ȃ�������

			// �X�N���[��
			m_aBg[nCntBg].Info.fTexU += m_aBg[nCntBg].Info.fMoveU;
			m_aBg[nCntBg].Info.fTexV += m_aBg[nCntBg].Info.fMoveV;

			if (m_aBg[nCntBg].Info.fTexU >= 1.0f)
			{
				m_aBg[nCntBg].Info.fTexU = 0.0f;
			}
			else if (m_aBg[nCntBg].Info.fTexU <= 0.0f)
			{
				m_aBg[nCntBg].Info.fTexU = 1.0f;
			}

			if (m_aBg[nCntBg].Info.fTexV >= 1.0f)
			{
				m_aBg[nCntBg].Info.fTexV = 0.0f;
			}
			else if (m_aBg[nCntBg].Info.fTexV <= 0.0f)
			{
				m_aBg[nCntBg].Info.fTexV = 1.0f;
			}

			// ���_���ݒ�
			SetVtx(nCntBg);
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBG::Draw(void)
{
	for (int nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		// �`�揈��
		m_aBg[nCntBg].pObject2D->Draw();
	}
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CBG::SetVtx(void)
{

}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CBG::SetVtx(int nCntBg)
{
	// ���_�ݒ�
	m_aBg[nCntBg].pObject2D->SetVtx();

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_aBg[nCntBg].pObject2D->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_aBg[nCntBg].Info.fTexU,			m_aBg[nCntBg].Info.fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_aBg[nCntBg].Info.fTexU + 1.0f,	m_aBg[nCntBg].Info.fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_aBg[nCntBg].Info.fTexU,			m_aBg[nCntBg].Info.fTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_aBg[nCntBg].Info.fTexU + 1.0f,	m_aBg[nCntBg].Info.fTexV + 1.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_aBg[nCntBg].pObject2D->GetVtxBuff()->Unlock();
}

//==========================================================================
// �w�i�ړ��ʐݒ�j
//==========================================================================
void CBG::SetMoveTex(const D3DXVECTOR3 move)
{
	for (int nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		m_aBg[nCntBg].Info.fTexU += (0.0f - -move.x) * 0.0005f;
		m_aBg[nCntBg].Info.fTexV += (0.0f - -move.y) * 0.001f;
	}
}
