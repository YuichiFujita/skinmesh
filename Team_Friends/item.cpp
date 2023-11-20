//=============================================================================
// 
//  アイテム処理 [item.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "item.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "explosion.h"
#include "3D_effect.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"
#include "game.h"
#include "player.h"
#include "impactwave.h"
#include "scene.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TIME_FADEOUT	(40)	// フェードアウト時間

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CItem::m_apModelFile[CGameManager::STATUS_MAX] =	// テクスチャのファイル
{
	"data\\MODEL\\item_power.x",	// 火力
	"data\\MODEL\\item_speed.x",	// 駆動性
	"data\\MODEL\\item_life.x",	// 耐久力
};
int CItem::m_nNumAll = 0;		// アイテムの総数

//==========================================================================
// 関数ポインタ
//==========================================================================
CItem::STATE_FUNC CItem::m_StateFuncList[] =
{
	&CItem::StateNone,
	&CItem::StateDamage,
	&CItem::StateFadeOut,
};

//==========================================================================
// コンストラクタ
//==========================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_type = CGameManager::STATUS_POWER;	// 種類
	m_state = STATE_NONE;			// 状態
	m_nCntState = 0;			// 状態遷移カウンター
	m_nLife = 0;
	m_nIdxBulletManager = 0;			// アイテムマネージャのインデックス番号
	m_nCntEmission = 0;	// 発生物のカウンター

	// テクスチャデータの配列分繰り返す
	m_nModelIdx = 0;		// テクスチャのインデックス番号

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CItem::~CItem()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CItem *CItem::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 生成用のオブジェクト
	CItem *pBullet = NULL;

	if (pBullet == NULL)
	{// NULLだったら

		// メモリの確保
		pBullet = DEBUG_NEW CItem;

		if (pBullet != NULL)
		{// メモリの確保が出来ていたら

			// 位置割り当て
			pBullet->SetPosition(pos);

			// 位置割り当て
			pBullet->SetRotation(rot);

			// 初期化処理
			pBullet->Init();
		}

		return pBullet;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CItem::Init(void)
{
	HRESULT hr;

	// 各種変数の初期化
	m_nLifeMax = 60 * 30;
	m_nLife = m_nLifeMax;	// 寿命

	// 種類
	m_type = (CGameManager::eStatus)Random(0, (int)CGameManager::STATUS_MAX - 1);

	// 初期化処理
	hr = CObjectX::Init(m_apModelFile[m_type]);
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}
	// 種類の設定
	CObject::SetType(TYPE_BULLET);

	// ランダム移動量
	SetMove(D3DXVECTOR3(Random(-50, 50) * 0.1f, 10.0f, Random(-50, 50) * 0.1f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CItem::Uninit(void)
{
	//// 削除
	//if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME && CGame::GetBulletManager() != NULL)
	//{// アイテムマネージャの削除
	//	CGame::GetBulletManager()->Delete(m_nIdxBulletManager);
	//}

	// 終了処理
	CObjectX::Uninit();

	// 総数減算
	m_nNumAll--;
}

//==========================================================================
// 更新処理
//==========================================================================
void CItem::Update(void)
{
	// 位置更新
	UpdatePos();

	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();

	// 当たり判定処理
	CollisionPlayer();

	if (IsDeath() == true)
	{// 死亡フラグが立っていたら
		return;
	}

	// 寿命減算
	m_nLife--;

	if (m_nLife <= 0 && m_state != STATE_FADEOUT)
	{// 寿命が尽きたら
		
		// フェードアウト状態
		m_state = STATE_FADEOUT;
		m_nCntState = TIME_FADEOUT;
	}

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 移動処理
//==========================================================================
void CItem::UpdatePos(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 位置更新
	pos += move;

	// 重力
	move.y -= mylib_const::GRAVITY;

	if (CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos))
	{// 地面と接触していたら
		move *= 0.8f;
		move.y *= 0.6f;
		move.y *= -1;
	}

	// 慣性補正
	move += (mylib_const::DEFAULT_VECTOR3 - move) * 0.01f;

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// 通常状態処理
//==========================================================================
void CItem::StateNone(void)
{
	// 状態遷移カウンター減算
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
		m_state = STATE_NONE;
	}
}

//==========================================================================
// ダメージ状態処理
//==========================================================================
void CItem::StateDamage(void)
{
	// 状態遷移カウンター減算
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
		m_state = STATE_NONE;
	}
}

//==========================================================================
// フェードアウト処理
//==========================================================================
void CItem::StateFadeOut(void)
{
	// 色取得
	D3DXCOLOR col = GetColor();

	// 状態遷移カウンター減算
	m_nCntState--;

	// 不透明度更新
	col.a = (float)m_nCntState / (float)TIME_FADEOUT;

	// 色設定
	SetColor(col);

	if (m_nCntState <= 0)
	{
		m_nCntState = 0;

		// 終了処理
		Uninit();
		return;
	}
}

//==========================================================================
// プレイヤーとの判定
//==========================================================================
void CItem::CollisionPlayer(void)
{
	// プレイヤー情報取得
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{// NULLだったら
			continue;
		}

		// プレイヤーの情報取得
		D3DXVECTOR3 PlayerPosition = pPlayer->GetPosition();
		//D3DXVECTOR3 PlayerPosition = pPlayer->GetCenterPosition();
		D3DXVECTOR3 PlayerRotation = pPlayer->GetRotation();
		float fPlayerRadius = pPlayer->GetRadius();

		// 情報取得
		D3DXVECTOR3 pos = GetPosition();
		float fRadius = GetVtxMax().x;

		if (SphereRange(pos, PlayerPosition, fRadius, fPlayerRadius))
		{// 当たっていたら

			// ステータス付与
			pPlayer->GiveStatus(m_type);

			// パーティクル生成
			my_particle::Create(pos, my_particle::TYPE_ENEMY_FADE);

			// 終了処理
			Uninit();
			continue;
		}
	}
}

//==========================================================================
// プレイヤーアイテムの更新
//==========================================================================
void CItem::UpdateTypePlayer(void)
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CItem::Draw(void)
{
	// 描画
	CObjectX::Draw(GetColor().a);
}

//==========================================================================
// 状態設定
//==========================================================================
void CItem::SetState(STATE state, int nCntState)
{
	m_state = state;			// 状態
	m_nCntState = nCntState;	// 状態遷移カウンター
}

//==========================================================================
// 状態取得
//==========================================================================
CItem::STATE CItem::GetState(void)
{
	return m_state;
}

//==========================================================================
// 総数取得
//==========================================================================
int CItem::GetNumAll(void)
{
	return m_nNumAll;
}
