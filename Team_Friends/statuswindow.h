//=============================================================================
// 
//  ステータスウィンドウヘッダー [statuswindow.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STATUSWINDOW_H_
#define _STATUSWINDOW_H_	// 二重インクルード防止

#include "object.h"
#include "gamemanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectCircleGauge2D;
class CMultiNumber;

//==========================================================================
// クラス定義
//==========================================================================
// ステータスウィンドウクラス定義
class CStatusWindow : public CObject
{
public:

	CStatusWindow(int nPriority = 7);
	~CStatusWindow();

	// オーバーライドされた関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CObjectCircleGauge2D *GetGauge(CGameManager::eStatus status);	// ゲージ取得
	static CStatusWindow *Create(D3DXVECTOR3 pos, bool bJoin);
private:
	
	bool m_bJoin;	// 参加してるか
	CObject2D *m_apOnOffSign[2];							// オンオフサイン
	CObject2D *m_apWindowShape[CGameManager::STATUS_MAX];	// ウィンドウの型
	CObjectCircleGauge2D *m_pCircleGauge2D[CGameManager::STATUS_MAX];	// 円ゲージのポインタ
	CMultiNumber *m_pStatusNumber[CGameManager::STATUS_MAX];			// ステータスの数字
};



#endif