//=============================================================================
// 
// カーソル処理 [cursor.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "cursor.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "fade.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define MOVE	(1.0f)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CCursor::m_apTextureFile[mylib_const::MAX_PLAYER] =	// テクスチャのファイル
{
	"data\\TEXTURE\\000.png",
	"data\\TEXTURE\\001.png",
	"data\\TEXTURE\\002.png",
	"data\\TEXTURE\\003.png",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCursor::CCursor(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nMyPlayerIdx = 0;	// プレイヤーインデックス番号
	m_nTexIdx = 0;		// テクスチャのインデックス番号
	m_nSelectIdx = 0;	// 選択肢のインデックス番号
	m_bDecide = false;	// 決まったかどうか
}

//==========================================================================
// デストラクタ
//==========================================================================
CCursor::~CCursor()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCursor *CCursor::Create(const D3DXVECTOR3 pos, int nIdx)
{
	// 生成用のオブジェクト
	CCursor *pEffect = NULL;

	if (pEffect == NULL)
	{// NULLだったら

		// メモリの確保
		pEffect = DEBUG_NEW CCursor;

		if (pEffect != NULL)
		{// メモリの確保が出来ていたら

			// プレイヤーインデックス
			pEffect->m_nMyPlayerIdx = nIdx;

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
HRESULT CCursor::Init(void)
{
	HRESULT hr;

	// 初期化処理
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	m_nSelectIdx = -1;

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// テクスチャの割り当て
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_nMyPlayerIdx]);

	// テクスチャの割り当て
	BindTexture(m_nTexIdx);

	// サイズ取得
	D3DXVECTOR2 size = CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx) * 0.2f;

	// サイズ設定
	SetSize(size);

	return S_OK;
}

//==================================================================================
// 終了処理
//==================================================================================
void CCursor::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//==================================================================================
// 更新処理
//==================================================================================
void CCursor::Update(void)
{

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{
		return;
	}

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();


	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x < 0)
	{//←キーが押された,左移動

		if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
		{//A+W,左上移動
			move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
			move.y += cosf(-D3DX_PI * 0.75f) * MOVE;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
		{//A+S,左下移動
			move.x += sinf(-D3DX_PI * 0.25f) * MOVE;
			move.y += cosf(-D3DX_PI * 0.25f) * MOVE;
		}
		else
		{//A,左移動
			move.x += sinf(-D3DX_PI * 0.5f) * MOVE;
			move.y += cosf(-D3DX_PI * 0.5f) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x > 0)
	{//Dキーが押された,右移動

		if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
		{//D+W,右上移動
			move.x += sinf(D3DX_PI * 0.75f) * MOVE;
			move.y += cosf(D3DX_PI * 0.75f) * MOVE;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
		{//D+S,右下移動
			move.x += sinf(D3DX_PI * 0.25f) * MOVE;
			move.y += cosf(D3DX_PI * 0.25f) * MOVE;
		}
		else
		{//D,右移動
			move.x += sinf(D3DX_PI * 0.5f) * MOVE;
			move.y += cosf(D3DX_PI * 0.5f) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
	{//Wが押された、上移動
		move.x += sinf(D3DX_PI * 1.0f) * MOVE;
		move.y += cosf(D3DX_PI * 1.0f) * MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
	{//Sが押された、下移動
		move.x += sinf(D3DX_PI * 0.0f) * MOVE;
		move.y += cosf(D3DX_PI * 0.0f) * MOVE;
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_B, m_nMyPlayerIdx))
	{// キャンセル
		m_bDecide = false;
	}

	// 移動
	pos += move;

	// 慣性
	move.x += (0.0f - move.x) * 0.1f;
	move.y += (0.0f - move.y) * 0.1f;

	// 位置・移動量
	SetPosition(pos);
	SetMove(move);

	// 頂点座標の設定
	SetVtx();
}

//==================================================================================
// 描画処理
//==================================================================================
void CCursor::Draw(void)
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

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CCursor::SetVtx(void)
{
	// 頂点設定
	CObject2D::SetVtx();
}

