//=============================================================================
// 
//  敵のマネージャ処理 [enemymanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "debugproc.h"
#include "enemymanager.h"
#include "calculation.h"
#include "manager.h"
#include "game.h"
#include "gamemanager.h"
#include "renderer.h"
#include "enemy.h"
#include "particle.h"
#include "fade.h"
#include "elevation.h"
#include "player.h"
#include "impactwave.h"
#include "camera.h"
#include "sound.h"
#include "enemybase.h"
#include "effect_enemyspawn.h"
#include "instantfade.h"
#include "stagecleartext.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define SPAWN_STRENGTH	(100.0f)	// スポーン時の高さ
#define KILL_INT		(30)		// 倒したインターバル

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CEnemyManager::CEnemyManager()
{
	// 値のクリア
	memset(&m_pEnemy[0], 0, sizeof(m_pEnemy));		// 敵へのポインタ
	memset(&m_aPattern[0], 0, sizeof(m_aPattern));	// 配置の種類
	m_state = STATE_NONE;	// 状態
	m_nCntSpawn = 0;		// 出現カウント
	m_nPatternNum = 0;		// 出現パターン数
	m_nNumChara = 0;		// 敵の種類の総数
	m_nNumAll = 0;			// 敵の総数
	m_bChangeStage = false;	// ステージ変更中か
}

