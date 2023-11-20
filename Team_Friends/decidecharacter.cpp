//=============================================================================
// 
//  キャラクター決め画面処理 [decidecharacter.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "decidecharacter.h"
#include "decideplayer_screen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "cursor.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define ALPHATIME	(60)		// 不透明度更新の時間

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CDecideCharacter::m_apTextureFile[VTX_MAX] =			// テクスチャのファイル
{
	"data\\TEXTURE\\decideplayer_text.png",
};

const char *CDecideCharacter::m_apTextureFile_Select[VTXCHARACTER_MAX] =	// テクスチャのファイル
{
	"data\\TEXTURE\\decidecharacter_01.png",
	"data\\TEXTURE\\decidecharacter_02.png",
	"data\\TEXTURE\\decidecharacter_03.png",
	"data\\TEXTURE\\decidecharacter_04.png",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CDecideCharacter::CDecideCharacter(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	memset(&m_pObj2D[0], NULL, sizeof(m_pObj2D));				// オブジェクト2Dのオブジェクト
	memset(&m_pSelect2D[0], NULL, sizeof(m_pSelect2D));			// 選択肢のオブジェクト
	memset(&m_apCursor[0], NULL, sizeof(m_apCursor));			// カーソルのオブジェクト
	memset(&m_bDecide[0], false, sizeof(m_bDecide));			// 決定したか
	m_bAllDecide = false;										// 全て決定したか
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));				// テクスチャのインデックス番号
	memset(&m_nTexIdx_Select[0], 0, sizeof(m_nTexIdx_Select));	// テクスチャのインデックス番号
}

//==========================================================================
// デストラクタ
//==========================================================================
CDecideCharacter::~CDecideCharacter()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDecideCharacter *CDecideCharacter::Create(void)
{
	// 生成用のオブジェクト
	CDecideCharacter *pTitleScreen = NULL;

	if (pTitleScreen == NULL)
	{// NULLだったら

		// メモリの確保
		pTitleScreen = DEBUG_NEW CDecideCharacter;

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
HRESULT CDecideCharacter::Init(void)
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

	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
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
		D3DXVECTOR2 size = pTexture->GetImageSize(m_nTexIdx_Select[nCntSelect]) * 0.2f;

		m_pSelect2D[nCntSelect]->SetSize(size);			// サイズ
		m_pSelect2D[nCntSelect]->SetSizeOrigin(size);	// 元のサイズ

		size *= 1.5f;
		float fDistance = size.x * 2.0f;
		m_pSelect2D[nCntSelect]->SetPosition(D3DXVECTOR3(640.0f - (fDistance * 2.0f) + nCntSelect * fDistance + size.x, 600.0f, 0.0f));	// 位置
		m_pObj2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);	// 色
	}

	for (int i = 0; i < CManager::GetInstance()->GetNumPlayer(); i++)
	{
		float fDistance = 100.0f;
		D3DXVECTOR3 pos = D3DXVECTOR3(640.0f - (fDistance * 2.0f) + i * fDistance + (fDistance * 0.5f), 200.0f, 0.0f);

		// カーソル生成
		m_apCursor[i] = CCursor::Create(pos, i);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDecideCharacter::Uninit(void)
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != NULL)
		{// NULLじゃなかったら

			// 終了処理
			m_pObj2D[nCntSelect] = NULL;
		}
	}

	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
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
void CDecideCharacter::Delete(void)
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

	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
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
void CDecideCharacter::Update(void)
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

	if (m_bDecide[0] == false &&
		(pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_B, 0)))
	{// キャンセル

		for (int i = 0; i < CManager::GetInstance()->GetNumPlayer(); i++)
		{
			if (m_apCursor[i] == NULL)
			{
				continue;
			}
			m_apCursor[i]->Uninit();
			m_apCursor[i] = NULL;
		}

		// 削除
		Delete();

		// プレイヤー人数選択画面生成
		CDecidePlayerScreen::Create();
		return;
	}


	if (m_bAllDecide == true &&
		(pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_START, 0)))
	{// 決定が押された

		// ゲームに遷移する
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
	}


	for (int nCntSelect = 0; nCntSelect < VTXCHARACTER_MAX; nCntSelect++)
	{
		if (m_pSelect2D[nCntSelect] == NULL)
		{// NULLだったら
			continue;
		}

		// 選択肢との当たり判定
		if (CollisionSelect(nCntSelect))
		{
			//break;
		}

		// 頂点情報設定
		m_pSelect2D[nCntSelect]->SetVtx();
	}
}

