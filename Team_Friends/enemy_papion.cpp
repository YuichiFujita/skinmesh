//=============================================================================
// 
//  飛行敵処理 [enemy_papion.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "enemy_papion.h"
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
#include "3D_Effect.h"
#include "hp_gauge.h"
#include "shadow.h"
#include "sound.h"

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CEnemyPapion::CEnemyPapion(int nPriority) : CEnemy(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CEnemyPapion::~CEnemyPapion()
{

}


//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEnemyPapion::Init(void)
{
	// 初期化処理
	CEnemy::Init();

	m_state = STATE_NONE;	// 親追い掛け状態
	m_Oldstate = STATE_NONE;
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEnemyPapion::Uninit(void)
{
	// 終了処理
	CEnemy::Uninit();
}

//==========================================================================
// 死亡処理
//==========================================================================
void CEnemyPapion::Kill(void)
{
	// 死亡処理
	CEnemy::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CEnemyPapion::Update(void)
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

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// 目標の向き取得
	float fRotDest = GetRotDest();

	// プレイヤー情報
	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);
	if (pPlayer == NULL)
	{
		return;
	}

	// 親の位置取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

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

	// 目標の向き設定
	SetRotDest(fRotDest);

}

//==========================================================================
// 着地時の処理
//==========================================================================
void CEnemyPapion::ProcessLanding(void)
{
	// 着地時の処理
	CEnemy::ProcessLanding();
}

//==========================================================================
// 攻撃状態移行処理
//==========================================================================
void CEnemyPapion::ChangeToAttackState(void)
{
	return;

}

//==========================================================================
// 出現
//==========================================================================
void CEnemyPapion::Spawn(void)
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
void CEnemyPapion::StateAttack(void)
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


	//// プレイヤー情報
	//CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	//if (pPlayer == NULL)
	//{// NULLだったら
	//	return;
	//}

	//// 親の位置取得
	//D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	//// 位置取得
	//D3DXVECTOR3 pos = GetPosition();

	//// 向き取得
	//D3DXVECTOR3 rot = GetRotation();

	//// 目標の向き取得
	//float fRotDest = GetRotDest();

	//// 目標の角度を求める
	//fRotDest = atan2f((pos.x - posPlayer.x), (pos.z - posPlayer.z));

	//// 目標との差分
	//float fRotDiff = fRotDest - rot.y;

	////角度の正規化
	//RotNormalize(fRotDiff);

	////角度の補正をする
	//rot.y += fRotDiff * 0.025f;

	//// 角度の正規化
	//RotNormalize(rot.y);

	//// 向き設定
	//SetRotation(rot);
}

//==========================================================================
// 追い掛け移動
//==========================================================================
void CEnemyPapion::ChaseMove(float fMove)
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
void CEnemyPapion::MotionSet(void)
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
void CEnemyPapion::AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo)
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

	// プレイヤーと敵のベクトル
	D3DXVECTOR3 vec = weponpos - posPlayer;

	// 目標の角度を求める
	float fRotDest = atan2f(vec.x, vec.z);
	float fRotHeight = atan2f((weponpos.y - posPlayer.y - 100.0f), D3DXVec3Length(&vec));

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 位置取得
	D3DXVECTOR3 rot = GetRotation();

	// 弾の生成
	CObject *pBullet = CBullet::Create(
		CBullet::TYPE_ENEMY,
		CBullet::MOVETYPE_NORMAL,
		D3DXVECTOR3(pos.x, pos.y, pos.z),
		rot,
		D3DXVECTOR3(fabsf(sinf(D3DX_PI * 0.5f + fRotHeight) * 5.0f), cosf(D3DX_PI * 0.5f + fRotHeight) * 5.0f, 0.0f),
		40.0f);
}

//==========================================================================
// 描画処理
//==========================================================================
void CEnemyPapion::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//==========================================================================
// 敵の情報取得
//==========================================================================
CEnemyPapion *CEnemyPapion::GetEnemy(void)
{
	// 自分自身のポインタを取得
	return this;
}