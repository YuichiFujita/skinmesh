//=============================================================================
// 
//  ゲームマネージャヘッダー [gamemanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// ゲームマネージャクラス定義
class CGameManager
{
public:

	// シーンの種類
	enum SceneType
	{
		SCENE_MAIN = 0,			// メイン
		SCENE_MAINCLEAR,		// メインクリア
		SCENE_RUSH,				// ラッシュ
		SCENE_BOSS,				// ボス
		SCENE_TRANSITIONWAIT,	// 遷移待機
		SCENE_TRANSITION,		// 遷移
		SCENE_MAX
	};

	// ステータス項目
	enum eStatus
	{
		STATUS_POWER = 0,	// 火力
		STATUS_SPEED,		// 駆動性
		STATUS_LIFE,		// 耐久
		STATUS_MAX
	};

	CGameManager();
	~CGameManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetType(SceneType type);	// シーンの種類設定
	SceneType GetType(void);		// シーンの種類取得
	void SetEnableRush(void) { m_bEndRush = true; }		// ラッシュの終了フラグを有効にする
	bool IsRushEnd(void) { return m_bEndRush; }			// 終了のフラグ取得
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// 操作フラグを切り替えする
	bool IsControll(void) { return m_bControll; }				// 操作のフラグ取得
	bool IsEndNormalStage(void) { return m_bEndNormalStage; }	// 通常ステージのア終了判定
	void AddNowStage(void);	// ステージの加算
	int GetNowStage(void);	// 現在のステージ取得
	void SetNumStage(int nStage) { m_nNumStage = nStage; }	// ステージの総数設定
	int GetNumStage(void) { return m_nNumStage; }			// ステージの総数取得

	static CGameManager *Create(void);	// 生成処理

private:

	// メンバ関数
	void SetBoss(void);
	void SetEnemy(void);

	// メンバ変数
	SceneType m_SceneType;	// シーンの種類
	bool m_bEndRush;		// ラッシュが終了したか
	bool m_bControll;		// 操作できるか
	bool m_bEndNormalStage;	// 通常ステージが終了したか
	int m_nNowStage;		// 現在のステージ
	int m_nNumStage;		// ステージの総数
};



#endif