//==========================================================================
// デストラクタ
//==========================================================================
CEnemyManager::~CEnemyManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CEnemyManager *CEnemyManager::Create(const std::string pTextFile)
{
	// 生成用のオブジェクト
	CEnemyManager *pModel = NULL;

	if (pModel == NULL)
	{// NULLだったら

		// メモリの確保
		pModel = DEBUG_NEW CEnemyManager;

		if (pModel != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pModel->ReadText(pTextFile);
			if (FAILED(hr))
			{// 失敗していたら
				return NULL;
			}

			// 初期化処理
			hr = pModel->Init();

			if (FAILED(hr))
			{// 失敗していたら
				return NULL;
			}
		}

		return pModel;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEnemyManager::Init(void)
{
	// 総数リセット
	m_nNumAll = 0;
	m_nCntSpawn = 0;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEnemyManager::Uninit(void)
{
	for (int nCntEnemy = 0; nCntEnemy < mylib_const::MAX_OBJ; nCntEnemy++)
	{
		if (m_pEnemy[nCntEnemy] != NULL)
		{
			m_pEnemy[nCntEnemy] = NULL;
		}
	}
}

//==========================================================================
// 破棄
//==========================================================================
void CEnemyManager::Release(int nIdx)
{
	if (m_pEnemy[nIdx] != NULL)
	{
		m_pEnemy[nIdx] = NULL;
	}

	// 総数減算
	m_nNumAll--;
}

//==========================================================================
// 破棄
//==========================================================================
void CEnemyManager::Kill(void)
{
	for (int nCntEnemy = 0; nCntEnemy < mylib_const::MAX_OBJ; nCntEnemy++)
	{
		if (m_pEnemy[nCntEnemy] != NULL)
		{
			m_pEnemy[nCntEnemy]->Uninit();
			m_pEnemy[nCntEnemy] = NULL;
		}
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CEnemyManager::Update(void)
{
	if (m_nNumAll <= 0 && m_bChangeStage == false)
	{// 全員倒されたら

		// ステージ変更中にする
		m_bChangeStage = true;

		// 通常クリア状態にする
		CGame::GetGameManager()->SetType(CGameManager::SCENE_MAINCLEAR);

		// 遷移なしフェード追加
		CManager::GetInstance()->GetInstantFade()->SetFade();

		// 遷移状態に変更
		CGame::GetGameManager()->SetType(CGameManager::SCENE_TRANSITION);

		CStageClearText::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

		//// 遷移なしフェード追加
		//CManager::GetInstance()->GetInstantFade()->SetFade();
	}

	

	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- 敵情報 ----------------\n"
		"【残り人数】[%d]\n", m_nNumAll);
}

//==========================================================================
// ステージ毎の敵配置
//==========================================================================
void CEnemyManager::SetStageEnemy(void)
{
	// ゲームマネージャ取得
	CGameManager *pGameManager = CGame::GetGameManager();
	if (pGameManager == NULL || pGameManager->IsEndNormalStage() == true)
	{
		return;
	}

	// ステージの総数取得
	int nNumStage = pGameManager->GetNumStage();
	int nNowStage = pGameManager->GetNowStage();

	if (nNumStage <= nNowStage)
	{
		return;
	}

	// 敵拠点データ取得
	CEnemyBase *pEnemyBase = CGame::GetEnemyBase();
	if (pEnemyBase == NULL)
	{
		return;
	}

	// 拠点の数取得
	int nNumBase = pEnemyBase->GetNumBase(nNowStage);

	for (int i = 0; i < nNumBase; i++)
	{
		// 拠点ごとのデータ取得
		CEnemyBase::sInfo sEnemyBaseInfo = pEnemyBase->GetEnemyBaseInfo(nNowStage, i);

		// 敵の配置
		SetEnemy(sEnemyBaseInfo.pos, sEnemyBaseInfo.rot, sEnemyBaseInfo.nPattern);
	}

	// ステージ加算
	pGameManager->AddNowStage();
}

//==========================================================================
// 敵配置
//==========================================================================
CEnemy **CEnemyManager::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPattern)
{
	int nNumSpawn = m_aPattern[nPattern].nNumEnemy;	// スポーンする数
	int nCntNULL = 0;
	int nCntStart = 0;
	Pattern NowPattern = m_aPattern[nPattern];
	CEnemy *pEnemy[mylib_const::MAX_PATTEN_ENEMY] = {};
	memset(&pEnemy[0], NULL, sizeof(pEnemy));

	for (int nCntEnemy = 0; nCntEnemy < nNumSpawn; nCntEnemy++)
	{
		for (nCntNULL = nCntStart; nCntNULL < mylib_const::MAX_OBJ; nCntNULL++, nCntStart++)
		{
			if (m_pEnemy[nCntNULL] != NULL)
			{// 情報が入ってたら
				continue;
			}

			int nType = NowPattern.EnemyData[nCntEnemy].nType;


			// 計算用マトリックス
			D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

			// マトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// 敵拠点の向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// パターン内の位置を反映する
			D3DXMatrixTranslation(&mtxTrans, NowPattern.EnemyData[nCntEnemy].pos.x, NowPattern.EnemyData[nCntEnemy].pos.y, NowPattern.EnemyData[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			// スポーン時の向きを掛け合わせる
			D3DXVECTOR3 spawnPos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

			// 拠点の位置分加算
			spawnPos += pos;


			// 敵の生成
			m_pEnemy[nCntNULL] = CEnemy::Create(
				nCntNULL,						// インデックス番号
				sMotionFileName[nType].c_str(),	// ファイル名
				spawnPos,						// 位置
				(CEnemy::TYPE)nType);			// 種類

			if (m_pEnemy[nCntNULL] == NULL)
			{// 失敗していたら

				delete m_pEnemy[nCntNULL];
				m_pEnemy[nCntNULL] = NULL;
				break;
			}

			// ポインタコピー
			pEnemy[nCntEnemy] = m_pEnemy[nCntNULL];

			// 向き設定
			m_pEnemy[nCntNULL]->SetRotation(rot);
			m_pEnemy[nCntNULL]->SetRotDest(rot.y);

			// 総数加算
			m_nNumAll++;
			break;
		}
	}

	return &pEnemy[0];
}

//==========================================================================
// パターン数
//==========================================================================
int CEnemyManager::GetPatternNum(void)
{
	return m_nPatternNum;
}

//==========================================================================
// パターン取得
//==========================================================================
CEnemyManager::Pattern CEnemyManager::GetPattern(int nPattern)
{
	return m_aPattern[nPattern];
}

//==========================================================================
// 敵の総数取得
//==========================================================================
int CEnemyManager::GetNumAll(void)
{
	return m_nNumAll;
}

//==========================================================================
// テキスト読み込み処理
//==========================================================================
HRESULT CEnemyManager::ReadText(const std::string pTextFile)
{

	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pTextFile.c_str(), "r");

	if (pFile == NULL)
	{//ファイルが開けた場合
		return E_FAIL;
	}

	char aComment[MAX_COMMENT];	// コメント
	int nType = 0;				// 配置する種類
	int nCntPatten = 0;			// パターンのカウント
	int nCntFileName = 0;

	memset(&m_aPattern[0], NULL, sizeof(m_aPattern));	// 読み込みデータ
	m_nPatternNum = 0;
	m_nNumChara = 0;

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// キャラクター数の設定
		if (strcmp(aComment, "NUM_CHARACTER") == 0)
		{// NUM_CHARACTERがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &m_nNumChara);	// キャラクター数
		}

		while (nCntFileName != m_nNumChara)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(aComment, "MOTION_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// ファイル名保存
				sMotionFileName[nCntFileName] = aComment;

				nCntFileName++;	// ファイル数加算
			}
		}

		// 各パターンの設定
		if (strcmp(aComment, "PATTERNSET") == 0)
		{// 配置情報の読み込みを開始

			int nCntEnemy = 0;			// 敵の配置カウント

			while (strcmp(aComment, "END_PATTERNSET") != 0)
			{// END_PATTERNSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	//確認する

				if (strcmp(aComment, "FIXEDMOVE") == 0)
				{// FIXEDMOVEが来たら一定の動きの種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &m_aPattern[nCntPatten].nFixedType);	// 一定の動きの種類
				}

				if (strcmp(aComment, "ENEMYSET") == 0)
				{// ENEMYSETで敵情報の読み込み開始

					while (strcmp(aComment, "END_ENEMYSET") != 0)
					{// END_ENEMYSETが来るまで繰り返す

						fscanf(pFile, "%s", &aComment[0]);	// 確認する

						if (strcmp(aComment, "TYPE") == 0)
						{// TYPEが来たらキャラファイル番号読み込み

							fscanf(pFile, "%s", &aComment[0]);	// =の分
							fscanf(pFile, "%d", &m_aPattern[nCntPatten].EnemyData[nCntEnemy].nType);	// キャラファイル番号
						}

						if (strcmp(aComment, "POS") == 0)
						{// POSが来たら位置読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%f", &m_aPattern[nCntPatten].EnemyData[nCntEnemy].pos.x);	// X座標
							fscanf(pFile, "%f", &m_aPattern[nCntPatten].EnemyData[nCntEnemy].pos.y);	// Y座標
							fscanf(pFile, "%f", &m_aPattern[nCntPatten].EnemyData[nCntEnemy].pos.z);	// Z座標
						}

						if (strcmp(aComment, "STARTFRAME") == 0)
						{// STARTFRAMEが来たら初期フレーム読み込み

							fscanf(pFile, "%s", &aComment[0]);	// =の分
							fscanf(pFile, "%d", &m_aPattern[nCntPatten].EnemyData[nCntEnemy].nStartFrame);	// 初期フレーム
						}

					}// END_ENEMYSETのかっこ

					nCntEnemy++;	// 敵のカウントを加算
					m_aPattern[nCntPatten].nNumEnemy++;	// 敵のカウントを加算
				}
			}// END_PATTERNSETのかっこ

			nCntPatten++;	// パターン加算
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// パターン数
	m_nPatternNum = nCntPatten;

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// 敵取得
//==========================================================================
CEnemy **CEnemyManager::GetEnemy(void)
{
	return &m_pEnemy[0];
}

//==========================================================================
// 敵のモーションファイル名取得
//==========================================================================
const char *CEnemyManager::GetMotionFilename(int nType)
{
	return &sMotionFileName[nType][0];
}
