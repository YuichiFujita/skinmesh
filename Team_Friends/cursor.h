//=============================================================================
// 
// カーソル処理 [cursor.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CURSOR_H_
#define _CURSOR_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// カーソルクラス定義
class CCursor : public CObject2D
{
public:

	CCursor(int nPriority = 8);
	~CCursor();

	static CCursor *Create(const D3DXVECTOR3 pos, int nIdx);
	
	//  オーバーライドされた関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	void SetEnbaleDicide(bool bDecide) { m_bDecide = bDecide; }	// 決定状態設定
	bool IsDecide(void) { return m_bDecide; }					// 決定状態取得
	void SetSelectIdx(int nIdx) { m_nSelectIdx = nIdx; }		// 選択肢の番号設定
	int GetSelectIdx(void) { return m_nSelectIdx; }				// 選択肢の番号取得

private:

	int m_nMyPlayerIdx;		// プレイヤーインデックス番号
	int m_nTexIdx;			// テクスチャのインデックス番号
	int m_nSelectIdx;		// 選択肢のインデックス番号
	bool m_bDecide;			// 決まったかどうか
	static const char *m_apTextureFile[mylib_const::MAX_PLAYER];	// テクスチャのファイル
};

#endif