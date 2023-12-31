//=============================================================================
// 
//  群体敵処理 [enemy_crowd.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "enemy_crowd.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"
#include "input.h"
#include "calculation.h"
#include "bullet.h"
#include "explosion.h"
#include "particle.h"
#include "motion.h"
#include "model.h"
#include "elevation.h"
#include "player.h"
#include "enemymanager.h"
#include "camera.h"
#include "ballast.h"
#include "3D_Effect.h"
#include "hp_gauge.h"
#include "shadow.h"
#include "sound.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define PLAYER_SERCH	(800.0f)	// プレイヤー探索範囲
#define CHACE_DISTABCE	(50.0f)		// 追い掛ける時の間隔
#define JUMP			(18.0f)		// ジャンプ力初期値

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CEnemyCrowd::CEnemyCrowd(int nPriority) : CEnemy(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CEnemyCrowd::~CEnemyCrowd()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEnemyCrowd::Init(void)
{
	// 初期化処理
	CEnemy::Init();

	// 体力取得
	int nLife = GetLife();

	//// 体力ゲージ
	//m_pHPGauge = CHP_Gauge::Create(80.0f, nLife, 0.8f);
	m_state = STATE_SPAWN;	// 親追い掛け状態
	m_Oldstate = STATE_PLAYERCHASE;
	m_ActType = ACTTYPE_CHASE;
	//if (m_pHPGauge == NULL)
	//{// NULLだったら
	//	m_pHPGauge = NULL;
	//}

	
	// 生存時間
	m_nSurvivalLifeOrigin = m_nSurvivalLife;
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEnemyCrowd::Uninit(void)
{
	// 終了処理
	CEnemy::Uninit();
}

//==========================================================================
// 死亡処理
//==========================================================================
void CEnemyCrowd::Kill(void)
{
	// 死亡処理
	CEnemy::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CEnemyCrowd::Update(void)
{
	// 死亡の判定
	if (IsDeath() == true)
	{// 死亡フラグが立っていたら
		return;
	}

	// 更新処理
	CEnemy::Update();

	if (IsDeath() == true)
	{// 死亡フラグが立っていたら
		return;
	}

}

//==========================================================================
// 着地時の処理
//==========================================================================
void CEnemyCrowd::ProcessLanding(void)
{
	// 着地時の処理
	CEnemy::ProcessLanding();
}

//==========================================================================
// 攻撃状態移行処理
//==========================================================================
void CEnemyCrowd::ChangeToAttackState(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// プレイヤー情報
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{
			return;
		}

		// 親の位置取得
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		float fRadius = 500.0f;

		if (CircleRange3D(pos, posPlayer, fRadius, pPlayer->GetRadius()) == true && m_sMotionFrag.bJump == false)
		{// 一定距離間にプレイヤーが入ったら

			// 攻撃状態にする
			m_state = STATE_ATTACK;
			m_sMotionFrag.bATK = true;

			// 追い掛けるプレイヤーの番号設定
			m_nTargetPlayerIndex = nCntPlayer;
			break;
		}
	}
}

//==========================================================================
// 出現
//==========================================================================
void CEnemyCrowd::Spawn(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 状態遷移カウンター減算
	m_nCntState++;

	// 位置更新
	pos = (D3DXVECTOR3(0.0f, SPAWN_GRAVITY, 0.0f) * (float)(m_nCntState * m_nCntState) + move * (float)m_nCntState) + m_posOrigin;

	// 起伏との判定
	if (CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos))
	{// 地面と当たっていたら
		m_state = m_Oldstate;	// 過去の情報保存
		m_nCntState = 0;	// 遷移カウンター設定
		move.y = 0.0f;			// 移動量ゼロ
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// 攻撃処理
//==========================================================================
void CEnemyCrowd::StateAttack(void)
{
	// 攻撃処理
	CEnemy::StateAttack();

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());

	// 攻撃情報の総数取得
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	bool bAtkWait = true;	// 攻撃待機中
	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == NULL)
		{// NULLだったら
			continue;
		}

		// モーションカウンター取得
		if (m_pMotion->GetAllCount() > aInfo.AttackInfo[nCntAttack]->nMinCnt)
		{// 攻撃判定中

			// 攻撃判定中にする
			bAtkWait = false;
		}
	}

	if (bAtkWait == false)
	{// 判定中の時
		return;
	}


	// プレイヤー情報
	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);
	if (pPlayer == NULL)
	{// NULLだったら
		return;
	}

	// 親の位置取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 目標の向き取得
	float fRotDest = GetRotDest();

	// 目標の角度を求める
	fRotDest = atan2f((pos.x - posPlayer.x), (pos.z - posPlayer.z));

	// 目標との差分
	float fRotDiff = fRotDest - rot.y;

	//角度の正規化
	RotNormalize(fRotDiff);

	//角度の補正をする
	rot.y += fRotDiff * 0.025f;

	// 角度の正規化
	RotNormalize(rot.y);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// 追い掛け移動
