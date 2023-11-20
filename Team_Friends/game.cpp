//=============================================================================
// 
//  ゲーム処理 [game.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "fade.h"
#include "camera.h"

#include "input.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "map.h"
#include "elevation.h"
#include "sound.h"
#include "edit_enemybase.h"
#include "bulletmanager.h"
#include "stage.h"
#include "compactcore.h"
#include "statuswindow.h"

#include "enemymanager.h"
#include "player.h"
#include "player_union.h"
#include "enemybase.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CScore *CGame::m_pScore = NULL;					// スコアのオブジェクト
CBulletManager *CGame::m_pBulletManager = NULL;		// 弾マネージャのオブジェクト
CEditEnemyBase *CGame::m_pEditEnemyBase = NULL;		// 敵の拠点エディター
CStage *CGame::m_pStage = NULL;						// ステージのオブジェクト
CGameManager *CGame::m_pGameManager = NULL;			// ゲームマネージャのオブジェクト
CGame::EEditType CGame::m_EditType = EDITTYPE_OFF;		// エディットの種類
CEnemyBase *CGame::m_pEnemyBase = NULL;	// 敵の拠点
CEnemyManager *CGame::m_pEnemyManager = NULL;	// 敵マネージャのオブジェクト
CStatusWindow *CGame::m_pStatusWindow[CGameManager::STATUS_MAX] = {};	// ステータスウィンドウのオブジェクト
bool CGame::m_bEdit = false;				// エディットの判定

//==========================================================================
// コンストラクタ
//==========================================================================
CGame::CGame()
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CGame::~CGame()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGame::Init(void)
{
	// エディット判定OFF
	m_bEdit = false;

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	//**********************************
	// ゲームマネージャ
	//**********************************
	m_pGameManager = CGameManager::Create();

	//**********************************
	// 敵マネージャ
	//**********************************
	m_pEnemyManager = CEnemyManager::Create("data\\TEXT\\enemydata\\manager.txt");

	if (m_pEnemyManager == NULL)
	{// NULLだったら
		return E_FAIL;
	}

	//**********************************
	// 敵の拠点
	//**********************************
	m_pEnemyBase = CEnemyBase::Create("data\\TEXT\\enemydata\\base.txt");
	if (m_pEnemyBase == NULL)
	{// NULLだったら
		return E_FAIL;
	}

	//**********************************
	// プレイヤー
	//**********************************
	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetNumPlayer(); nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer != NULL)
		{
			pPlayer->SetPosition(D3DXVECTOR3(-500.0f + nCntPlayer * 50.0f, 1000.0f, -1000.0f));
			pPlayer->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		bool bJoin = true;
		if (CManager::GetInstance()->GetNumPlayer() <= nCntPlayer)
		{
			bJoin = false;
		}
		m_pStatusWindow[nCntPlayer] = CStatusWindow::Create(D3DXVECTOR3(160.0f + nCntPlayer * 320.0f, 600.0f, 0.0f), bJoin);
	}

	//**********************************
	// 弾マネージャ
	//**********************************
	m_pBulletManager = CBulletManager::Create();

	// ステージ
	m_pStage = CStage::Create("data\\TEXT\\stage\\info.txt");

	// スコアの生成処理
	m_pScore = CScore::Create(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));

	CManager::GetInstance()->GetCamera()->Reset(CScene::MODE_GAME);

	// BGM再生
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_GAME);

	// 合体後プレイヤー生成
	//CPlayerUnion::Create(CPlayerUnion::TYPE_ALL);

	CCompactCore::Create(D3DXVECTOR3(500.0f, 400.0f, 0.0f));

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGame::Uninit(void)
{

	// スコアの破棄
	if (m_pScore != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pScore->Uninit();

		// メモリの開放
		delete m_pScore;
		m_pScore = NULL;
	}

	if (m_pBulletManager != NULL)
	{
		// 終了させる
		m_pBulletManager->Uninit();
		delete m_pBulletManager;
		m_pBulletManager = NULL;
	}

	if (m_pEditEnemyBase != NULL)
	{
		// 終了させる
		m_pEditEnemyBase->Uninit();
		delete m_pEditEnemyBase;
		m_pEditEnemyBase = NULL;
	}

	// ステージの破棄
	if (m_pStage != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = NULL;
	}

	if (m_pGameManager != NULL)
	{
		// 終了処理
		m_pGameManager->Uninit();
		delete m_pGameManager;
		m_pGameManager = NULL;
	}

	// 敵マネージャ
	if (m_pEnemyManager != NULL)
	{
		m_pEnemyManager->Uninit();
		delete m_pEnemyManager;
		m_pEnemyManager = NULL;
	}

	// 敵の拠点
	if (m_pEnemyBase != NULL)
	{
		m_pEnemyBase->Uninit();
		delete m_pEnemyBase;
		m_pEnemyBase = NULL;
	}


	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGame::Update(void)
{
	// ゲームマネージャ
	if (m_pGameManager != NULL)
	{
		// 更新処理
		m_pGameManager->Update();
	}

	CManager::GetInstance()->GetDebugProc()->Print(
		"現在のモード：【ゲーム】\n"
		"切り替え：【 F 】\n\n");

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

#if 1
	if (CManager::GetInstance()->GetEdit() == NULL &&
		m_pEnemyManager != NULL &&
		GetEnemyManager()->GetState() != CEnemyManager::STATE_COMBOANIM)
	{
		// スコアの更新処理
		m_pScore->Update();
	}
#endif

	if (pInputKeyboard->GetTrigger(DIK_F4))
	{// F4でエディット切り替え

		// 切り替え
		m_EditType = (EEditType)(((int)m_EditType + 1) % (int)EDITTYPE_MAX);	// 追従の種類

		// リセット
		EditReset();

		// エディット判定ON
		m_bEdit = true;

		switch (m_EditType)
		{
		case CGame::EDITTYPE_OFF:	// 全部オフ
			// エディット判定OFF
			m_bEdit = false;
			break;

		case EDITTYPE_ENEMYBASE:
			if (m_pEditEnemyBase == NULL)
			{// NULLだったら

				// エディットの生成処理
				m_pEditEnemyBase = CEditEnemyBase::Create();
			}
			break;

		}
	}

	if (GetEnemyManager() != NULL)
	{// 敵マネージャの更新処理
		GetEnemyManager()->Update();
	}

	if (m_pEditEnemyBase != NULL)
	{// 敵の拠点エディターの更新処理
		m_pEditEnemyBase->Update();
	}

	// 敵の拠点
	if (m_pEnemyBase != NULL)
	{
		m_pEnemyBase->Update();
	}

	// ステージの更新
	if (m_pStage != NULL)
	{
		m_pStage->Update();
	}

#if _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_F))
	{
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);
	}

	if (pInputKeyboard->GetTrigger(DIK_I))
	{
		CCompactCore::Create(D3DXVECTOR3(500.0f, 200.0f, 0.0f));
	}
#endif

	// シーンの更新
	CScene::Update();

}

