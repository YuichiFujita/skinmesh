//==========================================
//
//  完成ロゴ(logo_complete.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_complete.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  コンストラクタ
//==========================================
CLogo_Comp::CLogo_Comp()
{

}

//==========================================
//  デストラクタ
//==========================================
CLogo_Comp::~CLogo_Comp()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CLogo_Comp::Init(void)
{
	//初期化処理
	HRESULT hr = CLogo::Init();

	//テクスチャの割り当て
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[3]));

	return hr;
}

//==========================================
//  終了処理
//==========================================
void CLogo_Comp::Uninit(void)
{
	//終了
	CLogo::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CLogo_Comp::Update(void)
{
	//更新
	CLogo::Update();
}

//==========================================
//  描画処理
//==========================================
void CLogo_Comp::Draw(void)
{	
	//描画
	CLogo::Draw();
}

//==========================================
//  生成処理
//==========================================
CLogo_Comp* CLogo_Comp::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//インスタンス生成
	CLogo_Comp *pLogo = DEBUG_NEW CLogo_Comp;

	//初期化処理
	pLogo->Init();

	//値を設定
	pLogo->SetPosition(pos);
	pLogo->SetRotation(rot);

	return pLogo;
}
