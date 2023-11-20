//=============================================================================
// 
//  プレイヤー決め画面ヘッダー [decideplayer_screen.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _DECIDEPLAYER_SCREEN_H_
#define _DECIDEPLAYER_SCREEN_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObject2D;

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤー決め画面クラス定義
class CDecidePlayerScreen : public CObject
{
public:

	CDecidePlayerScreen(int nPriority = 8);
	~CDecidePlayerScreen();

	static CDecidePlayerScreen *Create(void);

	// オーバーライドされた関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Delete(void);
private:
	// 列挙型定義
	enum VTX
	{
		VTX_TEXT = 0,	// テキスト
		VTX_MAX
	};

	// 列挙型定義
	enum VTXSELECT
	{
		VTXSELECT_SELECT1P = 0,	// 1P
		VTXSELECT_SELECT2P,		// 2P
		VTXSELECT_SELECT3P,		// 3P
		VTXSELECT_SELECT4P,		// 4P
		VTXSELECT_MAX
	};

	// メンバ関数
	void UpdateSelect(int nCntSelect);	// 選択肢の更新処理

	// メンバ変数
	int m_nCntAlpha;		// 不透明度のカウンター
	int m_nNowSelect;		// 現在の選択肢
	int m_nTexIdx[VTX_MAX];						// テクスチャのインデックス番号
	int m_nTexIdx_Select[VTXSELECT_MAX];						// テクスチャのインデックス番号
	CObject2D *m_pObj2D[VTX_MAX];					// オブジェクト2Dのオブジェクト
	CObject2D *m_pSelect2D[VTXSELECT_MAX];				// 選択肢のオブジェクト
	static const char *m_apTextureFile[VTX_MAX];	// テクスチャのファイル
	static const char *m_apTextureFile_Select[VTXSELECT_MAX];	// テクスチャのファイル
};



#endif