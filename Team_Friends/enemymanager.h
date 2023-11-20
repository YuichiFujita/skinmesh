//=============================================================================
// 
//  敵のマネージャヘッダー [enemymanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"

// 前方宣言
class CEnemy;

//==========================================================================
// クラス定義
//==========================================================================
// 敵のマネージャクラス定義
class CEnemyManager
{
public:

	// 構造体定義
	struct EnemyData
	{
		int nType;	// キャラクター種類
		int nStartFrame;	// 初期フレーム
		D3DXVECTOR3 pos;		// 位置
	};

	struct Pattern
	{
		int nNumEnemy;	// 敵の数
		int nFixedType;	// 一定の動きの種類
		EnemyData EnemyData[mylib_const::MAX_PATTEN_ENEMY];
	};

	// 列挙型定義
	enum STATE
	{
		STATE_NONE = 0,		// 何もない状態
		STATE_COMBOANIM,	// コンボ時のアニメーション
		STATE_MAX
	};

	CEnemyManager();
	~CEnemyManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Kill(void);		// 破棄

	static CEnemyManager *Create(const std::string pTextFile);
	HRESULT ReadText(const std::string pTextFile);	// 外部ファイル読み込み処理
	void SetStageEnemy(void);	// ステージ毎の敵配置
	CEnemy **SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPattern);	// 敵配置
	int GetPatternNum(void);
	void Release(int nIdx);		// 破棄


	bool IsChangeStage(void) { return m_bChangeStage; }	// ステージ変更中か
	void SetEnableChangeStage(bool bChange) { m_bChangeStage = bChange; }	// ステージ変更の状態切り替え
	STATE GetState(void) { return m_state; }	// 状態取得
	int GetNumAll(void);		// 敵の総数取得
	Pattern GetPattern(int nPattern);	// パターン取得
	CEnemy **GetEnemy(void);	// 敵取得
	const char *GetMotionFilename(int nType);
protected:


private:

	CEnemy *m_pEnemy[mylib_const::MAX_OBJ];						// 敵へのポインタ
	Pattern m_aPattern[mylib_const::MAX_PATTEN_ENEMY];			// 配置の種類
	std::string sMotionFileName[mylib_const::MAX_PATTEN_ENEMY];	// モーションファイル名
	STATE m_state;			// 状態
	int m_nCntSpawn;		// 出現カウント
	int m_nPatternNum;		// 出現パターン数
	int m_nNumChara;		// 敵の種類の総数
	int m_nNumAll;			// 敵の総数
	bool m_bChangeStage;	// ステージ変更中か
};



#endif