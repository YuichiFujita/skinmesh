//=============================================================================
// 
//  �����L���O�X�R�A���� [rankingscore.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "rankingscore.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "object2D.h"
#include "number.h"
#include "calculation.h"
#include "ranking.h"
#include "rankingmanager.h"
#include "sound.h"
#include "input.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define RANKINGFILE			"data\\BIN\\ranking.bin"
#define NUMBERTEXTURE		"data\\TEXTURE\\number_blackclover_01.png"	// �e�N�X�`���̃t�@�C��
#define TEX_U				(0.1f)				// U�̕���
#define BASE_WIDTH			(40.0f)		// ����
#define BASE_HEIGHT			(50.0f)		// �c��
#define BASE_DIS_X			(50.0f)		// �Ԋu
#define DIS_X				(50.0f)				// �Ԋu
#define POSY_BASE			(330.0f)			// Y�ʒu
#define BASETEXT_POSX		(180.0f)			// X�ʒu
#define TEXT_MOVETIME		(30)				// �ړ�����
#define TEXT_INTEVAL		(1280.0f)			// �����L���O�̊Ԋu
#define INIT_POSX			(500.0f)			// �����ʒu

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CRankingScore::m_apTextureFile[VTX_MAX] =		// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\rankingnum_01.png",
	"data\\TEXTURE\\rankinglogo_02.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRankingScore::CRankingScore(int nPriority)
{
	// �l�̃N���A
	m_nNumRanking = 0;				// �����L���O��
	memset(&m_nScore[0], 0, sizeof(m_nScore));	// �X�R�A
	memset(&m_nNowScore[0], 0, sizeof(m_nNowScore));		// ����̃X�R�A
	memset(&m_pObj2D[0], NULL, sizeof(m_pObj2D));	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nTexIdxNumber = 0;			// �����e�N�X�`���̃C���f�b�N�X�ԍ�
	memset(&m_nIdxNewRecord[0], 0, sizeof(m_nIdxNewRecord));	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nCntNewRecord = 0;			// �j���[���R�[�h�̃J�E���^�[
	memset(&m_fPosDestX[0], 0, sizeof(m_fPosDestX));	// �ڕW�̈ʒu
	memset(&m_bNewRecord[0], NULL, sizeof(m_bNewRecord));	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	memset(&m_bArrival[0], NULL, sizeof(m_bArrival));	// ��������
	m_bMove = false;
	memset(&m_pScore[0], NULL, sizeof(m_pScore));	// �����̃I�u�W�F�N�g
	m_nType = TYPE_NONE;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRankingScore::~CRankingScore()
{

}

//==========================================================================
// ��������
//==========================================================================
CRankingScore *CRankingScore::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CRankingScore *pScore = NULL;

	if (pScore == NULL)
	{// NULL��������

		// �������̊m��
		pScore = DEBUG_NEW CRankingScore;

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
HRESULT CRankingScore::Init(void)
{
	// ��ސݒ�
	SetType(CObject::TYPE_SCORE);

	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		// ��������
		m_pObj2D[nCntVtx] = CObject2D::Create(7);

		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntVtx] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntVtx]);

		// �e�N�X�`���̊��蓖��
		m_pObj2D[nCntVtx]->GetObject2D()->BindTexture(m_nTexIdx[nCntVtx]);

		// �e��ϐ��̏�����
		switch (nCntVtx)
		{
		case VTX_LOGO:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.3f);	// �T�C�Y
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(D3DXVECTOR3(m_pObj2D[nCntVtx]->GetSize().x, 60.0f, 0.0f));	// �ʒu
			break;

		case VTX_NUM:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.5f);	// �T�C�Y
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(D3DXVECTOR3(m_pObj2D[nCntVtx]->GetSize().x, 360.0f, 0.0f));	// �ʒu
			break;

		default:
			break;
		}

		// ��ނ̐ݒ�
		m_pObj2D[nCntVtx]->GetObject2D()->SetType(CObject::TYPE_SCORE);
	}

	// 
	int InitPos = 2;

	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
		{
			// �ڕW�̈ʒu
			m_fPosDestX[nCntPlayer][nCntRanking] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + nCntPlayer * TEXT_INTEVAL;

			if (nCntPlayer >= 3)
			{
				m_fPosDestX[nCntPlayer][nCntRanking] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 - InitPos * TEXT_INTEVAL;
			}

			for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
			{
				// ��������
				m_pScore[nCntPlayer][nCntRanking][nCntScore] = CNumber::Create(CNumber::OBJECTTYPE_2D, 7);

				// �e��ϐ��̏�����
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetSize(D3DXVECTOR2(BASE_WIDTH, BASE_HEIGHT));	// �T�C�Y
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + nCntScore * BASE_DIS_X + nCntPlayer * TEXT_INTEVAL, nCntRanking * 120.0f + 170.0f, 0.0f));	// �ʒu

				if (nCntPlayer >= 3)
				{
					m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + nCntScore * BASE_DIS_X - InitPos * TEXT_INTEVAL, nCntRanking * 120.0f + 170.0f, 0.0f));	// �ʒu
				}

				// ��ނ̐ݒ�
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetType(CObject::TYPE_SCORE);

				// �e�N�X�`���̊��蓖��
				m_nTexIdxNumber = CManager::GetInstance()->GetTexture()->Regist(NUMBERTEXTURE);

				// �e�N�X�`���̊��蓖��
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->BindTexture(m_nTexIdxNumber);
			}
		}

		if (nCntPlayer >= 3)
		{
			InitPos--;
		}
	}

	// �����L���O�ǂݍ���
	Load();

	if (CManager::GetInstance()->GetOldMode() == CScene::MODE_RESULT)
	{
		// ����̃X�R�A�擾
		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{// �������ݒ�
			m_nNowScore[nCntPlayer] = CManager::GetInstance()->GetRankingManager()->GetNowScore(nCntPlayer);
		}

		// �\�[�g����
		Sort();

		// �Z�[�u����
		Save();
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRankingScore::Uninit(void)
{
	// �I������
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_pObj2D[nCntVtx] != NULL)
		{// �������̊m�ۂ�����Ă�����

			// �I������
			m_pObj2D[nCntVtx]->Uninit();
			m_pObj2D[nCntVtx] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
		{
			for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
			{
				// �I������
				if (m_pScore[nCntRanking][nCntScore] != NULL)
				{// �������̊m�ۂ�����Ă�����

					// �I������
					m_pScore[nCntPlayer][nCntRanking][nCntScore]->Uninit();
					delete m_pScore[nCntPlayer][nCntRanking][nCntScore];
					m_pScore[nCntPlayer][nCntRanking][nCntScore] = NULL;
				}
			}
		}
	}

	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRankingScore::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
		{
			//�L�����N�^�[�؂�ւ�
			if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LB, 0) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LT, 0) || pInputKeyboard->GetTrigger(DIK_LEFT) == true)
			{
				if (m_bMove == false)
				{
					for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
					{
						// �ʒu�擾
						PrePos[nCntPlayer] = m_pScore[nCntPlayer][0][0]->GetObject2D()->GetPosition();
					}

					// �ڕW�ʒu�̕ύX
					for (int nCnt = 0; nCnt < RANKINGNUM_PLAYER; nCnt++)
					{
						for (int nCntScore = 0; nCntScore < RANKINGNUM; nCntScore++)
						{
							m_fPosDestX[nCnt][nCntScore] -= TEXT_INTEVAL;


							if (m_fPosDestX[nCnt][nCntScore] < SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 - TEXT_INTEVAL * 2)
							{
								m_fPosDestX[nCnt][nCntScore] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + TEXT_INTEVAL * 2;

								// �ʒu�擾
								D3DXVECTOR3 pos = m_pScore[nCnt][nCntRanking][nCntScore]->GetObject2D()->GetPosition();

								// �ڕW�ʒu����
								pos.x = m_fPosDestX[nCnt][nCntScore];

								// �ʒu�ݒ�
								m_pScore[nCnt][nCntRanking][nCntScore]->GetObject2D()->SetPosition(pos);
							}
						}
					}
				}

				m_nType = TYPE_RIGHT;

				m_bMove = true;
			}

			if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_RB, 0) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_RT, 0) || pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
			{
				if (m_bMove == false)
				{
					for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
					{
						// �ʒu�擾
						PrePos[nCntPlayer] = m_pScore[nCntPlayer][0][0]->GetObject2D()->GetPosition();
					}

					// �ڕW�ʒu�̕ύX
					for (int nCnt = 0; nCnt < RANKINGNUM_PLAYER; nCnt++)
					{
						for (int nCntScore = 0; nCntScore < RANKINGNUM; nCntScore++)
						{
							m_fPosDestX[nCnt][nCntScore] += TEXT_INTEVAL;


							if (m_fPosDestX[nCnt][nCntScore] > SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + TEXT_INTEVAL * 2)
							{
								m_fPosDestX[nCnt][nCntScore] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 - TEXT_INTEVAL * 2;

								// �ʒu�擾
								D3DXVECTOR3 pos = m_pScore[nCnt][nCntRanking][nCntScore]->GetObject2D()->GetPosition();

								// �ڕW�ʒu����
								pos.x = m_fPosDestX[nCnt][nCntScore];

								// �ʒu�ݒ�
								m_pScore[nCnt][nCntRanking][nCntScore]->GetObject2D()->SetPosition(pos);
							}
						}
					}
				}

				m_nType = TYPE_LEFT;

				m_bMove = true;
			}


			// �ړ�����
			if (m_bMove == true)
			{
				Moving(nCntRanking, nCntPlayer);
			}


			// �l�̐ݒ菈��
			SetValue(nCntRanking, nCntPlayer);
		}

		if (m_bNewRecord[nCntPlayer] == true)
		{// �L�^�X�V���Ă�����

			// �j���[���R�[�h�̍X�V����
			UpdateNewRecord();
		}
	}

}

