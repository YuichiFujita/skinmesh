//=============================================================================
// 
//  コンパクトコア処理 [compactcore.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "compactcore.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "3D_effect.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"
#include "game.h"
#include "player.h"
#include "player_union.h"
#include "impactwave.h"
#include "scene.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TIME_FADEOUT	(40)	// フェードアウト時間
#define TIME_GET		(120)	// 取得時間
#define MODELFILE		"data\\MODEL\\compactcore_01.x"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CCompactCore::m_nNumAll = 0;		// アイテムの総数

//==========================================================================
// 関数ポインタ
//==========================================================================
CCompactCore::STATE_FUNC CCompactCore::m_StateFuncList[] =
{
	&CCompactCore::StateNone,
	&CCompactCore::StateGet,
	&CCompactCore::StateFadeOut,
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCompactCore::CCompactCore(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;		// 状態
	m_nCntState = 0;			// 状態遷移カウンター
	m_nLife = 0;				// 寿命
	m_nLifeMax = 0;				// 寿命の最大値
	m_nCntEmission = 0;			// 発生物のカウンター
	m_nNumGetPlayer = 0;		// 取得してるプレイヤーの数

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CCompactCore::~CCompactCore()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CCompactCore *CCompactCore::Create(const D3DXVECTOR3 pos)
{
	// 生成用のオブジェクト
	CCompactCore *pBullet = NULL;

	if (pBullet == NULL)
	{// NULLだったら

		// メモリの確保
		pBullet = DEBUG_NEW CCompactCore;

		if (pBullet != NULL)
		{// メモリの確保が出来ていたら

			// 位置割り当て
			pBullet->SetPosition(pos);

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
HRESULT CCompactCore::Init(void)
{
	HRESULT hr;

	// 各種変数の初期化
	m_nLifeMax = 60 * 30;
	m_nLife = m_nLifeMax;	// 寿命

	// 初期化処理
	hr = CObjectX::Init(MODELFILE);
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}
	// 種類の設定
	CObject::SetType(TYPE_BULLET);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCompactCore::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();

	// 総数減算
	m_nNumAll--;
}

//==========================================================================
// 更新処理
//==========================================================================
void CCompactCore::Update(void)
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
	//m_nLife--;

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
void CCompactCore::UpdatePos(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	if (CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) == false)
	{// 地面と接触していたら

		// 位置更新
		pos.y += -mylib_const::GRAVITY;
	}

	// 位置設定
	SetPosition(pos);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// 通常状態処理
//==========================================================================
void CCompactCore::StateNone(void)
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
// 取得状態処理
//==========================================================================
void CCompactCore::StateGet(void)
{
	// 状態遷移カウンター減算
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;

		// 終了処理
		Uninit();
		return;
	}
}

//==========================================================================
// フェードアウト処理
//==========================================================================
void CCompactCore::StateFadeOut(void)
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
void CCompactCore::CollisionPlayer(void)
{
	if (m_state == STATE_GET)
	{
		return;
	}

	// 取得してるプレイヤーの数リセット
	m_nNumGetPlayer = 0;

	// 取得したプレイヤーのインデックス
	int nGetPlayerIdx[mylib_const::MAX_PLAYER];

	// プレイヤー情報取得
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL || 
			pPlayer->GetState() == CPlayer::STATE_COMPACTUNION || pPlayer->GetState() == CPlayer::STATE_RELEASEUNION)
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

			// パーティクル生成
			my_particle::Create(pos, my_particle::TYPE_ENEMY_FADE);

			// インデックス保存
			nGetPlayerIdx[m_nNumGetPlayer] = nCntPlayer;

			// 取得人数加算
			m_nNumGetPlayer++;

			if (m_nNumGetPlayer >= 2)
			{
				// 取得状態にする
				m_state = STATE_GET;
				m_nCntState = TIME_GET;

				// プレイヤーを簡易合体状態に設定
				int nParent = nGetPlayerIdx[0];
				int nExcept = nGetPlayerIdx[1];

				CPlayer **ppPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
				ppPlayer[nParent]->SetState(CPlayer::STATE_COMPACTUNION);
				ppPlayer[nExcept]->SetState(CPlayer::STATE_COMPACTUNION);
				

				// 各インデックスの種類取得
				int nParentType = CManager::GetInstance()->GetByPlayerPartsType(nParent);
				int nExceptType = CManager::GetInstance()->GetByPlayerPartsType(nExcept);

				CPlayerUnion *pPlayerUnion = NULL;

				// 種類取得してその種類に該当する合体先を生成する
				if (nParentType == CPlayerUnion::PARTS_LEG ||
					nExceptType == CPlayerUnion::PARTS_LEG)
				{// 脚があるやつ

					if ((nParentType == CPlayerUnion::PARTS_LEG && nExceptType == CPlayerUnion::PARTS_BODY) ||
						(nParentType == CPlayerUnion::PARTS_BODY && nExceptType == CPlayerUnion::PARTS_LEG))
					{
						// 体と脚
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_BODYtoLEG);
					}
					else if ((nParentType == CPlayerUnion::PARTS_LEG && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)) ||
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && nExceptType == CPlayerUnion::PARTS_LEG))
					{
						// 腕と脚
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_LEGtoARM);
					}

					int nControllIdx = nParentType;
					if (nExceptType == CPlayerUnion::PARTS_LEG)
					{
						nControllIdx = nExceptType;
					}
					pPlayerUnion->SetControllMoveIdx(nParent);
				}
				else
				{// 脚なしでの合体

					if ((nParentType == CPlayerUnion::PARTS_BODY && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)) ||
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && nExceptType == CPlayerUnion::PARTS_BODY))
					{
						// 体と腕
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_BODYtoARM);
					}
					else if (
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)) ||
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)))
					{
						// 腕と腕
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_ARMtoARM);
					}
					pPlayerUnion->SetControllMoveIdx(nParent);
				}

				// プレイヤー毎のパーツインデックス番号
				pPlayerUnion->SetPlayerByPartsIdx(0, nParent);
				pPlayerUnion->SetPlayerByPartsIdx(1, nExcept);
				pPlayerUnion->SetPosition(pos);
				return;
			}
			continue;
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCompactCore::Draw(void)
{
	// 描画
	CObjectX::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CCompactCore::SetState(STATE state, int nCntState)
{
	m_state = state;			// 状態
	m_nCntState = nCntState;	// 状態遷移カウンター
}

//==========================================================================
// 状態取得
//==========================================================================
CCompactCore::STATE CCompactCore::GetState(void)
{
	return m_state;
}

//==========================================================================
// 総数取得
//==========================================================================
int CCompactCore::GetNumAll(void)
{
	return m_nNumAll;
}
