//=============================================================================
// 
//  �v���C���[�̗̑̓Q�[�W���� [hp_gauge_player.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "hp_gauge_player.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WIDTH			(100.0f)	// ����
#define HEIGHT			(25.0f)	// �c��

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CHP_GaugePlayer::m_apTextureFile[] =					// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\player_HPgaugeMoto.png",
	"data\\TEXTURE\\player_HPgauge.png",
	"data\\TEXTURE\\player_HPFrampng.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHP_GaugePlayer::CHP_GaugePlayer(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		m_HPGauge[nCntGauge].pObj2D = NULL;
		m_HPGauge[nCntGauge].fMaxWidth = 0.0f;		// ���̍ő�l
		m_HPGauge[nCntGauge].fMaxHeight = 0.0f;		// �����̍ő�l
		m_HPGauge[nCntGauge].fWidthDest = 0.0f;		// ���̍���
		m_nTexIdx[nCntGauge] = 0;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
	}

	m_nLife = 0;	// �̗�
	m_nMaxLife = 0;	// �ő�̗�
	m_nCntTkTk = 0;	// �`�J�`�J�̃J�E���g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHP_GaugePlayer::~CHP_GaugePlayer()
{

}

//==========================================================================
// ��������
//==========================================================================
CHP_GaugePlayer *CHP_GaugePlayer::Create(D3DXVECTOR3 pos, int nMaxLife)
{
	// �����p�̃I�u�W�F�N�g
	CHP_GaugePlayer *pHPGauge = NULL;

	if (pHPGauge == NULL)
	{// NULL��������

		// �������̊m��
		pHPGauge = DEBUG_NEW CHP_GaugePlayer;

		if (pHPGauge != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ő�̗�
			pHPGauge->m_nMaxLife = nMaxLife;

			// �ʒu�ݒ�
			pHPGauge->SetPosition(pos);

			// ����������
			pHPGauge->Init();

			// ��ނ̐ݒ�
			//pHPGauge->SetType(TYPE_HPGAUGE);
		}

		return pHPGauge;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CHP_GaugePlayer::Init(void)
{
	// �ő�̗�
	m_nLife = m_nMaxLife;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// ��������
		m_HPGauge[nCntGauge].pObj2D = CObject2D::Create();
		if (m_HPGauge[nCntGauge].pObj2D == NULL)
		{// NULL��������
			Uninit();
			return NULL;
		}

		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntGauge] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntGauge]);

		// �e�N�X�`���̊��蓖��
		m_HPGauge[nCntGauge].pObj2D->BindTexture(m_nTexIdx[nCntGauge]);

		// ���_�J���[�̐ݒ�
		switch (nCntGauge)
		{
		case VTXTYPE_BLACK:		//���n

			// �F�ݒ�
			m_HPGauge[nCntGauge].pObj2D->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			break;

		case VTXTYPE_PINK:		//�ϓ�����Q�[�W

			// �F�ݒ�
			m_HPGauge[nCntGauge].pObj2D->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
			break;

		case VTXTYPE_FRAM:		//�t���[��

			// �F�ݒ�
			m_HPGauge[nCntGauge].pObj2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		}
	}

	D3DXVECTOR2 texsize = CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx[VTXTYPE_FRAM]);
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D == NULL)
		{// NULL��������
			continue;
		}

		switch (nCntGauge)
		{
		case VTXTYPE_BLACK:	//���n

			// �T�C�Y
			m_HPGauge[nCntGauge].pObj2D->SetSize(texsize * 0.3f);
			break;

		case VTXTYPE_PINK:	//�ϓ�����Q�[�W

			// �T�C�Y
			m_HPGauge[nCntGauge].pObj2D->SetSize(texsize * 0.3f);
			break;

		case VTXTYPE_FRAM:	//�t���[��

			// �T�C�Y
			m_HPGauge[nCntGauge].pObj2D->SetSize(texsize * 0.3f);
			break;
		}

		// �e��ϐ��̏�����
		m_HPGauge[nCntGauge].fMaxWidth = m_HPGauge[nCntGauge].pObj2D->GetSize().x;		// ���̍ő�l
		m_HPGauge[nCntGauge].fMaxHeight = m_HPGauge[nCntGauge].pObj2D->GetSize().y;		// �����̍ő�l
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CHP_GaugePlayer::Uninit(void)
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObj2D->Uninit();
			m_HPGauge[nCntGauge].pObj2D = NULL;
		}
	}

	// ���폜
	Release();
}