//==========================================================================
// �ړ�����
//==========================================================================
void CRankingScore::Moving(int nCntRanking, int nCntPlayer)
{
	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
	{
		// �ʒu�擾
		D3DXVECTOR3 pos = m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->GetPosition();

		if (pos.x == INIT_POSX)
		{
			
		}

		float fDest = m_fPosDestX[nCntPlayer][nCntRanking] + nCntScore * BASE_DIS_X;


		// �ڕW�̈ʒu�֕␳
		if(m_nType == TYPE_RIGHT)
		{//�E�ֈړ�

			if (fDest < pos.x)
			{
				pos.x += (fDest - (PrePos[nCntPlayer].x + nCntScore * BASE_DIS_X)) / (float)TEXT_MOVETIME;
			}

			if (fDest > pos.x)
			{// �ڕW�ŌŒ�
				pos.x = m_fPosDestX[nCntPlayer][nCntRanking] + nCntScore * BASE_DIS_X;
				m_bArrival[nCntPlayer][nCntRanking] = true;

				if (nCntPlayer == RANKINGNUM_PLAYER - 1 &&
					nCntRanking == RANKINGNUM - 1 &&
					nCntScore == RANKINGSCORE_DIGIT - 1)
				{
					CRanking::SetEnableArrival();

					m_bMove = false;

					m_nType = TYPE_NONE;
				}

				//// �T�E���h�Đ�
				//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_RANKINGSCORE);
			}
		}

		if (m_nType == TYPE_LEFT)
		{//���ֈړ�

			if (fDest > pos.x)
			{
				pos.x += (fDest - (PrePos[nCntPlayer].x + nCntScore * BASE_DIS_X)) / (float)TEXT_MOVETIME;
			}

			if (fDest < pos.x)
			{// �ڕW�ŌŒ�
				pos.x = m_fPosDestX[nCntPlayer][nCntRanking] + nCntScore * BASE_DIS_X;
				m_bArrival[nCntPlayer][nCntRanking] = true;

				if (nCntPlayer == RANKINGNUM_PLAYER - 1 &&
					nCntRanking == RANKINGNUM - 1 &&
					nCntScore == RANKINGSCORE_DIGIT - 1)
				{
					CRanking::SetEnableArrival();

					m_bMove = false;

					m_nType = TYPE_NONE;
				}

				//// �T�E���h�Đ�
				//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_RANKINGSCORE);
			}
		}

		// �ʒu�ݒ�
		m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetPosition(pos);
	}
}

