//=============================================================================
// 
//  プレイヤー処理 [player_title.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "game.h"
#include "player_title.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "model.h"
#include "motion.h"
#include "objectChara.h"
#include "shadow.h"

//==========================================
//  定数定義 : 金崎朋弥
//==========================================
namespace
{
	const char* CHARAFILE[CPlayerTitle::MAX] =
	{
		"data\\TEXT\\sample\\motion_union_player.txt", // 合体ファイルパス
		"data\\TEXT\\sample\\motion_ArmToArm.txt", // 腕ファイルパス
		"data\\TEXT\\character\\player\\motion_player.txt", // 足ファイルパス
		"data\\TEXT\\character\\player\\motion_player.txt" // 胴ファイルパス
	};
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerTitle::CPlayerTitle(int nPriority) : CPlayer(nPriority),
m_nModelType(NONE)
{
	// 値のクリア
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerTitle::~CPlayerTitle()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerTitle::Init(void)
{
	// 種類の設定
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// 状態
	m_bLandOld = true;		// 前回の着地状態

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE[m_nModelType]);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// モーションの生成処理
	m_pMotion = CMotion::Create(CHARAFILE[m_nModelType]);

	// オブジェクトキャラクターの情報取得
	CObjectChara *pObjChar = GetObjectChara();

	// モーションの設定
	m_pMotion->SetModel(pObjChar->GetModel(), pObjChar->GetNumModel(), CObjectChara::GetObjectChara());

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// ポーズのリセット
	m_pMotion->ResetPose(MOTION_DEF);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerTitle::Uninit(void)
{
	// 終了処理
	CPlayer::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerTitle::Update(void)
{
	if (IsDeath() == true)
	{
		return;
	}

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// モーション更新
	if (m_pMotion != NULL)
	{
		m_pMotion->Update();
	}

	// 影の位置更新
	if (m_pShadow != NULL)
	{
		// 位置取得
		D3DXVECTOR3 pos = GetPosition();

		m_pShadow->SetPosition(D3DXVECTOR3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerTitle::Draw(void)
{
	// 描画処理
	CPlayer::Draw();
}

//==========================================
//  生成処理 : 金崎朋弥
//==========================================
CPlayerTitle* CPlayerTitle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type)
{
	// インスタンス生成
	CPlayerTitle* pPlayer = DEBUG_NEW CPlayerTitle;

	//モデルを設定
	pPlayer->m_nModelType = (int)type;

	// 初期化処理
	pPlayer->Init();

	// 値を保存
	pPlayer->SetPosition(pos);
	pPlayer->SetRotation(rot);

	return pPlayer;
}