//==========================================================================
// 選択肢の更新処理
//==========================================================================
bool CDecideCharacter::CollisionSelect(int nCntSelect)
{
	// 情報取得
	D3DXVECTOR3 pos = m_pSelect2D[nCntSelect]->GetPosition();
	D3DXVECTOR2 size = m_pSelect2D[nCntSelect]->GetSize();
	D3DXVECTOR2 sizeOrigin = m_pSelect2D[nCntSelect]->GetSizeOrigin();

	// 四角と円の判定
	bool bHit = false;
	int nCursorIdx = -1;

	// 全て決定状態
	m_bAllDecide = true;

	for (int i = 0; i < CManager::GetInstance()->GetNumPlayer(); i++)
	{
		if (m_apCursor[i] == NULL)
		{
			continue;
		}

		// カーソルごとの選択肢のインデックス番号取得
		int nDecideIdx = m_apCursor[i]->GetSelectIdx();

		if (nDecideIdx >= 0)
		{
			// 決定状態取得
			m_bDecide[nDecideIdx] = m_apCursor[i]->IsDecide();

			if (m_bDecide[nDecideIdx] == true)
			{
				continue;
			}
			else
			{
				// 全て決定してないようにする
				m_bAllDecide = false;
			}
		}
		else
		{
			// 全て決定してないようにする
			m_bAllDecide = false;
		}

		// 情報取得
		D3DXVECTOR3 posCursor = m_apCursor[i]->GetPosition();
		D3DXVECTOR2 sizeCursor = m_apCursor[i]->GetSize();

		// 円と矩形の判定
		bHit = CollisionCircleSquare2D(posCursor, pos, mylib_const::DEFAULT_VECTOR3, sizeCursor.x, size);

		if (bHit == true)
		{
			// カーソルのインデックス番号保存
			nCursorIdx = i;
			break;
		}
	}

	if (bHit && m_bDecide[nCntSelect] == false)
	{
		size.x += ((sizeOrigin.x * 1.2f) - size.x) * 0.25f;
		size.y += ((sizeOrigin.y * 1.2f) - size.y) * 0.25f;

		// キーボード情報取得
		CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

		// ゲームパッド情報取得
		CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, nCursorIdx))
		{
			// 決定判定
			m_bDecide[nCntSelect] = true;
			m_apCursor[nCursorIdx]->SetEnbaleDicide(true);
			m_apCursor[nCursorIdx]->SetSelectIdx(nCntSelect);
			CManager::GetInstance()->SetByPlayerPartsType(nCursorIdx, nCntSelect);
		}

	}
	else
	{
		size.x += (sizeOrigin.x - size.x) * 0.25f;
		size.y += (sizeOrigin.y - size.y) * 0.25f;
	}

	if (m_bDecide[nCntSelect] == true)
	{
		m_pSelect2D[nCntSelect]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
	}
	else
	{
		m_pSelect2D[nCntSelect]->SetColor(mylib_const::DEFAULT_COLOR);
	}

	// 情報設定
	m_pSelect2D[nCntSelect]->SetPosition(pos);
	m_pSelect2D[nCntSelect]->SetSize(size);

	return bHit;
}

//==========================================================================
// 描画処理
//==========================================================================
void CDecideCharacter::Draw(void)
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

