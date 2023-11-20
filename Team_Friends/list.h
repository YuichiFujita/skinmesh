//==========================================================================
//
// リスト構造ヘッダー[list.h]
// Author：相馬靜雅
//
//==========================================================================
#ifndef _LIST_H_
#define _LIST_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// リスト構造クラスの定義
//==========================================================================
class CList : public CObject
{
public:

	CList();	// コンストラクタ
	~CList();	// デストラクタ

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Release(void);	// 開放処理

	void SetNext(CList* pNext);	// 次のオブジェクト設定
	CList *GetNext(void);		// 次のオブジェクト取得
	void SetPrev(CList* pPrev);	// 前のオブジェクト設定
	CList *GetPrev(void);		// 前のオブジェクト取得
	bool IsDeath(void);			// 死亡判定取得

	static CList *Create(void);	// 生成処理
private:

	CList *m_pPrev;		// 前のオブジェクトへのポインタ
	CList *m_pNext;		// 次のオブジェクトへのポインタ
	bool m_bDeath;		// 死亡フラグ
};




#endif