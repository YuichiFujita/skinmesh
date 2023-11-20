//==========================================
//
//  MECHANIONのロゴ(logo_mechanion.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_mechanion.h"
#include "manager.h"
#include "texture.h"
#include "input.h"

//==========================================
//  コンストラクタ
//==========================================
CLogo_Mech::CLogo_Mech()
{

}

//==========================================
//  デストラクタ
//==========================================
CLogo_Mech::~CLogo_Mech()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CLogo_Mech::Init(void)
{
	//初期化処理
	HRESULT hr = CLogo::Init();

	//テクスチャの割り当て
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[0]));

	//完成状態で生成する
	SetComplete(true);

	return hr;
}

//==========================================
//  終了処理
//==========================================
void CLogo_Mech::Uninit(void)
{
	//終了
	CLogo::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CLogo_Mech::Update(void)
{
	//更新
	CLogo::Update();
}

//==========================================
//  描画処理
//==========================================
void CLogo_Mech::Draw(void)
{
	//描画
	CLogo::Draw();
}

//==========================================
//  生成処理
//==========================================
CLogo_Mech* CLogo_Mech::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//インスタンス生成
	CLogo_Mech* pLogo = DEBUG_NEW CLogo_Mech;

	//初期化処理
	pLogo->Init();

	//値を設定
	pLogo->SetPosition(pos);
	pLogo->SetRotation(rot);

	return pLogo;
}
