//==========================================
//
//  ロゴのクラス(logo.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================
// 静的メンバ変数宣言
//==========================================
const char* CLogo::m_apTextureFile[] =
{
	"data\\TEXTURE\\title\\title_00.png",
	"data\\TEXTURE\\title\\title_01.png",
	"data\\TEXTURE\\title\\title_02.png",
	"data\\TEXTURE\\title\\title_03.png",
	"data\\TEXTURE\\title\\title_11.png"
};

//==========================================
//  コンストラクタ
//==========================================
CLogo::CLogo()
{
	m_bComplete = false;
}

//==========================================
//  デストラクタ
//==========================================
CLogo::~CLogo()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CLogo::Init(void)
{
	//初期化処理
	HRESULT hr = CObject3D::Init();

	//タイプの設定
	SetType(TYPE_OBJECT3D);

	//サイズを設定
	SetSize(D3DXVECTOR3(24.0f, 6.0f, 0.0f));

	return hr;
}

//==========================================
//  終了処理
//==========================================
void CLogo::Uninit(void)
{
	//終了
	CObject3D::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CLogo::Update(void)
{
	//更新
	CObject3D::Update();
}

//==========================================
//  描画処理
//==========================================
void CLogo::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//描画
	CObject3D::Draw();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
