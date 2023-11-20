//=============================================================================
// 
//  ランキングスコア処理 [rankingscore.cpp]
//  Author : 相馬靜雅
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
// マクロ定義
//==========================================================================
#define RANKINGFILE			"data\\BIN\\ranking.bin"
#define NUMBERTEXTURE		"data\\TEXTURE\\number_blackclover_01.png"	// テクスチャのファイル
#define TEX_U				(0.1f)				// Uの分割
#define BASE_WIDTH			(40.0f)		// 横幅
#define BASE_HEIGHT			(50.0f)		// 縦幅
#define BASE_DIS_X			(50.0f)		// 間隔
#define DIS_X				(50.0f)				// 間隔
#define POSY_BASE			(330.0f)			// Y位置
#define BASETEXT_POSX		(180.0f)			// X位置
#define TEXT_MOVETIME		(30)				// 移動時間
#define TEXT_INTEVAL		(1280.0f)			// ランキングの間隔
#define INIT_POSX			(500.0f)			// 初期位置

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CRankingScore::m_apTextureFile[VTX_MAX] =		// テクスチャのファイル
{
	"data\\TEXTURE\\rankingnum_01.png",
	"data\\TEXTURE\\rankinglogo_02.png",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CRankingScore::CRankingScore(int nPriority)
{
	// 値のクリア
	m_nNumRanking = 0;				// ランキング数
	memset(&m_nScore[0], 0, sizeof(m_nScore));	// スコア
	memset(&m_nNowScore[0], 0, sizeof(m_nNowScore));		// 今回のスコア
	memset(&m_pObj2D[0], NULL, sizeof(m_pObj2D));	// オブジェクト2Dのオブジェクト
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));	// テクスチャのインデックス番号
	m_nTexIdxNumber = 0;			// 数字テクスチャのインデックス番号
	memset(&m_nIdxNewRecord[0], 0, sizeof(m_nIdxNewRecord));	// テクスチャのインデックス番号
	m_nCntNewRecord = 0;			// ニューレコードのカウンター
	memset(&m_fPosDestX[0], 0, sizeof(m_fPosDestX));	// 目標の位置
	memset(&m_bNewRecord[0], NULL, sizeof(m_bNewRecord));	// オブジェクト2Dのオブジェクト
	memset(&m_bArrival[0], NULL, sizeof(m_bArrival));	// 到着判定
	m_bMove = false;
	memset(&m_pScore[0], NULL, sizeof(m_pScore));	// 数字のオブジェクト
	m_nType = TYPE_NONE;
}

