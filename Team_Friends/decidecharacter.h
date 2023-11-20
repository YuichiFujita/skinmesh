//=============================================================================
// 
//  キャラクター決め画面ヘッダー [decidecharacter.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _DECIDECHARACTER_H_
#define _DECIDECHARACTER_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObject2D;
class CCursor;

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// キャラクター決め画面クラス定義
class CDecideCharacter : public CObject
{
public:

	CDecideCharacter(int nPriority = 8);
	~CDecideCharacter();

	static CDecideCharacter *Create(void);

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
	enum VTXCHARACTER
	{
		VTXCHARACTER_1P = 0,	// 1P
		VTXCHARACTER_2P,		// 2P
		VTXCHARACTER_3P,		// 3P
		VTXCHARACTER_4P,		// 4P
		VTXCHARACTER_MAX
	};

	// メンバ関数
	bool CollisionSelect(int nCntSelect);	// 選択肢の更新処理

	// メンバ変数
	int m_nTexIdx[VTX_MAX];						// テクスチャのインデックス番号
	int m_nTexIdx_Select[VTXCHARACTER_MAX];						// テクスチャのインデックス番号
	CObject2D *m_pObj2D[VTX_MAX];					// オブジェクト2Dのオブジェクト
	CObject2D *m_pSelect2D[VTXCHARACTER_MAX];				// 選択肢のオブジェクト
	CCursor *m_apCursor[mylib_const::MAX_PLAYER];	// カーソルのオブジェクト
	bool m_bDecide[mylib_const::MAX_PLAYER];		// 決定したか
	bool m_bAllDecide;								// 全て決定したか
	static const char *m_apTextureFile[VTX_MAX];	// テクスチャのファイル
	static const char *m_apTextureFile_Select[VTXCHARACTER_MAX];	// テクスチャのファイル
};



#endif