//==========================================================================
// �j���[���R�[�h�̍X�V����
//==========================================================================
void CRankingScore::UpdateNewRecord(void)
{
	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
		{
			// �ʒu�擾
			D3DXVECTOR3 pos = m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->GetPosition();

			// �T�C�Y�擾
			D3DXVECTOR2 size = m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->GetSize();

			// �F�擾
			D3DXCOLOR col = m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->GetColor();

			// �Ȑ��␳
			CuadricCurveComp(col.a, 180, 0.3f, 1.0f, m_nCntNewRecord);

			// �ʒu�ݒ�
			m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->SetPosition(pos);

			// �T�C�Y�ݒ�
			m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->SetSize(size);

			// �F�ݒ�
			m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->SetColor(col);
		}
	}
}
//
////==========================================================================
//// �S�Ă̓����ݒ�
////==========================================================================
//void CRankingScore::SetAllArrival(void)
//{
//
//	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
//	{
//		m_bArrival[nCntScore] = true;	// ��������
//	}
//
//	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
//	{
//		for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
//		{
//			// �ʒu�擾
//			D3DXVECTOR3 pos = m_pScore[nCntRanking][nCntScore]->GetObject2D()->GetPosition();
//
//			if (pos.x == INIT_POSX)
//			{
//				// �T�E���h�Đ�
//				CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_NUMBERMOVE);
//			}
//
//			// �F�擾
//			D3DXCOLOR col = m_pScore[nCntRanking][nCntScore]->GetObject2D()->GetColor();
//
//			// �ړ�
//			float fDest = m_fPosDestX[nCntRanking] + nCntScore * BASE_DIS_X;
//			pos.x = fDest;
//
//			// �s�����x�ݒ�
//			col.a = 1.0f;
//
//			// �ʒu�ݒ�
//			m_pScore[nCntRanking][nCntScore]->GetObject2D()->SetPosition(pos);
//
//			// �F�ݒ�
//			m_pScore[nCntRanking][nCntScore]->GetObject2D()->SetColor(col);
//		}
//	}
//
//}

