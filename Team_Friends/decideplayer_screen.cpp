//=============================================================================
// 
//  タイトル画面処理 [decideplayer_screen.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "decideplayer_screen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "decidecharacter.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define ALPHATIME	(60)		// 不透明度更新の時間

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CDecidePlayerScreen::m_apTextureFile[VTX_MAX] =			// テクスチャのファイル
{
	"data\\TEXTURE\\decideplayer_text.png",
};

const char *CDecidePlayerScreen::m_apTextureFile_Select[VTXSELECT_MAX] =	// テクスチャのファイル
{
	"data\\TEXTURE\\decideplayer_select01.png",
	"data\\TEXTURE\\decideplayer_select02.png",
	"data\\TEXTURE\\decideplayer_select03.png",
	"data\\TEXTURE\\decideplayer_select04.png",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CDecidePlayerScreen::CDecidePlayerScreen(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	memset(&m_pObj2D[0], NULL, sizeof(m_pObj2D));				// オブジェクト2Dのオブジェクト
	memset(&m_pSelect2D[0], NULL, sizeof(m_pSelect2D));			// 選択肢のオブジェクト
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));				// テクスチャのインデックス番号
	memset(&m_nTexIdx_Select[0], 0, sizeof(m_nTexIdx_Select));	// テクスチャのインデックス番号
	m_nCntAlpha = 0;		// 不透明度のカウンター
	m_nNowSelect = 0;		// 現在の選択肢
}

