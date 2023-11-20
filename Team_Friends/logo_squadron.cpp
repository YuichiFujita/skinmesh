//==========================================
//
//  戦隊のロゴ(logo_squadron.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_squadron.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//==========================================
//  静的メンバ変数宣言
//==========================================
const float CLogo_Sqou::m_fFlashTime = 0.1f;
const int CLogo_Sqou::m_nFlashNum = 2;

//==========================================
//  コンストラクタ
//==========================================
CLogo_Sqou::CLogo_Sqou()
{
	m_nCntFlash = 0;
	m_fTime = 0.0f;

}

//==========================================
//  デストラクタ
//==========================================
CLogo_Sqou::~CLogo_Sqou()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CLogo_Sqou::Init(void)
{
	//初期化処理
	HRESULT hr = CLogo::Init();

	// 透明状態で出現
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	//テクスチャの割り当て
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[2]));

	return hr;
}

//==========================================
//  終了処理
//==========================================
void CLogo_Sqou::Uninit(void)
{
	//終了
	CLogo::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CLogo_Sqou::Update(void)
{
	// 経過時間を取得
	m_fTime += CManager::GetInstance()->GetDeltaTime();

	// 色の変更処理
	if (m_nCntFlash <= m_nFlashNum)
	{
		// 一定時間経過
		if (m_fFlashTime <= m_fTime)
		{
			m_fTime = 0.0f; // 時間のリセット

			if (GetColor().a == 0.0f)
			{
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (GetColor().a == 1.0f)
			{
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				m_nCntFlash++; // 回数を加算
			}
		}
	}
	else if(GetColor().a < 1.0f)
	{
		// 不透明から発色する
		if (m_fTime >= 1.0f)
		{
			m_fTime = 1.0f;
		}
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fTime));
	}
	else
	{
		SetComplete(true);
	}

	//更新
	CLogo::Update();
}

//==========================================
//  描画処理
//==========================================
void CLogo_Sqou::Draw(void)
{
	//描画
	CLogo::Draw();
}

//==========================================
//  生成処理
//==========================================
CLogo_Sqou* CLogo_Sqou::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//インスタンス生成
	CLogo_Sqou* pLogo = DEBUG_NEW CLogo_Sqou;

	//初期化処理
	pLogo->Init();

	//値を設定
	pLogo->SetPosition(pos);
	pLogo->SetRotation(rot);

	return pLogo;
}