//==========================================================================
// �l�̐ݒ菈��
//==========================================================================
void CRankingScore::SetValue(int nCntRanking, int nCntPlayer)
{
	int aTexU[RANKINGSCORE_DIGIT];
	int nDigit = 1;		// aTexU�v�Z�p

	// �v�Z�p����o��
	for (int nCntDigit = 0; nCntDigit < RANKINGSCORE_DIGIT; nCntDigit++)
	{
		nDigit *= 10;
	}

	// �e�N�X�`�����W�ɑ������
	
	for (int nCntTex = 0; nCntTex < RANKINGSCORE_DIGIT; nCntTex++)
	{// �������ݒ�

		aTexU[nCntTex] = m_nScore[nCntPlayer][nCntRanking] % nDigit / (nDigit / 10);
		nDigit /= 10;
	}


	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
	{
		if (m_pScore[nCntPlayer][nCntRanking][nCntScore] != NULL)
		{// NULL����Ȃ�������

			D3DXVECTOR2 *pTex = m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->GetTex();

			// �e�N�X�`�����W�̐ݒ�
			pTex[0] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U, 0.0f);
			pTex[1] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U + TEX_U, 0.0f);
			pTex[2] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U, 1.0f);
			pTex[3] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U + TEX_U, 1.0f);

			// ���_�ݒ�
			m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetVtx();
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRankingScore::Draw(void)
{

}

