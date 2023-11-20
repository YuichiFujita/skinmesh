//=============================================================================
// 
//  アイテムヘッダー [item.h]
//  Author : 髙田佳依
// 
//=============================================================================

#ifndef _ITEM_H_
#define _ITEM_H_	// 二重インクルード防止

#include "objectX.h"
#include "gamemanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// アイテムクラス定義
class CItem : public CObjectX
{
public:

	// 状態列挙型定義
	enum STATE
	{
		STATE_NONE = 0,	// なにもなし
		STATE_DMG,		// 跳ね返し
		STATE_FADEOUT,	// フェードアウト状態
		STATE_MAX		// メモ：フェードとスポーン時が必要
	};

	CItem(int nPriority = 6);
	~CItem();


	// オーバーライドされた関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetState(STATE state, int nCntState);	// 状態設定
	STATE GetState(void);	// 状態取得

	static int GetNumAll(void);
	static CItem *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

private:
	void UpdatePos(void);		// 移動
	void UpdateTypePlayer(void);	// プレイヤー弾の更新
	void CollisionPlayer(void);		// プレイヤーとの判定
	
	void StateNone(void);		// 何もない状態
	void StateDamage(void);		// ダメージ状態処理
	void StateFadeOut(void);		// フェードアウト処理


	CGameManager::eStatus m_type;				// 種類
	STATE m_state;				// 状態
	int m_nCntState;			// 状態遷移カウンター
	int m_nIdxBulletManager;			// 弾マネージャのインデックス番号
	int m_nLife;							// 寿命
	int m_nLifeMax;					// 寿命の最大値
	static int m_nNumAll;					// 弾の総数
	static const char *m_apModelFile[CGameManager::STATUS_MAX];	// モデルのファイル
	int m_nModelIdx;				// モデルのインデックス番号
	int m_nCntEmission;	// 発生物のカウンター

	typedef void(CItem::*STATE_FUNC)(void);
	static STATE_FUNC m_StateFuncList[];
};


#endif