//==========================================================================
// デストラクタ
//==========================================================================
CRankingScore::~CRankingScore()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRankingScore *CRankingScore::Create(void)
{
	// 生成用のオブジェクト
	CRankingScore *pScore = NULL;

	if (pScore == NULL)
	{// NULLだったら

		// メモリの確保
		pScore = DEBUG_NEW CRankingScore;

		if (pScore != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pScore->Init();
		}

		return pScore;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRankingScore::Init(void)
{
	// 種類設定
	SetType(CObject::TYPE_SCORE);

	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		// 生成処理
		m_pObj2D[nCntVtx] = CObject2D::Create(7);

		// テクスチャの割り当て
		m_nTexIdx[nCntVtx] = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[nCntVtx]);

		// テクスチャの割り当て
		m_pObj2D[nCntVtx]->GetObject2D()->BindTexture(m_nTexIdx[nCntVtx]);

		// 各種変数の初期化
		switch (nCntVtx)
		{
		case VTX_LOGO:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.3f);	// サイズ
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(D3DXVECTOR3(m_pObj2D[nCntVtx]->GetSize().x, 60.0f, 0.0f));	// 位置
			break;

		case VTX_NUM:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.5f);	// サイズ
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(D3DXVECTOR3(m_pObj2D[nCntVtx]->GetSize().x, 360.0f, 0.0f));	// 位置
			break;

		default:
			break;
		}

		// 種類の設定
		m_pObj2D[nCntVtx]->GetObject2D()->SetType(CObject::TYPE_SCORE);
	}

	// 
	int InitPos = 2;

	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
		{
			// 目標の位置
			m_fPosDestX[nCntPlayer][nCntRanking] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + nCntPlayer * TEXT_INTEVAL;

			if (nCntPlayer >= 3)
			{
				m_fPosDestX[nCntPlayer][nCntRanking] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 - InitPos * TEXT_INTEVAL;
			}

			for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
			{
				// 生成処理
				m_pScore[nCntPlayer][nCntRanking][nCntScore] = CNumber::Create(CNumber::OBJECTTYPE_2D, 7);

				// 各種変数の初期化
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetSize(D3DXVECTOR2(BASE_WIDTH, BASE_HEIGHT));	// サイズ
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + nCntScore * BASE_DIS_X + nCntPlayer * TEXT_INTEVAL, nCntRanking * 120.0f + 170.0f, 0.0f));	// 位置

				if (nCntPlayer >= 3)
				{
					m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + nCntScore * BASE_DIS_X - InitPos * TEXT_INTEVAL, nCntRanking * 120.0f + 170.0f, 0.0f));	// 位置
				}

				// 種類の設定
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetType(CObject::TYPE_SCORE);

				// テクスチャの割り当て
				m_nTexIdxNumber = CManager::GetInstance()->GetTexture()->Regist(NUMBERTEXTURE);

				// テクスチャの割り当て
				m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->BindTexture(m_nTexIdxNumber);
			}
		}

		if (nCntPlayer >= 3)
		{
			InitPos--;
		}
	}

	// ランキング読み込み
	Load();

	if (CManager::GetInstance()->GetOldMode() == CScene::MODE_RESULT)
	{
		// 今回のスコア取得
		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{// 桁数分設定
			m_nNowScore[nCntPlayer] = CManager::GetInstance()->GetRankingManager()->GetNowScore(nCntPlayer);
		}

		// ソート処理
		Sort();

		// セーブ処理
		Save();
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRankingScore::Uninit(void)
{
	// 終了処理
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_pObj2D[nCntVtx] != NULL)
		{// メモリの確保がされていたら

			// 終了処理
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
				// 終了処理
				if (m_pScore[nCntRanking][nCntScore] != NULL)
				{// メモリの確保がされていたら

					// 終了処理
					m_pScore[nCntPlayer][nCntRanking][nCntScore]->Uninit();
					delete m_pScore[nCntPlayer][nCntRanking][nCntScore];
					m_pScore[nCntPlayer][nCntRanking][nCntScore] = NULL;
				}
			}
		}
	}

	// 情報削除
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CRankingScore::Update(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
		{
			//キャラクター切り替え
			if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LB, 0) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LT, 0) || pInputKeyboard->GetTrigger(DIK_LEFT) == true)
			{
				if (m_bMove == false)
				{
					for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
					{
						// 位置取得
						PrePos[nCntPlayer] = m_pScore[nCntPlayer][0][0]->GetObject2D()->GetPosition();
					}

					// 目標位置の変更
					for (int nCnt = 0; nCnt < RANKINGNUM_PLAYER; nCnt++)
					{
						for (int nCntScore = 0; nCntScore < RANKINGNUM; nCntScore++)
						{
							m_fPosDestX[nCnt][nCntScore] -= TEXT_INTEVAL;


							if (m_fPosDestX[nCnt][nCntScore] < SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 - TEXT_INTEVAL * 2)
							{
								m_fPosDestX[nCnt][nCntScore] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + TEXT_INTEVAL * 2;

								// 位置取得
								D3DXVECTOR3 pos = m_pScore[nCnt][nCntRanking][nCntScore]->GetObject2D()->GetPosition();

								// 目標位置を代入
								pos.x = m_fPosDestX[nCnt][nCntScore];

								// 位置設定
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
						// 位置取得
						PrePos[nCntPlayer] = m_pScore[nCntPlayer][0][0]->GetObject2D()->GetPosition();
					}

					// 目標位置の変更
					for (int nCnt = 0; nCnt < RANKINGNUM_PLAYER; nCnt++)
					{
						for (int nCntScore = 0; nCntScore < RANKINGNUM; nCntScore++)
						{
							m_fPosDestX[nCnt][nCntScore] += TEXT_INTEVAL;


							if (m_fPosDestX[nCnt][nCntScore] > SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 + TEXT_INTEVAL * 2)
							{
								m_fPosDestX[nCnt][nCntScore] = SCREEN_WIDTH / 2 - m_pObj2D[VTX_LOGO]->GetSize().x / 2 - TEXT_INTEVAL * 2;

								// 位置取得
								D3DXVECTOR3 pos = m_pScore[nCnt][nCntRanking][nCntScore]->GetObject2D()->GetPosition();

								// 目標位置を代入
								pos.x = m_fPosDestX[nCnt][nCntScore];

								// 位置設定
								m_pScore[nCnt][nCntRanking][nCntScore]->GetObject2D()->SetPosition(pos);
							}
						}
					}
				}

				m_nType = TYPE_LEFT;

				m_bMove = true;
			}


			// 移動処理
			if (m_bMove == true)
			{
				Moving(nCntRanking, nCntPlayer);
			}


			// 値の設定処理
			SetValue(nCntRanking, nCntPlayer);
		}

		if (m_bNewRecord[nCntPlayer] == true)
		{// 記録更新していたら

			// ニューレコードの更新処理
			UpdateNewRecord();
		}
	}

}

