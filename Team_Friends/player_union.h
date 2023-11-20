//=============================================================================
// 
//  合体プレイヤーヘッダー [player_union.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_UNION_H_
#define _PLAYER_UNION_H_	// 二重インクルード防止

#include "main.h"
#include "objectChara.h"
#include "motion.h"

//==========================================================================
// マクロ定義
//==========================================================================

// 前方宣言
class CShadow;
class CTargetPoint;
class CHP_GaugePlayer;

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerUnion : public CObject
{
public:

	// 状態定義
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_INVINCIBLE,	// 無敵
		STATE_DMG,			// ダメージ
		STATE_KNOCKBACK,	// ノックバック
		STATE_DEAD,			// 死
		STATE_FADEOUT,		// フェードアウト
		STATE_ATTACK,		// 攻撃処理
		STATE_MAX
	};

	// 合体種類
	enum eType
	{
		TYPE_ALL = 0,	// 全員合体
		TYPE_BODYtoLEG,	// 胴×脚
		TYPE_BODYtoARM,	// 胴×腕
		TYPE_LEGtoARM,	// 脚×腕
		TYPE_ARMtoARM,	// 腕×腕
		TYPE_MAX
	};

	enum eParts
	{
		PARTS_BODY = 0,
		PARTS_LEG,
		PARTS_R_ARM,
		PARTS_L_ARM,
		PARTS_MAX
	};

	CPlayerUnion(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CPlayerUnion();

	// オーバーライドされた関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual bool Hit(const int nValue);	// ヒット処理
	int GetState(void);

	static CPlayerUnion *Create(eType type);	// 生成
	void UninitByMode(void);
	void Kill(void);	// 死亡処理
	void SetPlayerByPartsIdx(int nPartsIdx, int nPlayerIdx);	// プレイヤー毎のパーツインデックス番号設定
	void SetControllMoveIdx(int nIdx);	// 移動の操作するインデックス番号設定

protected:

	// 列挙型定義
	enum MOTION
	{
		MOTION_DEF = 0,			// ニュートラルモーション
		MOTION_WALK,			// 移動モーション
		MOTION_ATK,				// 攻撃
		MOTION_CHARGE,			// チャージ
		MOTION_KNOCKBACK,		// やられモーション
		MOTION_DEAD,			// 死亡モーション
		MOTION_JUMP,			// ジャンプ
		MOTION_FALL,			// 落下中
		MOTION_MAX
	};

	// モーションの判定
	struct SMotionFrag
	{
		bool bJump;			// ジャンプ中かどうか
		bool bATK;			// 攻撃中かどうか
		bool bCharge;		// チャージ中かどうか
		bool bMove;			// 移動中かどうか
	};

	void ReadMultiCharacter(const char *pTextFile);			// 複数キャラクター読み込み
	bool Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &move);	// 当たり判定
	virtual void AttackAction(int nIdx, int nModelNum, CMotion::AttackInfo ATKInfo);	// 攻撃時処理
	virtual void ControllParts(void);	// パーツのコントロール処理
	virtual void MotionSet(int nIdx);	// モーションの設定
	bool ControllMove(int nIdx);	// 移動操作
	virtual HRESULT CreateParts(void);	// パーツの設定

	bool m_bJump;				// ジャンプ中かどうか
	bool m_bLandOld;			// 過去の着地情報
	bool m_bHitStage;			// ステージの当たり判定
	bool m_bLandField;			// フィールドの着地判定
	bool m_bHitWall;			// 壁の当たり判定
	bool m_bKnockBack;			// ノックバック中かどうか
	bool m_bDead;			// 死亡中かどうか

	int m_nPartsIdx[mylib_const::MAX_PLAYER];	// プレイヤー毎のパーツインデックス番号
	int m_nMyPlayerIdx;			// プレイヤーインデックス番号
	int m_nControllMoveIdx;		// 移動操作するやつのインデックス番号
	int m_nCntWalk;				// 歩行カウンター
	int m_nUnionLife;			// 合体時間
	int m_nCntInputAtk;			// 攻撃の入力カウンター

	int m_nInputSuperAtkIdx;	// 必殺技入力のインデックス
	int m_nIntervalAlternately;	// 交互入力のインターバル
	int m_nCntInputSuperATK;	// 必殺技の入力回数

	D3DXCOLOR m_mMatcol;		// マテリアルの色
	STATE m_Oldstate;			// 前回の状態
	STATE m_state;			// 状態
	int m_nCntState;			// 状態遷移カウンター
	D3DXVECTOR3 m_posKnokBack;	// ノックバックの位置
	D3DXVECTOR3 m_KnokBackMove;	// ノックバックの移動量
	CMotion *m_pMotion[PARTS_MAX];			// パーツ分のモーションポインタ
	SMotionFrag m_sMotionFrag[PARTS_MAX];	// モーションのフラグ
	CObjectChara *m_pObjChara[PARTS_MAX];	// パーツ分のオブジェクトキャラクターポインタ
private:

	// コンボ時ワザの種類
	enum eSPSkill
	{
		SKILL_MAX
	};

	// メンバ関数
	void UpdateState(void);	// 状態更新処理
	void KnockBack(void);	// ノックバック
	void Damage(void);		// ダメージ
	void Dead(void);		// 死亡
	void FadeOut(void);		// フェードアウト
	void Invincible(void);	// 無敵
	void Controll(void);		// 操作
	void ControllBody(int nIdx);		// 胴操作
	void ControllLeg(int nIdx);			// 脚操作
	void ControllRightArm(int nIdx);	// 右腕操作
	void ControllLeftArm(int nIdx);		// 左腕操作

	void Atack(int nIdx);		// 攻撃


	int m_nTexIdx;				// テクスチャのインデックス番号
	int m_nIdxXFile;			// Xファイルのインデックス番号
	float m_fRotDest;			// 目標の向き
	CShadow *m_pShadow;			// 影の情報
	CTargetPoint *m_pTargetP;	// 目標の地点
	CHP_GaugePlayer *m_pHPGauge;	// HPゲージの情報
	static bool m_bAllLandInjectionTable;	// 全員の射出台着地判定
	static bool m_bLandInjectionTable[mylib_const::MAX_PLAYER];	// 射出台の着地判定
};



#endif