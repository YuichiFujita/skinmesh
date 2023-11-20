//=============================================================================
// 
//  �̗̓Q�[�W���� [hp_gauge.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "hp_gauge.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WIDTH			(25.0f)	// ����
#define HEIGHT			(5.0f)	// �c��

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CHP_Gauge::m_apTextureFile[] =					// �e�N�X�`���̃t�@�C��
{
	NULL,
	NULL,
	"data\\TEXTURE\\hypnosis_fram.png",
};
int CHP_Gauge::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHP_Gauge::CHP_Gauge(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		m_HPGauge[nCntGauge].pObjBillboard = NULL;
		m_HPGauge[nCntGauge].fMaxWidth = 0.0f;		// ���̍ő�l
		m_HPGauge[nCntGauge].fMaxHeight = 0.0f;		// �����̍ő�l
		m_HPGauge[nCntGauge].fWidthDest = 0.0f;		// ���̍���
		m_nTexIdx[nCntGauge] = 0;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
	}

	m_nLife = 0;	// �̗�
	m_nMaxLife = 0;	// �ő�̗�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHP_Gauge::~CHP_Gauge()
{

}

//==========================================================================
// ��������
//==========================================================================
CHP_Gauge *CHP_Gauge::Create(float fPosLen, int nMaxLife, float fSizeBuff)
{
	// �����p�̃I�u�W�F�N�g
	CHP_Gauge *pHPGauge = NULL;

	if (pHPGauge == NULL)
	{// NULL��������

		// �������̊m��
		pHPGauge = DEBUG_NEW CHP_Gauge;

		//if (pHPGauge->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pHPGauge;
		//	return NULL;
		//}

		if (pHPGauge != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ���_����̒�����n��
			pHPGauge->m_fPosLength = fPosLen;

			// �ő�̗�
			pHPGauge->m_nMaxLife = nMaxLife;

			for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
			{
				// ��������
				pHPGauge->m_HPGauge[nCntGauge].pObjBillboard = 
					pHPGauge->m_HPGauge[nCntGauge].pObjBillboard->Create();

				if (pHPGauge->m_HPGauge[nCntGauge].pObjBillboard == NULL)
				{// NULL��������
					pHPGauge->Uninit();
					return NULL;
				}

				pHPGauge->m_HPGauge[nCntGauge].pObjBillboard->SetSize(
					D3DXVECTOR2(WIDTH * fSizeBuff, HEIGHT * fSizeBuff));	// �T�C�Y
			}

			// ����������
			pHPGauge->Init();

			// ��ނ̐ݒ�
			pHPGauge->SetType(TYPE_HPGAUGE);
		}

		return pHPGauge;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CHP_Gauge::Init(void)
{
	// �ő�̗�
	m_nLife = m_nMaxLife;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{

		// �e��ϐ��̏�����
		m_HPGauge[nCntGauge].fMaxWidth = m_HPGauge[nCntGauge].pObjBillboard->GetSize().x;		// ���̍ő�l
		m_HPGauge[nCntGauge].fMaxHeight = m_HPGauge[nCntGauge].pObjBillboard->GetSize().y;		// �����̍ő�l

		// ���_�J���[�̐ݒ�
		switch (nCntGauge)
		{
		case VTXTYPE_BLACK:		//���n

			// �F�ݒ�
			m_HPGauge[nCntGauge].pObjBillboard->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			break;

		case VTXTYPE_PINK:		//�ϓ�����Q�[�W

			// �F�ݒ�
			m_HPGauge[nCntGauge].pObjBillboard->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
			break;

		case VTXTYPE_FRAM:		//�t���[��

			// �F�ݒ�
			m_HPGauge[nCntGauge].pObjBillboard->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		}


		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntGauge] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntGauge]);

		// �e�N�X�`���̊��蓖��
		m_HPGauge[nCntGauge].pObjBillboard->BindTexture(m_nTexIdx[nCntGauge]);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CHP_Gauge::Uninit(void)
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObjBillboard->Uninit();
			m_HPGauge[nCntGauge].pObjBillboard = NULL;
			m_HPGauge[nCntGauge] = {};
		}
	}

	// ���폜
	Release();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObjBillboard = NULL;
			m_HPGauge[nCntGauge] = {};
		}
	}

	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CHP_Gauge::Update(void)
{

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != NULL)
		{// NULL����Ȃ�������

			if (nCntGauge == VTXTYPE_PINK)
			{// �Q�[�W���������@

				// ��������
				GaugeDecrement(nCntGauge);
			}

			// ���_���W�ݒ�
			SetVtx(nCntGauge);
		}
	}

}

//==================================================================================
// �ʒu�X�V
//==================================================================================
void CHP_Gauge::UpdatePosition(D3DXVECTOR3 pos, int nLife)
{
	// ��������ɂ�����
	pos.y += m_fPosLength;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != NULL)
		{// NULL����Ȃ�������

			// �ʒu�ݒ�
			m_HPGauge[nCntGauge].pObjBillboard->SetPosition(pos);

			// ���݂̗̑͐ݒ�
			m_nLife = nLife;

			//�ڕW�̕��ݒ�
			m_HPGauge[nCntGauge].fWidthDest =
				m_HPGauge[nCntGauge].fMaxWidth * ((float)m_nLife / (float)m_nMaxLife);
		}
	}
}

//==================================================================================
// ��������
//==================================================================================
void CHP_Gauge::GaugeDecrement(int nCntGauge)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	// �����ŏ��X�Ɍ��炵�Ă���
	size.x +=
		(m_HPGauge[nCntGauge].fWidthDest - size.x) * 0.15f;

	// �T�C�Y�ݒ�
	m_HPGauge[nCntGauge].pObjBillboard->SetSize(size);

	// ���_���W�ݒ�
	SetVtx(nCntGauge);

}

//==================================================================================
// �F�X�V
//==================================================================================
void CHP_Gauge::ChangeColor(int nCntGauge)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	// �F�擾
	D3DXCOLOR col = m_HPGauge[nCntGauge].pObjBillboard->GetColor();

	if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.95f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.6f)
	{//HP�Q�[�W8��

		col = D3DXCOLOR(0.2f, 0.6f, 0.2f, 1.0f);
	}
	else if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.6f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.4f)
	{//HP�Q�[�W6��

		col = D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f);
	}
	else if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.4f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.2f)
	{//HP�Q�[�W4��

		col = D3DXCOLOR(0.8f, 0.5f, 0.2f, 1.0f);
	}
	else if (size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.2f)
	{//�댯�G���A

		col = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	}
	else
	{//���^��

		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

	// �F�ݒ�
	m_HPGauge[nCntGauge].pObjBillboard->SetColor(col);

	// �T�C�Y�擾
	m_HPGauge[nCntGauge].pObjBillboard->SetSize(size);

}

//==========================================================================
// �`�揈��
//==========================================================================
void CHP_Gauge::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �`�揈��
		m_HPGauge[nCntGauge].pObjBillboard->Draw();

		// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CHP_Gauge::SetVtx(void)
{

}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CHP_Gauge::SetVtx(int nCntGauge)
{
	// ���_�ݒ�
	m_HPGauge[nCntGauge].pObjBillboard->SetVtx();

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_HPGauge[nCntGauge].pObjBillboard->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), -size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), -size.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_HPGauge[nCntGauge].pObjBillboard->GetVtxBuff()->Unlock();

}
