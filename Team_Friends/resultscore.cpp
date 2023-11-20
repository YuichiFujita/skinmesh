//=============================================================================
// 
//  �w�i���� [resultscore.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "resultscore.h"
#include "result.h"
#include "manager.h"
#include "resultmanager.h"
#include "texture.h"
#include "renderer.h"
#include "object2D.h"
#include "multinumber.h"
#include "calculation.h"
#include "rankingmanager.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE				"data\\TEXTURE\\resultscoretext_00.png"	// �e�N�X�`���̃t�@�C��
#define NUMBERTEXTURE		"data\\TEXTURE\\number_blackclover_01.png"	// �e�N�X�`���̃t�@�C��
#define MAX_VALUE			(999999)			// �l�̍ő�l
#define TEX_U				(0.1f)				// U�̕���
#define WIDTH				(40.0f)				// ����
#define HEIGHT				(50.0f)				// �c��
#define DIS_X				(50.0f)				// �Ԋu
#define POSY				(600.0f)			// Y�ʒu
#define TEXT_POSX			(400.0f)			// X�ʒu
#define INT_SCORESET		(120)				// �X�R�A�̕ϓ�����
#define TEXT_MOVETIME		(20)				// �ړ�����
#define INIT_POSX			(1600.0f)			// �����ʒu

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResultScore::CResultScore(int nPriority)
{
	// �l�̃N���A
	m_nToatalNum = 0;			// �l
	m_nToatalNumDest = 0;		// �l
	m_nTexIdx = 0;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nTexIdxNumber = 0;		// �����e�N�X�`���̃C���f�b�N�X�ԍ�
	m_fToatalPosDest_X = 0.0f;	// �ŏI�X�R�A�̖ڕW�ʒu
	m_bArrivalToatal = false;	// �ŏI�X�R�A�̓�������
	m_bSetToatal = false;		// �ŏI�X�R�A�̐ݒ蔻��
	m_pToatalObj2D = NULL;		// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	m_pToatalScore = NULL;		// �����̃I�u�W�F�N�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResultScore::~CResultScore()
{

}

//==========================================================================
// ��������
//==========================================================================
CResultScore *CResultScore::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CResultScore *pScore = NULL;

	if (pScore == NULL)
	{// NULL��������

		// �������̊m��
		pScore = DEBUG_NEW CResultScore;

		if (pScore != NULL)
		{// �������̊m�ۂ��o���Ă�����

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
HRESULT CResultScore::Init(void)
{
	// ��ސݒ�
	SetType(CObject::TYPE_SCORE);

	m_fToatalPosDest_X = TEXT_POSX;		// �ŏI�X�R�A�̖ڕW�ʒu

	// �ŏI�X�R�A�̐���
	CreateToatalScore();

	// �ڕW�̃X�R�A�擾
	m_nToatalNumDest = CManager::GetInstance()->GetResultManager()->GetToatalScore();
	CManager::GetInstance()->GetRankingManager()->SetNowScore(m_nToatalNumDest, 0);
	
	return S_OK;
}

//==========================================================================
// �ŏI�X�R�A�̐���
//==========================================================================
void CResultScore::CreateToatalScore(void)
{
	// ��������
	m_pToatalObj2D = CObject2D::Create(7);

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

	// �e�N�X�`���̊��蓖��
	m_pToatalObj2D->GetObject2D()->BindTexture(m_nTexIdx);

	// �e��ϐ��̏�����
	m_pToatalObj2D->GetObject2D()->SetSize(CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx) * 0.4f);	// �T�C�Y
	m_pToatalObj2D->GetObject2D()->SetPosition(D3DXVECTOR3(INIT_POSX, POSY, 0.0f));	// �ʒu

	// ��ނ̐ݒ�
	m_pToatalObj2D->GetObject2D()->SetType(CObject::TYPE_SCORE);

	// ��������
	m_pToatalScore = CMultiNumber::Create(D3DXVECTOR3(INIT_POSX + m_pToatalObj2D->GetSize().x + 50.0f, POSY, 0.0f), D3DXVECTOR2(WIDTH, HEIGHT), RESULTSCORE_DIGIT, CNumber::OBJECTTYPE_2D);
}

//==========================================================================
// �I������
//==========================================================================
void CResultScore::Uninit(void)
{
	
	// �I������
	if (m_pToatalScore != NULL)
	{// �������̊m�ۂ�����Ă�����

		// �I������
		m_pToatalScore->Uninit();
		delete m_pToatalScore;
		m_pToatalScore = NULL;
	}

	// �I������
	if (m_pToatalObj2D != NULL)
	{// �������̊m�ۂ�����Ă�����

		 // �I������
		m_pToatalObj2D->Uninit();
		m_pToatalObj2D = NULL;
	}

	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CResultScore::Update(void)
{
	// �ŏI�X�R�A�̈ړ�����
	MoveToatalScore();

	// �ŏI�X�R�A�̐ݒ菈��
	SetToatalValue();
}

//==========================================================================
// �ŏI�X�R�A�̈ړ�����
//==========================================================================
void CResultScore::MoveToatalScore(void)
{
	if (m_pToatalObj2D == NULL)
	{// �ŏI�X�R�A����������Ă�����
		return;
	}

	// �ʒu�擾
	D3DXVECTOR3 pos = m_pToatalObj2D->GetPosition();

	if (pos.x == INIT_POSX)
	{
		// �T�E���h�Đ�
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_NUMBERMOVE);
	}

	// �ړ�
	pos.x += (m_fToatalPosDest_X - INIT_POSX) / (float)TEXT_MOVETIME;
	if (pos.x <= m_fToatalPosDest_X)
	{// �␳����
		pos.x = m_fToatalPosDest_X;
		m_bArrivalToatal = true;	// �ŏI�X�R�A�̓�������
	}

	// �ʒu�ݒ�
	m_pToatalObj2D->SetPosition(pos);

	if (m_pToatalScore == NULL)
	{// NULL��������
		return;
	}

	// �ʒu�擾
	D3DXVECTOR3 posNumber = m_pToatalScore->GetPosition();

	// �ʒu�X�V
	m_pToatalScore->SetPosition(
		D3DXVECTOR3(
			pos.x + m_pToatalObj2D->GetSize().x + 50.0f,
			posNumber.y,
			0.0f));

	if (m_bArrivalToatal == true)
	{// �������Ă�����

		// �ڕW�̃X�R�A�擾
		m_nToatalNumDest = CManager::GetInstance()->GetResultManager()->GetToatalScore();

		CManager::GetInstance()->GetRankingManager()->SetNowScore(m_nToatalNumDest, 0);

		if (m_nToatalNumDest > m_nToatalNum)
		{// �K�莞�Ԃ����ĕ␳
			m_nToatalNum += (int)((float)m_nToatalNumDest / (float)INT_SCORESET);

			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_NUMBER);
		}
		else
		{// �ڕW�̒l�ŌŒ�
			m_nToatalNum = m_nToatalNumDest;
			m_bSetToatal = true;			// �ŏI�X�R�A�̐ݒ蔻��
			CResult::SetEnableArrival();
		}
	}
}

//==========================================================================
// �ŏI�X�R�A�̐ݒ菈��
//==========================================================================
void CResultScore::SetToatalValue(void)
{
	if (m_pToatalScore == NULL)
	{// NULL��������
		return;
	}

	// �l�̐ݒ菈��
	m_pToatalScore->SetValue(m_nToatalNum);
}

//==========================================================================
// �S�Ă̓����ݒ�
//==========================================================================
void CResultScore::SetAllArrival(void)
{
	m_bArrivalToatal = true;		// �ŏI�X�R�A�̓�������
	m_bSetToatal = true;			// �ŏI�X�R�A�̐ݒ蔻��

	// �ڕW�̒l�ɐݒ�
	m_nToatalNum = m_nToatalNumDest;

	// �ʒu�擾
	D3DXVECTOR3 pos = m_pToatalObj2D->GetPosition();

	// �ړ�
	pos.x = m_fToatalPosDest_X;

	// �ʒu�ݒ�
	m_pToatalObj2D->SetPosition(pos);

	if (m_pToatalScore == NULL)
	{// NULL��������
		return;
	}

	// �ʒu�擾
	D3DXVECTOR3 posNumber = m_pToatalScore->GetPosition();

	// �ʒu�X�V
	m_pToatalScore->SetPosition(
		D3DXVECTOR3(
			pos.x + m_pToatalObj2D->GetSize().x + 50.0f,
			posNumber.y,
			0.0f));

}

//==========================================================================
// �`�揈��
//==========================================================================
void CResultScore::Draw(void)
{

}

//==========================================================================
// �l�̐ݒ菈��
//==========================================================================
void CResultScore::SetValue(int nScore)
{
	m_nToatalNumDest = nScore;
}