//==========================================================================
// 描画処理
//==========================================================================
void CGame::Draw(void)
{

}

//==========================================================================
// スコアの取得
//==========================================================================
CScore *CGame::GetScore(void)
{
	return m_pScore;
}

//==========================================================================
// 弾マネージャの取得
//==========================================================================
CBulletManager *CGame::GetBulletManager(void)
{
	return m_pBulletManager;
}

//==========================================================================
// ステージの取得
//==========================================================================
CStage *CGame::GetStage(void)
{
	return m_pStage;
}

//==========================================================================
// ゲームマネージャの取得
//==========================================================================
CGameManager *CGame::GetGameManager(void)
{
	return m_pGameManager;
}


//==========================================================================
// 敵マネージャの取得
//==========================================================================
CEnemyManager *CGame::GetEnemyManager(void)
{
	return m_pEnemyManager;
}

//==========================================================================
// 敵の拠点
//==========================================================================
CEnemyBase *CGame::GetEnemyBase(void)
{
	return m_pEnemyBase;
}

//==========================================================================
// ステータスウィンドウ
//==========================================================================
CStatusWindow *CGame::GetStatusWindow(int nIdx)
{
	return m_pStatusWindow[nIdx];
}

//==========================================================================
// リセット処理
//==========================================================================
void CGame::Reset(void)
{
	// ステージの破棄
	if (m_pStage != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pStage->Release();
		delete m_pStage;
		m_pStage = NULL;
	}

	// 敵の拠点
	if (m_pEnemyBase != NULL)
	{
		m_pEnemyBase->Uninit();
		delete m_pEnemyBase;
		m_pEnemyBase = NULL;
	}

	// 敵マネージャ
	if (m_pEnemyManager != NULL)
	{
		m_pEnemyManager->Kill();
	}

	// ステージ
	m_pStage = CStage::Create("data\\TEXT\\stage\\boss_info.txt");

	//**********************************
	// 敵の拠点
	//**********************************
	m_pEnemyBase = CEnemyBase::Create("data\\TEXT\\enemydata\\base_boss.txt");
	if (m_pEnemyBase == NULL)
	{// NULLだったら
		return;
	}
}

//==========================================================================
// エディターリセット処理
//==========================================================================
void CGame::EditReset(void)
{
	
	if (m_pEditEnemyBase != NULL)
	{
		// 終了させる
		m_pEditEnemyBase->Release();
		m_pEditEnemyBase->Uninit();
		delete m_pEditEnemyBase;
		m_pEditEnemyBase = NULL;
	}

}