//==========================================================================
// デストラクタ
//==========================================================================
CDecidePlayerScreen::~CDecidePlayerScreen()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDecidePlayerScreen *CDecidePlayerScreen::Create(void)
{
	// 生成用のオブジェクト
	CDecidePlayerScreen *pTitleScreen = NULL;

	if (pTitleScreen == NULL)
	{// NULLだったら

		// メモリの確保
		pTitleScreen = DEBUG_NEW CDecidePlayerScreen;

		if (pTitleScreen != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pTitleScreen->Init();
		}

		return pTitleScreen;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDecidePlayerScreen::Init(void)
{
	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// テクスチャのオブジェクト取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		// 生成処理
		m_pObj2D[nCntSelect] = CObject2D::Create(8);

		// 種類の設定
		m_pObj2D[nCntSelect]->SetType(TYPE_OBJECT2D);

		// テクスチャの割り当て
		m_nTexIdx[nCntSelect] = pTexture->Regist(m_apTextureFile[nCntSelect]);

		// テクスチャの割り当て
		m_pObj2D[nCntSelect]->BindTexture(m_nTexIdx[nCntSelect]);

		// 各種変数の初期化
		D3DXVECTOR2 size;
		switch (nCntSelect)
		{
		case VTX_TEXT:
			// サイズ取得
			size = pTexture->GetImageSize(m_nTexIdx[nCntSelect]);
			m_pObj2D[nCntSelect]->SetSize(size * 0.4f);	// サイズ
			m_pObj2D[nCntSelect]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));	// 位置
			m_pObj2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);	// 色
			break;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		// 生成処理
		m_pSelect2D[nCntSelect] = CObject2D::Create(8);

		// 種類の設定
		m_pSelect2D[nCntSelect]->SetType(TYPE_OBJECT2D);

		// テクスチャの割り当て
		m_nTexIdx_Select[nCntSelect] = pTexture->Regist(m_apTextureFile_Select[nCntSelect]);

		// テクスチャの割り当て
		m_pSelect2D[nCntSelect]->BindTexture(m_nTexIdx_Select[nCntSelect]);

		// サイズ取得
		D3DXVECTOR2 size = pTexture->GetImageSize(m_nTexIdx_Select[nCntSelect]) * 0.4f;
		float fDistance = size.x * 2.0f;

		m_pSelect2D[nCntSelect]->SetSize(size);								// サイズ
		m_pSelect2D[nCntSelect]->SetPosition(D3DXVECTOR3(640.0f - (fDistance * 2.0f) + nCntSelect * fDistance + size.x, 550.0f, 0.0f));	// 位置
		m_pObj2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);	// 色
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDecidePlayerScreen::Uninit(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != NULL)
		{// NULLじゃなかったら

			// 終了処理
			m_pObj2D[nCntSelect] = NULL;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] != NULL)
		{// NULLじゃなかったら

			// 終了処理
			m_pSelect2D[nCntSelect] = NULL;
		}
	}

	// 情報削除
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CDecidePlayerScreen::Delete(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != NULL)
		{// NULLじゃなかったら

			// 終了処理
			m_pObj2D[nCntSelect]->Uninit();
			m_pObj2D[nCntSelect] = NULL;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] != NULL)
		{// NULLじゃなかったら

			// 終了処理
			m_pSelect2D[nCntSelect]->Uninit();
			m_pSelect2D[nCntSelect] = NULL;
		}
	}

	// 情報削除
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CDecidePlayerScreen::Update(void)
{

	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] == NULL)
		{// NULLだったら
			continue;
		}

		// 頂点情報設定
		m_pObj2D[nCntSelect]->SetVtx();
	}




	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// 現在の選択肢更新
	if ((pInputGamepad->GetStickSelect(CInputGamepad::STICK_X) == false && pInputGamepad->GetStickMoveL(0).x < 0) ||
		(pInputKeyboard->GetTrigger(DIK_A) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LEFT, 0)))
	{// 左

		// 左スティックの判定を渡す
		pInputGamepad->SetEnableStickSelect(true, CInputGamepad::STICK_X);

		// パターンNo.を更新
		m_nNowSelect = (m_nNowSelect + (VTXSELECT_MAX - 1)) % VTXSELECT_MAX;

		// サウンド再生
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);

		// カウンターリセット
		m_nCntAlpha = 0;
	}
	else if (pInputGamepad->GetStickSelect(CInputGamepad::STICK_X) == false && pInputGamepad->GetStickMoveL(0).x > 0 ||
		(pInputKeyboard->GetTrigger(DIK_D) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_RIGHT, 0)))
	{// 右

		// 左スティックの判定を渡す
		pInputGamepad->SetEnableStickSelect(true, CInputGamepad::STICK_X);

		// パターンNo.を更新
		m_nNowSelect = (m_nNowSelect + 1) % VTXSELECT_MAX;

		// サウンド再生
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CURSOR);

		// カウンターリセット
		m_nCntAlpha = 0;
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{// 決定が押された
		CManager::GetInstance()->SetNumPlayer(m_nNowSelect + 1);
		//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);

		// キャラ決め画面生成
		CDecideCharacter::Create();

		Delete();
		return;
	}


	for (int nCntSelect = 0; nCntSelect < VTXSELECT_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] == NULL)
		{// NULLだったら
			continue;
		}

		// 選択肢の更新処理
		UpdateSelect(nCntSelect);

		// 頂点情報設定
		m_pSelect2D[nCntSelect]->SetVtx();
	}
}

//==========================================================================
// 選択肢の更新処理
//==========================================================================
void CDecidePlayerScreen::UpdateSelect(int nCntSelect)
{
	// 色取得
	D3DXCOLOR col = m_pSelect2D[nCntSelect]->GetColor();

	// 不透明度更新
	if (m_nNowSelect == nCntSelect)
	{
		CuadricCurveComp(col.a, ALPHATIME, 0.3f, 1.0f, m_nCntAlpha);
	}
	else
	{
		col.a = 1.0f;
	}

	// 色設定
	m_pSelect2D[nCntSelect]->SetColor(col);
}

//==========================================================================
// 描画処理
//==========================================================================
void CDecidePlayerScreen::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	//{
	//	// 描画処理
	//	m_pObj2D[nCntSelect]->Draw();
	//}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

