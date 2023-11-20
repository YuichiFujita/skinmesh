//=============================================================================
// 
//  タイトル処理 [title.cpp]
//  Author : 相馬靜雅
//  Added by 金崎朋弥
// 
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "sound.h"
#include "title_logo.h"
#include "fog.h"
#include "player_title.h"
#include "enemy.h"

//==========================================
//  定数定義 金崎
//==========================================
namespace
{
	const D3DXCOLOR TARGET_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const float START_LENGTH = 300.0f; // 初期距離
	const float END_LENGTH = 3000.0f; // 目標距離
	const float FUNCTION = 0.01f; //倍率
}

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle::CTitle() :
m_col(D3DXCOLOR(0.0f, 0.0, 0.0f, 1.0f)),
m_fLength(START_LENGTH)
{
	// 値のクリア
	m_nCntSwitch = 0;		// 切り替えのカウンター
	m_pLogo = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitle::~CTitle()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitle::Init(void)
{
	// BGM再生
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_TITLE);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	//タイトルロゴの表示
	if (m_pLogo == nullptr)
	{
		m_pLogo = CTitleLogo::Create();
	}

	// 煙をかける
	Fog::Set(true);

	// フォグの値を設定する
	Fog::SetStart(START_LENGTH);
	Fog::SetEnd(m_fLength);
	Fog::SetCol(m_col);

	// プレイヤーを置いてみる
	CPlayerTitle::Create(D3DXVECTOR3(0.0f, 100.0f, -3000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_UNION);
	CPlayerTitle::Create(D3DXVECTOR3(160.0f, 0.0f, -2900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_ARM);
	CPlayerTitle::Create(D3DXVECTOR3(-160.0f, 0.0f, -2900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_ARM);
	CPlayerTitle::Create(D3DXVECTOR3(240.0f, 0.0f, -3200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_LEG);
	CPlayerTitle::Create(D3DXVECTOR3(-240.0f, 0.0f, -3200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayerTitle::PLAYER_BODY);

	// 敵置いてみる
	CEnemy::Create(0, "data\\TEXT\\motion_set_player.txt", D3DXVECTOR3(0.0f, 0.0f, -3000.0f), CEnemy::TYPE_TEST);

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle::Uninit(void)
{
	//煙を払う
	Fog::Set(false);

	// タイトルロゴを破棄
	if (m_pLogo != nullptr)
	{
		m_pLogo = nullptr;
	}

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle::Update(void)
{
	CManager::GetInstance()->GetDebugProc()->Print(
		"現在のモード：【タイトル】\n"
		"切り替え：【 F 】\n\n");

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// 切り替えのカウンター加算
	m_nCntSwitch++;

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	if (m_nCntSwitch <= 120)
	{
		return;
	}

	//タイトルロゴが完成していないときは抜ける
	if (!m_pLogo->GetComplete())
	{
		return;
	}

	//フォグを引く
	WhiteOut();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0) == true)
	{
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_DECIDEPLAYER);
	}

	if (m_nCntSwitch >= 60 * 40)
	{// 自動遷移
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RANKING);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitle::Draw(void)
{

}

//==========================================
//  フォグを引く処理
//==========================================
void CTitle::WhiteOut()
{
	// 目標距離まで引き伸ばす
	m_fLength += (END_LENGTH - m_fLength) * FUNCTION;

	// 目標色まで補正する
	m_col += (TARGET_COLOR - m_col) * FUNCTION;

	// 距離を適用する
	Fog::SetEnd(m_fLength);

	// 色を適用する
	Fog::SetCol(m_col);

#ifdef _DEBUG
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	if (pInputKeyboard->GetTrigger(DIK_1))
	{
		// fog消し
		Fog::Set(false);
	}
	if (pInputKeyboard->GetTrigger(DIK_2))
	{
		// fog焚き
		Fog::Set(true);
	}

	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_DECIDEPLAYER);
	}

	if (pInputKeyboard->GetTrigger(DIK_4))
	{// 自動遷移
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RANKING);
	}
#endif
}
