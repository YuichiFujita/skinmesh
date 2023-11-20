//=============================================================================
// 
// ステータスウィンドウの下地処理 [statuswindow_base.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "statuswindow_base.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CStatusWindow_Base::CStatusWindow_Base(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CStatusWindow_Base::~CStatusWindow_Base()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStatusWindow_Base *CStatusWindow_Base::Create(const D3DXVECTOR3 pos)
{
	// 生成用のオブジェクト
	CStatusWindow_Base *pEffect = NULL;

	if (pEffect == NULL)
	{// NULLだったら

		// メモリの確保
		pEffect = DEBUG_NEW CStatusWindow_Base;

		if (pEffect != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pEffect->Init();
			pEffect->SetPosition(pos);
		}

		return pEffect;
	}

	return NULL;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CStatusWindow_Base::Init(void)
{
	HRESULT hr;

	// 初期化処理
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// テクスチャの割り当て
	int nIdxTex = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\statuswindow\\statuswindow_base02.png");

	// テクスチャの割り当て
	BindTexture(nIdxTex);

	// サイズ取得
	D3DXVECTOR2 size = CManager::GetInstance()->GetTexture()->GetImageSize(nIdxTex) * 0.25f;
	size *= 0.5f;

	// サイズ設定
	SetSize(size);

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	return S_OK;
}

//==================================================================================
// 終了処理
//==================================================================================
void CStatusWindow_Base::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//==================================================================================
// 更新処理
//==================================================================================
void CStatusWindow_Base::Update(void)
{
	// 頂点座標の設定
	SetVtx();
}

//==================================================================================
// 描画処理
//==================================================================================
void CStatusWindow_Base::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject2D::Draw();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

