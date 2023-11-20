//=============================================================================
// 
//  �X�e�[�^�X�E�B���h�E���� [statuswindow.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "statuswindow.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "object2D.h"
#include "calculation.h"
#include "statuswindow_base.h"
#include "object_circlegauge2D.h"
#include "multinumber.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE_SHAPE = "data\\TEXTURE\\statuswindow\\statuswindow_shape02.png";		// �^�̃e�N�X�`��
	const char* TEXTURE_ONLINE = "data\\TEXTURE\\statuswindow\\ONLINE.png";		// �I�����C��
	const char* TEXTURE_OFFLINE = "data\\TEXTURE\\statuswindow\\OFFLINE.png";	// �I�t���C��
	const char* TEXTURE_NUMBER = "data\\TEXTURE\\number_status.png";	// ����
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStatusWindow::CStatusWindow(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	memset(&m_apOnOffSign[0], NULL, sizeof(m_apOnOffSign));		// �I���I�t�T�C��
	memset(&m_apWindowShape[0], NULL, sizeof(m_apWindowShape));	// �E�B���h�E�̌^
	memset(&m_pCircleGauge2D[0], NULL, sizeof(m_pCircleGauge2D));	// �~�Q�[�W�̃|�C���^
	memset(&m_pStatusNumber[0], NULL, sizeof(m_pStatusNumber));		// �X�e�[�^�X�̐���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStatusWindow::~CStatusWindow()
{

}

//==========================================================================
// ��������
//==========================================================================
CStatusWindow *CStatusWindow::Create(D3DXVECTOR3 pos, bool bJoin)
{
	// �����p�̃I�u�W�F�N�g
	CStatusWindow *pScore = NULL;

	if (pScore == NULL)
	{// NULL��������

		// �������̊m��
		pScore = DEBUG_NEW CStatusWindow;

		if (pScore != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �Q�����Ă邩
			pScore->m_bJoin = bJoin;

			// �ʒu�ݒ�
			pScore->SetPosition(pos);

			// ����������
			pScore->Init();
		}

		return pScore;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStatusWindow::Init(void)
{
	// ��ސݒ�
	SetType(CObject::TYPE_SCORE);

	D3DXVECTOR3 pos = GetPosition();
	CStatusWindow_Base *pBase = CStatusWindow_Base::Create(pos);
	D3DXVECTOR3 posBase = pBase->GetPosition();
	D3DXVECTOR2 sizeBase = pBase->GetSize();
	posBase.y = SCREEN_HEIGHT - sizeBase.y;
	pBase->SetPosition(posBase);

	// �e�N�X�`���̃I�u�W�F�N�g�擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	int nIdxTex = pTexture->Regist(TEXTURE_SHAPE);
	for (int i = 0; i < CGameManager::STATUS_MAX; i++)
	{
		if (m_apWindowShape[i] != NULL)
		{
			continue;
		}

		// �E�B���h�E�̌^����
		m_apWindowShape[i] = CObject2D::Create(GetPriority());
		m_apWindowShape[i]->BindTexture(nIdxTex);
		m_apWindowShape[i]->SetSize(pTexture->GetImageSize(nIdxTex) * 0.075f);

		D3DXVECTOR3 WindowPos = mylib_const::DEFAULT_VECTOR3;
		D3DXVECTOR2 WindowSize = m_apWindowShape[i]->GetSize();
		switch (i)
		{
		case CGameManager::STATUS_POWER:
			WindowPos = D3DXVECTOR3(pos.x - WindowSize.x * 1.5f, pos.y, pos.z);
			break;

		case CGameManager::STATUS_SPEED:
			WindowPos = D3DXVECTOR3(pos.x + WindowSize.x * 1.5f, pos.y, pos.z);
			break;

		case CGameManager::STATUS_LIFE:
			WindowPos = D3DXVECTOR3(pos.x, pos.y + WindowSize.y, pos.z);
			break;
		}

		// �ʒu�ݒ�
		m_apWindowShape[i]->SetPosition(WindowPos);
		m_apWindowShape[i]->SetType(CObject::TYPE_OBJECT2D);


		// ���p�`�Q�[�W����
		m_pCircleGauge2D[i] = CObjectCircleGauge2D::Create(6, WindowSize.y * 0.9f);
		m_pCircleGauge2D[i]->SetType(CObject::TYPE_OBJECT2D);
		m_pCircleGauge2D[i]->SetPosition(WindowPos);
		m_pCircleGauge2D[i]->SetRotation(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(30.0f)));

		// �X�e�[�^�X�̐���
		m_pStatusNumber[i] = CMultiNumber::Create(WindowPos, D3DXVECTOR2(WindowSize.x, WindowSize.x) * 0.2f, 4, CNumber::OBJECTTYPE_2D, TEXTURE_NUMBER, true, 8);

		// �l�̐ݒ菈��
		m_pStatusNumber[i]->SetValue(100);

	}

	int nIdxTexOnOffline = pTexture->Regist(TEXTURE_ONLINE);
	for (int i = 0; i < 2; i++)
	{
		m_apOnOffSign[i] = CObject2D::Create(GetPriority());

		m_apOnOffSign[i]->SetType(CObject::TYPE_OBJECT2D);

		if (m_bJoin == true)
		{
			nIdxTexOnOffline = pTexture->Regist(TEXTURE_ONLINE);
		}
		else
		{
			nIdxTexOnOffline = pTexture->Regist(TEXTURE_OFFLINE);
			m_apOnOffSign[i]->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
		}
		m_apOnOffSign[i]->BindTexture(nIdxTexOnOffline);
		m_apOnOffSign[i]->SetSize(pTexture->GetImageSize(nIdxTexOnOffline) * 0.075f);

		D3DXVECTOR3 posShape = m_apWindowShape[i]->GetPosition();
		D3DXVECTOR2 WindowSize = m_apWindowShape[i]->GetSize();
		m_apOnOffSign[i]->SetPosition(D3DXVECTOR3(posShape.x, posShape.y + WindowSize.y * 1.5f, posShape.z));

	}
	
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStatusWindow::Uninit(void)
{
	
	for (int i = 0; i < CGameManager::STATUS_MAX; i++)
	{
		if (m_apWindowShape[i] == NULL)
		{
			continue;
		}
		m_apWindowShape[i] = NULL;
	}

	// �����̃I�u�W�F�N�g�̏I������
	for (int i = 0; i < CGameManager::STATUS_MAX; i++)
	{
		if (m_pStatusNumber[i] == NULL)
		{
			continue;
		}
		m_pStatusNumber[i]->Uninit();
		delete m_pStatusNumber[i];
		m_pStatusNumber[i] = NULL;
	}

	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStatusWindow::Update(void)
{
	// �����̃I�u�W�F�N�g�̍X�V����
	for (int i = 0; i < CGameManager::STATUS_MAX; i++)
	{
		if (m_pStatusNumber[i] == NULL)
		{
			continue;
		}
		m_pStatusNumber[i]->Update();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStatusWindow::Draw(void)
{

}

//==========================================================================
// �Q�[�W�擾
//==========================================================================
CObjectCircleGauge2D *CStatusWindow::GetGauge(CGameManager::eStatus status)
{
	return m_pCircleGauge2D[status];
}
