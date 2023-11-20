//=============================================================================
// 
//  コンパクトコアヘッダー [compactcore.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COMPACTCORE_H_
#define _COMPACTCORE_H_	// 二重インクルード防止

#include "objectX.h"

//==========================================================================
// クラス定義
//==========================================================================
// コンパクトコアクラス定義
class CCompactCore : public CObjectX
{
public:

	// 列挙型定義
	enum STATE
	{
		STATE_NONE = 0,	// なにもなし
		STATE_GET,		// 獲得
		STATE_FADEOUT,	// フェードアウト状態
		STATE_MAX
	};

	CCompactCore(int nPriority = 6);
	~CCompactCore();

	// オーバーライドされた関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetState(STATE state, int nCntState);	// 状態設定
	STATE GetState(void);	// 状態取得

	static int GetNumAll(void);
	static CCompactCore *Create(const D3DXVECTOR3 pos);

private:
	void UpdatePos(void);			// 移動
	void CollisionPlayer(void);		// プレイヤーとの判定
	
	void StateNone(void);		// 何もない状態
	void StateGet(void);		// 取得状態
	void StateFadeOut(void);	// フェードアウト処理


	STATE m_state;				// 状態
	int m_nCntState;			// 状態遷移カウンター
	int m_nLife;				// 寿命
	int m_nLifeMax;				// 寿命の最大値
	static int m_nNumAll;		// 弾の総数
	int m_nCntEmission;			// 発生物のカウンター
	int m_nNumGetPlayer;		// 取得してるプレイヤーの数

	typedef void(CCompactCore::*STATE_FUNC)(void);
	static STATE_FUNC m_StateFuncList[];
};


#endif