//==========================================
//
//  タイトルロゴ(title_logo.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "title_logo.h"
#include "logo_complete.h"
#include "logo_mechanion.h"
#include "logo_mekanion.h"
#include "logo_squadron.h"
#include "manager.h"
#include "debugproc.h"
#include "camera.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 DIFF_POS = D3DXVECTOR3(0.0f, 15.0f, 50.0f);
}

//==========================================
//  コンストラクタ
//==========================================
CTitleLogo::CTitleLogo():
m_pMech(nullptr),
m_pMeka(nullptr),
m_pSqou(nullptr),
m_pComp(nullptr),
m_State(DEFAULT),
m_bComplete(false)
{
	
}

//==========================================
//  デストラクタ
//==========================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CTitleLogo::Init(void)
{
	//タイプの設定
	SetType(TYPE_OBJECT3D);

	//ロゴの位置を設定
	SetPosition(CManager::GetInstance()->GetCamera()->GetPositionV() + DIFF_POS);

	//MECHANIONロゴを表示
	if (m_pMech == nullptr)
	{
		m_pMech = CLogo_Mech::Create(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CTitleLogo::Uninit(void)
{	 
	//MECHANIONロゴを破棄
	if (m_pMech != nullptr)
	{
		m_pMech->Uninit();
		m_pMech = nullptr;
	}

	//メカニオンロゴを破棄
	if (m_pMeka != nullptr)
	{
		m_pMeka->Uninit();
		m_pMeka = nullptr;
	}

	//戦隊ロゴを破棄
	if (m_pSqou != nullptr)
	{
		m_pSqou->Uninit();
		m_pSqou = nullptr;
	}

	//完成したロゴを破棄
	if (m_pComp != nullptr)
	{
		m_pComp->Uninit();
		m_pComp = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//==========================================
//  更新処理
//==========================================
void CTitleLogo::Update(void)
{	
	//状態更新
	UpdateState();

	//デバッグ表示
	D3DXVECTOR3 pos = GetPosition();
	CManager::GetInstance()->GetDebugProc()->Print
	(
		"位置 : ( %f, %f, %f )\n", pos.x, pos.y, pos.z
	);
}

//==========================================
//  描画処理
//==========================================
void CTitleLogo::Draw(void)
{

}

//==========================================
//  生成処理
//==========================================
CTitleLogo* CTitleLogo::Create()
{
	//インスタンス生成
	CTitleLogo* pLogo = DEBUG_NEW CTitleLogo;

	//初期化処理
	pLogo->Init();

	return pLogo;
}

//==========================================
//  状態を更新
//==========================================
void CTitleLogo::UpdateState()
{
	//完成していないとき
	if (!m_bComplete)
	{
		switch (m_State)
		{
		case DEFAULT: // MECHANIONしかない

			if (m_pMech != nullptr && m_pMech->GetComplete())
			{
				//メカニオンロゴを表示
				if (m_pMeka == nullptr)
				{
					m_pMeka = CLogo_Meka::Create(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}

				//状態を進める
				m_State = WAKE;
			}
			break;

		case WAKE: // メカニオンまで出てる

			if (m_pMeka != nullptr && m_pMeka->GetComplete())
			{
				//戦隊ロゴを表示
				if (m_pSqou == nullptr)
				{
					m_pSqou = CLogo_Sqou::Create(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}

				//状態を進める
				m_State = FLASH;
			}
			break;

		case FLASH: //戦隊まで出てる

			if (m_pSqou != nullptr && m_pSqou->GetComplete())
			{
				//MECHANIONロゴを破棄
				if (m_pMech != nullptr)
				{
					m_pMech->Uninit();
					m_pMech = nullptr;
				}

				//メカニオンロゴを破棄
				if (m_pMeka != nullptr)
				{
					m_pMeka->Uninit();
					m_pMeka = nullptr;
				}

				//戦隊ロゴを破棄
				if (m_pSqou != nullptr)
				{
					m_pSqou->Uninit();
					m_pSqou = nullptr;
				}

				//完成したロゴを表示
				if (m_pComp == nullptr)
				{
					m_pComp = CLogo_Comp::Create(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}

				//状態を進める
				m_State = COMPLETE;
			}
			break;

		case COMPLETE: // 完成している
			m_bComplete = true; // タイトルロゴを完成
			break;

		default:
			break;
		}
	}
}