//==========================================================================
// �I������
//==========================================================================
void CHP_GaugePlayer::Kill(void)
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D != NULL)
		{// NULL����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObj2D->Uninit();
			m_HPGauge[nCntGauge].pObj2D = NULL;
		}
	}


	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CHP_GaugePlayer::Update(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �`�J�`�J�̃J�E���g
	m_nCntTkTk = (m_nCntTkTk + 1) % 120;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D != NULL)
		{// NULL����Ȃ�������

			if (nCntGauge == VTXTYPE_PINK)
			{// �Q�[�W���������@

				// �F�擾
				D3DXCOLOR col = m_HPGauge[nCntGauge].pObj2D->GetColor();

				col = HSVtoRGB(0.0f, 0.0f, 1.0f + sinf(D3DX_PI * ((float)m_nCntTkTk / 60.0f)) * 0.3f);

				// �F�ݒ�
				m_HPGauge[nCntGauge].pObj2D->SetColor(col);

				// ��������
				GaugeDecrement(nCntGauge);
			}

			// �ʒu�ݒ�
			m_HPGauge[nCntGauge].pObj2D->SetPosition(pos);

			// �T�C�Y�ݒ�
			//m_HPGauge[nCntGauge].pObj2D->SetSize(D3DXVECTOR2(m_sizeGauge.x - (m_HPGauge[nCntGauge].fMaxWidth - m_sizeGauge.x), size.y));

			// �X�V����
			m_HPGauge[nCntGauge].pObj2D->Update();

			// ���_���W�ݒ�
			SetVtx(nCntGauge);
		}
	}

}

//==================================================================================
// �ʒu�X�V
//==================================================================================
void CHP_GaugePlayer::SetLife(int nLife)
{
	// ���݂̗̑͐ݒ�
	m_nLife = nLife;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObj2D != NULL)
		{// NULL����Ȃ�������

			//�ڕW�̕��ݒ�
			m_HPGauge[nCntGauge].fWidthDest = m_HPGauge[nCntGauge].fMaxWidth * ((float)m_nLife / (float)m_nMaxLife);
		}
	}
}

//==================================================================================
// ��������
//==================================================================================
void CHP_GaugePlayer::GaugeDecrement(int nCntGauge)
{

	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObj2D->GetSize();

	// �����ŏ��X�Ɍ��炵�Ă���
	size.x += (m_HPGauge[nCntGauge].fWidthDest - size.x) * 0.15f;

	// �T�C�Y�ݒ�
	m_HPGauge[nCntGauge].pObj2D->SetSize(size);

	// ���_���W�ݒ�
	SetVtx(nCntGauge);
}

//==================================================================================
// �F�X�V
//==================================================================================
void CHP_GaugePlayer::ChangeColor(int nCntGauge)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObj2D->GetSize();

	// �F�擾
	D3DXCOLOR col = m_HPGauge[nCntGauge].pObj2D->GetColor();

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
	m_HPGauge[nCntGauge].pObj2D->SetColor(col);

	// �T�C�Y�擾
	m_HPGauge[nCntGauge].pObj2D->SetSize(size);

}

//==========================================================================
// �`�揈��
//==========================================================================
void CHP_GaugePlayer::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// �`�揈��
		m_HPGauge[nCntGauge].pObj2D->Draw();
	}
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CHP_GaugePlayer::SetVtx(void)
{

}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CHP_GaugePlayer::SetVtx(int nCntGauge)
{
	// ���_�ݒ�
	m_HPGauge[nCntGauge].pObj2D->SetVtx();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR2 *pTex = m_HPGauge[nCntGauge].pObj2D->GetTex();

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_HPGauge[nCntGauge].pObj2D->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObj2D->GetSize();

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x + -size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + -size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + -size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + -size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x),	pos.y + size.y, 0.0f);

	pTex[0] = D3DXVECTOR2(0.0f, 0.0f);
	pTex[1] = D3DXVECTOR2(size.x / m_HPGauge[nCntGauge].fMaxWidth, 0.0f);
	pTex[2] = D3DXVECTOR2(0.0f, 1.0f);
	pTex[3] = D3DXVECTOR2(size.x / m_HPGauge[nCntGauge].fMaxWidth, 1.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_HPGauge[nCntGauge].pObj2D->GetVtxBuff()->Unlock();

}