//==============================================================
// �\�[�g����
//==============================================================
void CRankingScore::Sort(void)
{
	// �~������
	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntData = 0; nCntData < m_nNumRanking / RANKINGNUM_PLAYER - 1; nCntData++)
		{
			for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking / RANKINGNUM_PLAYER; nCntSort++)
			{
				if (m_nScore[nCntPlayer][nCntData] < m_nScore[nCntPlayer][nCntSort])
				{// �v�f1���v�f2���傫��������

					int nTemp = m_nScore[nCntPlayer][nCntData];
					m_nScore[nCntPlayer][nCntData] = m_nScore[nCntPlayer][nCntSort];
					m_nScore[nCntPlayer][nCntSort] = nTemp;
				}
			}
		}

		// �����L���O�ő�l��������̂�������������
		if (m_nScore[m_nNumRanking / RANKINGNUM_PLAYER - 1] < m_nNowScore)
		{// ����ւ�

			m_nScore[nCntPlayer][m_nNumRanking / RANKINGNUM_PLAYER - 1] = m_nNowScore[nCntPlayer];	// �ŏ��l�ƍ���̃f�[�^����ւ�
			m_bNewRecord[nCntPlayer] = true;	// �t���OON
		}
	}

	

	// �~������
	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntData = 0; nCntData < m_nNumRanking / RANKINGNUM_PLAYER - 1; nCntData++)
		{
			for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking / RANKINGNUM_PLAYER; nCntSort++)
			{
				if (m_nScore[nCntPlayer][nCntData] < m_nScore[nCntPlayer][nCntSort])
				{// �v�f1���v�f2���傫��������

					int nTemp = m_nScore[nCntPlayer][nCntData];
					m_nScore[nCntPlayer][nCntData] = m_nScore[nCntPlayer][nCntSort];
					m_nScore[nCntPlayer][nCntSort] = nTemp;
				}
			}
		}

		if (m_bNewRecord[nCntPlayer] == true)
		{// �j���[���R�[�h�̎�

			for (int nCount = RANKINGNUM; nCount > 0; nCount--)
			{// �v�f�ԍ��L�����Ĕ�����

				if (m_nScore[nCount - 1] == m_nNowScore)
				{
					m_nIdxNewRecord[nCntPlayer] = nCount - 1;
					break;
				}
			}
		}
	}
}

//==============================================================
// �Z�[�u����
//==============================================================
void CRankingScore::Save(void)
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(RANKINGFILE, "wb");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ

		// �t�@�C���ɐ��l�������o��
		fwrite(&m_nNumRanking, sizeof(int), 1, pFile);

		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{
			fwrite(&m_nScore[nCntPlayer][0], sizeof(int), m_nNumRanking / RANKINGNUM_PLAYER, pFile);
		}

		// �t�@�C�������
		fclose(pFile);
	}
}

//==============================================================
// ���[�h����
//==============================================================
void CRankingScore::Load(void)
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(RANKINGFILE, "rb");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		// �t�@�C���ɐ��l��ǂݍ���
		fread(&m_nNumRanking, sizeof(int), 1, pFile);

		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{
			fread(&m_nScore[nCntPlayer][0], sizeof(int), m_nNumRanking / RANKINGNUM_PLAYER, pFile);
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ������ꍇ

		m_nNumRanking = RANKINGNUM * RANKINGNUM_PLAYER;

		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{
			for (int nCount = 0; nCount < m_nNumRanking / RANKINGNUM_PLAYER; nCount++)
			{// �S���[��
				m_nScore[nCntPlayer][nCount] = 150000 - nCount * 25000 + nCntPlayer * 50000;
			}
		}

		// �Z�[�u
		Save();
	}
}