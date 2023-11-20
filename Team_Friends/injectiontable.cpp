//=============================================================================
// 
//  射出台処理 [injectiontable.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "injectiontable.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "scene.h"
#include "instantfade.h"
#include "camera.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TIME_CHARGE	(120)	// チャージ時間
#define TIME_UP		(60)	// 上昇時間
#define MODELFILE	"data\\MODEL\\movefllor.x"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// 関数ポインタ
//==========================================================================
CInjectionTable::STATE_FUNC CInjectionTable::m_StateFuncList[] =
{
	&CInjectionTable::StateNone,
	&CInjectionTable::StateCharge,
	&CInjectionTable::StateUP,
};

//==========================================================================
// コンストラクタ
//==========================================================================
CInjectionTable::CInjectionTable(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;	// 状態
	m_nCntState = 0;		// 状態遷移カウンター
	m_nModelIdx = 0;		// モデルのインデックス番号
}

//==========================================================================
// デストラクタ
//==========================================================================
CInjectionTable::~CInjectionTable()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CInjectionTable *CInjectionTable::Create(const D3DXVECTOR3 pos)
{
	// 生成用のオブジェクト
	CInjectionTable *pTable = NULL;

	if (pTable == NULL)
	{// NULLだったら

		// メモリの確保
		pTable = DEBUG_NEW CInjectionTable;

		if (pTable != NULL)
		{// メモリの確保が出来ていたら

			// 位置割り当て
			pTable->SetPosition(pos);
			pTable->SetOriginPosition(pos);

			// 初期化処理
			pTable->Init();
		}

		return pTable;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CInjectionTable::Init(void)
{
	HRESULT hr;

	// 初期化処理
	hr = CObjectX::Init(MODELFILE);
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 種類の設定
	CObject::SetType(TYPE_STAGE);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CInjectionTable::Uninit(void)
{
	
	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CInjectionTable::Update(void)
{
	if (IsDeath() == true)
	{// 死亡フラグが立っていたら
		return;
	}

	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// 通常状態処理
//==========================================================================
void CInjectionTable::StateNone(void)
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
// チャージ状態処理
//==========================================================================
void CInjectionTable::StateCharge(void)
{
	// 状態遷移カウンター減算
	m_nCntState++;
	if (m_nCntState >= TIME_CHARGE)
	{
		m_nCntState = TIME_UP;
		m_state = STATE_UP;

		// カメラ取得
		CCamera *pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetEnableFollow(false);
	}
}

//==========================================================================
// 上昇状態
//==========================================================================
void CInjectionTable::StateUP(void)
{
	// 遷移状態に変更
	CGame::GetGameManager()->SetType(CGameManager::SCENE_TRANSITION);

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 位置更新
	pos.y += 10.0f;

	// 位置設定
	SetPosition(pos);

	// 状態遷移カウンター減算
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
		m_state = STATE_NONE;

		// 遷移なしフェード追加
		CManager::GetInstance()->GetInstantFade()->SetFade();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CInjectionTable::Draw(void)
{
	// 描画
	CObjectX::Draw(GetColor().a);
}

//==========================================================================
// 状態設定
//==========================================================================
void CInjectionTable::SetState(STATE state, int nCntState)
{
	m_state = state;			// 状態
	m_nCntState = nCntState;	// 状態遷移カウンター
}

//==========================================================================
// 状態設定(オーバーロード)
//==========================================================================
void CInjectionTable::SetState(STATE state)
{
	m_state = state;			// 状態
}

//==========================================================================
// 状態取得
//==========================================================================
CInjectionTable::STATE CInjectionTable::GetState(void)
{
	return m_state;
}