//==========================================================================
// 移動処理
//==========================================================================
void CRankingScore::Moving(int nCntRanking, int nCntPlayer)
{
	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
	{
		// 位置取得
		D3DXVECTOR3 pos = m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->GetPosition();

		if (pos.x == INIT_POSX)
		{
			
		}

		float fDest = m_fPosDestX[nCntPlayer][nCntRanking] + nCntScore * BASE_DIS_X;


		// 目標の位置へ補正
		if(m_nType == TYPE_RIGHT)
		{//右へ移動

			if (fDest < pos.x)
			{
				pos.x += (fDest - (PrePos[nCntPlayer].x + nCntScore * BASE_DIS_X)) / (float)TEXT_MOVETIME;
			}

			if (fDest > pos.x)
			{// 目標で固定
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

				//// サウンド再生
				//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_RANKINGSCORE);
			}
		}

		if (m_nType == TYPE_LEFT)
		{//左へ移動

			if (fDest > pos.x)
			{
				pos.x += (fDest - (PrePos[nCntPlayer].x + nCntScore * BASE_DIS_X)) / (float)TEXT_MOVETIME;
			}

			if (fDest < pos.x)
			{// 目標で固定
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

				//// サウンド再生
				//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_RANKINGSCORE);
			}
		}

		// 位置設定
		m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetPosition(pos);
	}
}

//==========================================================================
// ニューレコードの更新処理
//==========================================================================
void CRankingScore::UpdateNewRecord(void)
{
	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
		{
			// 位置取得
			D3DXVECTOR3 pos = m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->GetPosition();

			// サイズ取得
			D3DXVECTOR2 size = m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->GetSize();

			// 色取得
			D3DXCOLOR col = m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->GetColor();

			// 曲線補正
			CuadricCurveComp(col.a, 180, 0.3f, 1.0f, m_nCntNewRecord);

			// 位置設定
			m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->SetPosition(pos);

			// サイズ設定
			m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->SetSize(size);

			// 色設定
			m_pScore[nCntPlayer][m_nIdxNewRecord[nCntPlayer]][nCntScore]->GetObject2D()->SetColor(col);
		}
	}
}
//
////==========================================================================
//// 全ての到着設定
////==========================================================================
//void CRankingScore::SetAllArrival(void)
//{
//
//	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
//	{
//		m_bArrival[nCntScore] = true;	// 到着判定
//	}
//
//	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
//	{
//		for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
//		{
//			// 位置取得
//			D3DXVECTOR3 pos = m_pScore[nCntRanking][nCntScore]->GetObject2D()->GetPosition();
//
//			if (pos.x == INIT_POSX)
//			{
//				// サウンド再生
//				CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_NUMBERMOVE);
//			}
//
//			// 色取得
//			D3DXCOLOR col = m_pScore[nCntRanking][nCntScore]->GetObject2D()->GetColor();
//
//			// 移動
//			float fDest = m_fPosDestX[nCntRanking] + nCntScore * BASE_DIS_X;
//			pos.x = fDest;
//
//			// 不透明度設定
//			col.a = 1.0f;
//
//			// 位置設定
//			m_pScore[nCntRanking][nCntScore]->GetObject2D()->SetPosition(pos);
//
//			// 色設定
//			m_pScore[nCntRanking][nCntScore]->GetObject2D()->SetColor(col);
//		}
//	}
//
//}

