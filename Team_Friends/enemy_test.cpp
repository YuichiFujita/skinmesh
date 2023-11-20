//==========================================
//
//  敵の試験(enemy_test.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "enemy_test.h"
#include "player.h"
#include "manager.h"
#include "calculation.h"

//==========================================
//  コンストラクタ
//==========================================
CEnemyTest::CEnemyTest(int nPriority)
{

}

//==========================================
//  デストラクタ
//==========================================
CEnemyTest::~CEnemyTest()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CEnemyTest::Init(void)
{
	//初期化処理
	CEnemy::Init();

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CEnemyTest::Uninit(void)
{
	// 終了処理
	CEnemy::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CEnemyTest::Update(void)
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
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(m_nTargetPlayerIndex);
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

//==========================================
//  描画処理
//==========================================
void CEnemyTest::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//==========================================
//  殺す
//==========================================
void CEnemyTest::Kill(void)
{
	// 死亡処理
	CEnemy::Kill();
}

//==========================================
//  モーションセット
//==========================================
void CEnemyTest::MotionSet(void)
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
