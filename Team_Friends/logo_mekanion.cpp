//==========================================
//
//  メカニオンのロゴ(logo_mekanion.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_mekanion.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "debugproc.h"

//==========================================
//  コンストラクタ
//==========================================
CLogo_Meka::CLogo_Meka()
{
	m_posVirtual = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================
//  デストラクタ
//==========================================
CLogo_Meka::~CLogo_Meka()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CLogo_Meka::Init(void)
{
	//初期化処理
	HRESULT hr = CLogo::Init();

	// 倒した状態を初期値にする
	D3DXVECTOR3 rot = GetRotation(); // 角度を取得
	rot.x = D3DX_PI * -0.5f; // 倒す
	SetRotation(rot);

	//仮想中心の設定
	D3DXVECTOR3 pos = GetPosition(); // 中心座標を取得
	D3DXVECTOR3 size = GetSize(); // ポリゴンの大きさを取得
	m_posVirtual = pos; // 仮想中心を初期化
	m_posVirtual.y = pos.y - size.y; // 座標を補正する

	// 仮想中心に合わせて実際の中心座標を補正する
	pos.z = sinf(rot.x) * size.y + m_posVirtual.z; // z
	pos.y = cosf(rot.x) * size.y + m_posVirtual.y; // y
	SetPosition(pos);  //設定

	//テクスチャの割り当て
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[1]));

	return hr;
}

//==========================================
//  終了処理
//==========================================
void CLogo_Meka::Uninit(void)
{
	//終了
	CLogo::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CLogo_Meka::Update(void)
{
	//回転
	D3DXVECTOR3 rot = GetRotation();
	rot.x += 0.02f;
	if (rot.x >= 0.0f)
	{
		rot.x = 0.0f;
		SetComplete(true);
	}
	SetRotation(rot);

	// 仮想中心に合わせて実際の中心座標を補正する
	D3DXVECTOR3 pos = GetPosition(); // 中心座標を取得
	D3DXVECTOR3 size = GetSize(); // ポリゴンの大きさを取得
	pos.z = sinf(rot.x) * size.y + m_posVirtual.z; // z
	pos.y = cosf(rot.x) * size.y + m_posVirtual.y; // y
	SetPosition(pos);  //設定

#ifdef _DEBUG

	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_6))
	{
		//テクスチャの割り当て
		this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[1]));
	}
	if (pInputKeyboard->GetTrigger(DIK_7))
	{
		//テクスチャの割り当て
		this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[4]));
	}

#endif

	//更新
	CLogo::Update();
}

//==========================================
//  描画処理
//==========================================
void CLogo_Meka::Draw(void)
{
	//描画
	CLogo::Draw();
}

//==========================================
//  生成処理
//==========================================
CLogo_Meka* CLogo_Meka::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//インスタンス生成
	CLogo_Meka* pLogo = DEBUG_NEW CLogo_Meka;

	//値を設定
	pLogo->SetPosition(pos);

	//初期化処理
	pLogo->Init();

	return pLogo;
}