//==========================================================================
// 値の設定処理
//==========================================================================
void CRankingScore::SetValue(int nCntRanking, int nCntPlayer)
{
	int aTexU[RANKINGSCORE_DIGIT];
	int nDigit = 1;		// aTexU計算用

	// 計算用割り出し
	for (int nCntDigit = 0; nCntDigit < RANKINGSCORE_DIGIT; nCntDigit++)
	{
		nDigit *= 10;
	}

	// テクスチャ座標に代入する
	
	for (int nCntTex = 0; nCntTex < RANKINGSCORE_DIGIT; nCntTex++)
	{// 桁数分設定

		aTexU[nCntTex] = m_nScore[nCntPlayer][nCntRanking] % nDigit / (nDigit / 10);
		nDigit /= 10;
	}


	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
	{
		if (m_pScore[nCntPlayer][nCntRanking][nCntScore] != NULL)
		{// NULLじゃなかったら

			D3DXVECTOR2 *pTex = m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->GetTex();

			// テクスチャ座標の設定
			pTex[0] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U, 0.0f);
			pTex[1] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U + TEX_U, 0.0f);
			pTex[2] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U, 1.0f);
			pTex[3] = D3DXVECTOR2(aTexU[nCntScore] * TEX_U + TEX_U, 1.0f);

			// 頂点設定
			m_pScore[nCntPlayer][nCntRanking][nCntScore]->GetObject2D()->SetVtx();
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CRankingScore::Draw(void)
{

}

//==============================================================
// ソート処理
//==============================================================
void CRankingScore::Sort(void)
{
	// 降順処理
	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntData = 0; nCntData < m_nNumRanking / RANKINGNUM_PLAYER - 1; nCntData++)
		{
			for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking / RANKINGNUM_PLAYER; nCntSort++)
			{
				if (m_nScore[nCntPlayer][nCntData] < m_nScore[nCntPlayer][nCntSort])
				{// 要素1より要素2が大きかったら

					int nTemp = m_nScore[nCntPlayer][nCntData];
					m_nScore[nCntPlayer][nCntData] = m_nScore[nCntPlayer][nCntSort];
					m_nScore[nCntPlayer][nCntSort] = nTemp;
				}
			}
		}

		// ランキング最大値よりも今回のが小さかったら
		if (m_nScore[m_nNumRanking / RANKINGNUM_PLAYER - 1] < m_nNowScore)
		{// 入れ替え

			m_nScore[nCntPlayer][m_nNumRanking / RANKINGNUM_PLAYER - 1] = m_nNowScore[nCntPlayer];	// 最小値と今回のデータ入れ替え
			m_bNewRecord[nCntPlayer] = true;	// フラグON
		}
	}

	

	// 降順処理
	for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
	{
		for (int nCntData = 0; nCntData < m_nNumRanking / RANKINGNUM_PLAYER - 1; nCntData++)
		{
			for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking / RANKINGNUM_PLAYER; nCntSort++)
			{
				if (m_nScore[nCntPlayer][nCntData] < m_nScore[nCntPlayer][nCntSort])
				{// 要素1より要素2が大きかったら

					int nTemp = m_nScore[nCntPlayer][nCntData];
					m_nScore[nCntPlayer][nCntData] = m_nScore[nCntPlayer][nCntSort];
					m_nScore[nCntPlayer][nCntSort] = nTemp;
				}
			}
		}

		if (m_bNewRecord[nCntPlayer] == true)
		{// ニューレコードの時

			for (int nCount = RANKINGNUM; nCount > 0; nCount--)
			{// 要素番号記憶して抜ける

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
// セーブ処理
//==============================================================
void CRankingScore::Save(void)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(RANKINGFILE, "wb");

	if (pFile != NULL)
	{// ファイルが開けた場合

		// ファイルに数値を書き出す
		fwrite(&m_nNumRanking, sizeof(int), 1, pFile);

		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{
			fwrite(&m_nScore[nCntPlayer][0], sizeof(int), m_nNumRanking / RANKINGNUM_PLAYER, pFile);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}

//==============================================================
// ロード処理
//==============================================================
void CRankingScore::Load(void)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(RANKINGFILE, "rb");

	if (pFile != NULL)
	{// ファイルが開けた場合
		// ファイルに数値を読み込む
		fread(&m_nNumRanking, sizeof(int), 1, pFile);

		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{
			fread(&m_nScore[nCntPlayer][0], sizeof(int), m_nNumRanking / RANKINGNUM_PLAYER, pFile);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けなかった場合

		m_nNumRanking = RANKINGNUM * RANKINGNUM_PLAYER;

		for (int nCntPlayer = 0; nCntPlayer < RANKINGNUM_PLAYER; nCntPlayer++)
		{
			for (int nCount = 0; nCount < m_nNumRanking / RANKINGNUM_PLAYER; nCount++)
			{// 全部ゼロ
				m_nScore[nCntPlayer][nCount] = 150000 - nCount * 25000 + nCntPlayer * 50000;
			}
		}

		// セーブ
		Save();
	}
}