//==========================================================================
void CEnemyCrowd::ChaseMove(float fMove)
{
	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 移動量加算
	move.x += sinf(D3DX_PI + rot.y) * fMove;
	move.z += cosf(D3DX_PI + rot.y) * fMove;

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// モーションの設定
//==========================================================================
void CEnemyCrowd::MotionSet(void)
{
	if (m_pMotion->IsFinish() == true)
	{// 終了していたら

		// 現在の種類取得
		int nType = m_pMotion->GetType();

		if (m_sMotionFrag.bMove == true && m_sMotionFrag.bKnockback == false)
		{// 移動していたら

			m_sMotionFrag.bMove = false;	// 移動判定OFF

			// 移動モーション
			m_pMotion->Set(MOTION_WALK);
		}
		else if (m_sMotionFrag.bKnockback == true)
		{// やられ中だったら

			// やられモーション
			m_pMotion->Set(MOTION_KNOCKBACK);
		}
		else if (m_sMotionFrag.bATK == true)
		{// 攻撃していたら

			m_sMotionFrag.bATK = false;		// 攻撃判定OFF

			// 攻撃モーション
			m_pMotion->Set(MOTION_ATK);
		}
		else
		{
			// ニュートラルモーション
			m_pMotion->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CEnemyCrowd::AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo)
{
	D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

	// 攻撃時処理
	CEnemy::AttackAction(nModelNum, ATKInfo);

	if (GetObjectChara()->GetModel()[nModelNum] == NULL)
	{// NULLだったら
		return;
	}

	// 判定するパーツのマトリックス取得
	D3DXMATRIX mtxWepon = GetObjectChara()->GetModel()[nModelNum]->GetWorldMtx();

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, ATKInfo.Offset.x, ATKInfo.Offset.y, ATKInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	// 武器の位置
	D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);

	if (pPlayer == NULL)
	{// NULLだったら
		return;
	}

	// プレイヤーの位置
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();


	// 目標の角度を求める
	float fRotDest = atan2f((weponpos.x - posPlayer.x), (weponpos.z - posPlayer.z));

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 位置取得
	D3DXVECTOR3 rot = GetRotation();

	// 弾の生成
	CObject *pBullet = CBullet::Create(
		CBullet::TYPE_ENEMY,
		CBullet::MOVETYPE_NORMAL,
		D3DXVECTOR3(pos.x, 50.0f, pos.z),
		rot,
		D3DXVECTOR3(3.0f, 0.0f, 0.0f),
		40.0f);

	
}

//==========================================================================
// 描画処理
//==========================================================================
void CEnemyCrowd::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//==========================================================================
// 敵の情報取得
//==========================================================================
CEnemyCrowd *CEnemyCrowd::GetEnemy(void)
{
	// 自分自身のポインタを取得
	return this;
}