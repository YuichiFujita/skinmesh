//=============================================================================
// 
//  射出台ヘッダー [injectiontable.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _INJECTIONTABLE_H_
#define _INJECTIONTABLE_H_	// 二重インクルード防止

#include "main.h"
#include "objectX.h"

//==========================================================================
// クラス定義
//==========================================================================
// 射出台クラス定義
class CInjectionTable : public CObjectX
{
public:

	// 列挙型定義
	enum STATE
	{
		STATE_NONE = 0,	// なにもなし
		STATE_CHARGE,	// チャージ
		STATE_UP,		// 上昇
		STATE_MAX
	};

	CInjectionTable(int nPriority = 6);
	~CInjectionTable();

	// オーバーライドされた関数
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	void SetState(STATE state, int nCntState);	// 状態設定
	void SetState(STATE state);					// 状態設定
	STATE GetState(void);	// 状態取得

	static CInjectionTable *Create(const D3DXVECTOR3 pos);	// 生成処理

private:
	void StateNone(void);	// 何もない状態
	void StateCharge(void);	// チャージ状態
	void StateUP(void);		// 上昇状態

	STATE m_state;			// 状態
	int m_nCntState;		// 状態遷移カウンター
	int m_nModelIdx;		// モデルのインデックス番号

	typedef void(CInjectionTable::*STATE_FUNC)(void);
	static STATE_FUNC m_StateFuncList[];
};


#endif