//=============================================================================
// 
// ステータスウィンドウの下地処理 [statuswindow_base.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STATUSWINDOW_BASE_H_
#define _STATUSWINDOW_BASE_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// ステータスウィンドウの下地クラス定義
class CStatusWindow_Base : public CObject2D
{
public:

	CStatusWindow_Base(int nPriority = 6);
	~CStatusWindow_Base();

	
	//  オーバーライドされた関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStatusWindow_Base *Create(const D3DXVECTOR3 pos);

private:

};

#endif