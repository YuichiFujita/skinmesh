//=============================================================================
// 
//  ランキングマネージャヘッダー [rankingmanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RANKINGMANAGER_H_
#define _RANKINGMANAGER_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define RANKINGNUM_PLAYER	(5) // プレイヤーの数

// 前方宣言

//==========================================================================
// クラス定義
//==========================================================================
// ランキングマネージャクラス定義
class CRankingManager
{
public:

	CRankingManager();
	~CRankingManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void Reset(void);				// リセット
	void SetNowScore(int nScore, int nPlayer);	// 今回のスコア設定
	int GetNowScore(int nPlayer);			// 今回のスコア取得
	static CRankingManager *Create(void);	// 生成処理
	
private:
	int m_nNowScore[RANKINGNUM_PLAYER];	// 今回のスコア
};



#endif