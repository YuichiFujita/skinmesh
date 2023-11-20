//==========================================================================
//
// リストマネージャヘッダー[listmanager.h]
// Author：相馬靜雅
//
//==========================================================================
#ifndef _LISTMANAGER_H_
#define _LISTMANAGER_H_	// 二重インクルード防止

#include "list.h"
#include "object.h"

//==========================================================================
// リスト構造クラスの定義
//==========================================================================
class CListManager
{
public:

	CListManager();			// コンストラクタ
	~CListManager();		// デストラクタ

	void ReleaseAll(void);	// 全ての開放処理
	void Push(CList *pObj);			// オブジェクト追加
	void Pop(CList *pObj);			// オブジェクトを取り出す
	CList *GetTop(void);			// 先頭のオブジェクト取得
	static CListManager *Create(void);	// 生成処理
private:

	CList *m_pTop;	// 先頭のオブジェクトへのポインタ
	CList *m_pCur;	// 最後尾のオブジェクトへのポインタ
	int m_nNumAll;	// 総数
};




#endif