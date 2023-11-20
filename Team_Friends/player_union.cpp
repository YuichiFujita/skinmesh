//=============================================================================
// 
//  プレイヤー処理 [player_union.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "game.h"
#include "player_union.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "enemy.h"
#include "calculation.h"
#include "score.h"
#include "texture.h"
#include "Xload.h"
#include "model.h"
#include "motion.h"
#include "hp_gauge.h"
#include "objectChara.h"
#include "elevation.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"
#include "ballast.h"
#include "impactwave.h"
#include "sound.h"
#include "enemymanager.h"
#include "bullet.h"
#include "stage.h"
#include "objectX.h"
#include "gamemanager.h"
#include "instantfade.h"
#include "hp_gauge_player.h"
#include "fade.h"
#include "listmanager.h"
#include "item.h"
#include "injectiontable.h"

// 派生先
#include "union_bodytoleg.h"
#include "union_bodytoarm.h"
#include "union_armtoarm.h"
#include "union_legtoarm.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define JUMP			(20.0f * 1.5f)	// ジャンプ力初期値
#define MAX_LIFE		(100)			// 体力
#define TARGET_LEN		(400.0f)		// 目標までの距離
#define WALK_INT		(30)			// 歩行の時間
#define INVINCIBLE_INT	(2)				// 無敵の間隔
#define INVINCIBLE_TIME	(90)			// 無敵の時間
#define CONFUSIONTIME	(60 * 2)		// 混乱時間
#define DEADTIME		(120)
#define FADEOUTTIME		(60)
#define RADIUS			(250.0f)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
bool CPlayerUnion::m_bAllLandInjectionTable = false;	// 全員の射出台着地判定
bool CPlayerUnion::m_bLandInjectionTable[mylib_const::MAX_PLAYER] = {};	// 射出台の着地判定

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUnion::CPlayerUnion(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	// 共有変数
	m_bJump = false;			// ジャンプ中かどうか
	m_bLandOld = false;			// 過去の着地情報
	m_bHitStage = false;		// ステージの当たり判定
	m_bLandField = false;		// フィールドの着地判定
	m_bHitWall = false;			// 壁の当たり判定
	m_bKnockBack = false;		// ノックバック中かどうか
	m_bDead = false;			// 死亡中かどうか
	m_nUnionLife = 0;			// 合体時間
	m_nCntWalk = 0;				// 歩行カウンター
	m_nCntInputAtk = 0;			// 攻撃の入力カウンター
	m_state = STATE_NONE;		// 状態
	memset(&m_pMotion[0], NULL, sizeof(m_pMotion));	// パーツ分のモーションポインタ
	memset(&m_sMotionFrag[0], false, sizeof(m_sMotionFrag));	// モーションのフラグ

	// プライベート変数
	memset(&m_pObjChara[0], NULL, sizeof(m_pObjChara));	// パーツ分のオブジェクトキャラクターポインタ

	m_Oldstate = STATE_NONE;			// 前回の状態
	m_mMatcol = mylib_const::DEFAULT_COLOR;		// マテリアルの色
	m_posKnokBack = mylib_const::DEFAULT_VECTOR3;	// ノックバックの位置
	m_KnokBackMove = mylib_const::DEFAULT_VECTOR3;	// ノックバックの移動量
	m_nCntState = 0;			// 状態遷移カウンター
	m_nTexIdx = 0;				// テクスチャのインデックス番号
	m_nIdxXFile = 0;			// Xファイルのインデックス番号
	memset(&m_nPartsIdx[0], 0, sizeof(m_nPartsIdx));	// プレイヤー毎のパーツインデックス番号
	m_nMyPlayerIdx = 0;			// プレイヤーインデックス番号
	m_nControllMoveIdx = 0;		// 移動操作するやつのインデックス番号
	m_fRotDest = 0.0f;
	m_pShadow = NULL;			// 影の情報
	m_pTargetP = NULL;	// 目標の地点
	m_pHPGauge = NULL;	// HPゲージの情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUnion::~CPlayerUnion()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPlayerUnion *CPlayerUnion::Create(eType type)
{
	// 生成用のオブジェクト
	CPlayerUnion *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULLだったら

		// メモリの確保
		switch (type)
		{
		case TYPE_ALL:
			pPlayer = DEBUG_NEW CPlayerUnion;
			break;

		case TYPE_BODYtoLEG:
			pPlayer = DEBUG_NEW CUnion_BodytoLeg;
			break;

		case TYPE_BODYtoARM:
			pPlayer = DEBUG_NEW CUnion_BodytoArm;
			break;

		case TYPE_LEGtoARM:
			pPlayer = DEBUG_NEW CUnion_LegtoArm;
			break;

		case TYPE_ARMtoARM:
			pPlayer = DEBUG_NEW CUnion_ArntoArm;
			break;

		default:
			return NULL;
			break;
		}

		if (pPlayer != NULL)
		{// メモリの確保が出来ていたら

			// プレイヤーインデックス番号
			//pPlayer->m_nMyPlayerIdx = nIdx;

			// 初期化処理
			pPlayer->Init();
		}

		return pPlayer;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerUnion::Init(void)
{
	// 種類の設定
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// 状態
	m_nCntState = 0;		// 状態遷移カウンター
	m_bLandOld = true;		// 前回の着地状態
	m_bAllLandInjectionTable = false;	// 全員の射出台着地判定
	memset(&m_bLandInjectionTable[0], false, sizeof(m_bLandInjectionTable));	// 射出台の着地判定
	m_nUnionLife = 0;		// 合体時間

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		m_nPartsIdx[i] = -1;
	}

	// キャラ作成
	CreateParts();


	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 体力ゲージ
	m_pHPGauge = CHP_GaugePlayer::Create(D3DXVECTOR3(250.0f, 600.0f, 0.0f), 50);

	// 影の生成
	m_pShadow = CShadow::Create(pos, 50.0f);

	SetPosition(D3DXVECTOR3(-600.0f, 0.0f, -1000.0f));
	return S_OK;
}

//==========================================================================
// パーツの設定
//==========================================================================
HRESULT CPlayerUnion::CreateParts(void)
{
	HRESULT hr;


	// 複数キャラ読み込み
	ReadMultiCharacter("data\\TEXT\\multicharacter\\SuperUnion.txt");
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerUnion::Uninit(void)
{
	for (int i = 0; i < PARTS_MAX; i++)
	{
		if (m_pMotion[i] != NULL)
		{
			m_pMotion[i]->Uninit();
			delete m_pMotion[i];
			m_pMotion[i] = NULL;
		}
	}

	for (int i = 0; i < PARTS_MAX; i++)
	{
		if (m_pObjChara[i] != NULL)
		{
			m_pObjChara[i] = NULL;
		}
	}

	// HPゲージを消す
	if (m_pHPGauge != NULL)
	{
		m_pHPGauge->Uninit();
		m_pHPGauge = NULL;
	}

	// 影を消す
	if (m_pShadow != NULL)
	{
		//m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// 終了処理
	CObject::Release();

	// モード別終了処理
	UninitByMode();
}

//==========================================================================
// モード別終了処理
//==========================================================================
void  CPlayerUnion::UninitByMode(void)
{
	//CScene *pScene = CManager::GetInstance()->GetScene();
	//if (pScene != NULL)
	//{
	//	// プレイヤーをNULL
	//	CManager::GetInstance()->GetScene()->UninitPlayer(m_nMyPlayerIdx);
	//}
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerUnion::Kill(void)
{

	my_particle::Create(GetPosition(), my_particle::TYPE_ENEMY_FADE);

	// HPゲージを消す
	if (m_pHPGauge != NULL)
	{
		m_pHPGauge->Kill();
		m_pHPGauge = NULL;
	}

	// 影を消す
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	for (int i = 0; i < PARTS_MAX; i++)
	{
		if (m_pObjChara[i] != NULL)
		{
			m_pObjChara[i]->Uninit();
			m_pObjChara[i] = NULL;
		}
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerUnion::Update(void)
{
	if (IsDeath() == true)
	{
		return;
	}

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{// F5でリセット
		SetPosition(D3DXVECTOR3(0.0f, 0.0f, -1000.0f));
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// エディット中は抜ける
	if (CGame::GetElevation()->IsEdit())
	{
		return;
	}

	// エディット中は抜ける
	if (CGame::GetEditType() != CGame::EDITTYPE_OFF)
	{
		return;
	}

	// コンボ演出中は抜ける
	if (CGame::GetEnemyManager()->GetState() == CEnemyManager::STATE_COMBOANIM)
	{
		return;
	}

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// 操作
	Controll();

	for (int i = 0; i < PARTS_MAX; i++)
	{
		// モーションの設定処理
		MotionSet(i);
	}


	// モーション更新
	for (int i = 0; i < PARTS_MAX; i++)
	{
		if (m_pMotion[i] == NULL)
		{
			continue;
		}
		m_pMotion[i]->Update();

		// 攻撃処理
		Atack(i);
	}

	// 状態更新
	UpdateState();

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 位置・向き設定
	for (int i = 0; i < PARTS_MAX; i++)
	{
		if (m_pObjChara[i] == NULL)
		{
			continue;
		}

		m_pObjChara[i]->SetPosition(pos);
		m_pObjChara[i]->SetRotation(rot);
	}

	// 影の位置更新
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPosition(D3DXVECTOR3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	// HPゲージの位置更新
	if (m_pHPGauge != NULL)
	{
		m_pHPGauge->Update();
		m_pHPGauge->SetLife(50);
	}

#if 0
	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[プレイヤーの操作]------------------\n"
		"位置：【X：%f, Y：%f, Z：%f】 【W / A / S / D】\n"
		"向き：【X：%f, Y：%f, Z：%f】 【Z / C】\n"
		"移動量：【X：%f, Y：%f, Z：%f】\n"
		"体力：【%d】\n", pos.x, pos.y, pos.z, rot.x, rot.y, rot.y, move.x, move.y, move.z, 50);
#endif
}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayerUnion::Controll(void)
{

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 newPosition = GetPosition();
	D3DXVECTOR3 sakiPos = GetPosition();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 経過時間取得
	float fCurrentTime = CManager::GetInstance()->GetDeltaTime();

	if (CGame::GetGameManager()->IsControll())
	{// 行動できるとき

		// パーツのコントロール処理
		ControllParts();
	}

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 移動量加算
	newPosition.x += move.x;
	newPosition.z += move.z;

	sakiPos.x = newPosition.x + sinf(D3DX_PI + rot.y) * RADIUS;
	sakiPos.z = newPosition.z + cosf(D3DX_PI + rot.y) * RADIUS;

	// 角度の正規化
	RotNormalize(m_fRotDest);

	// 現在と目標の差分を求める
	float fRotDiff = m_fRotDest - rot.y;

	// 角度の正規化
	RotNormalize(fRotDiff);

	// 角度の補正をする
	rot.y += fRotDiff * 0.15f;

	// 角度の正規化
	RotNormalize(rot.y);

	// 重力処理
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DMG && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		move.y -= mylib_const::GRAVITY;

		// 位置更新
		newPosition.y += move.y;
		sakiPos.y = newPosition.y;
	}

	// 射出台の着地判定
	m_bLandInjectionTable[m_nMyPlayerIdx] = false;

	//**********************************
	// 当たり判定
	//**********************************
	bool bLandStage = Collision(sakiPos, move);

	bool bMove = false;
	if (m_bLandOld == false && bLandStage == true)
	{// 前回は着地していなくて、今回は着地している場合

		bMove = false;
	}

	if (m_bLandOld == true && bLandStage == true)
	{// 前回も今回も着地している場合
		bMove = true;
	}

	if (m_bHitWall == false && 
		(bLandStage == false || bMove == true || m_bLandField == true || m_bJump == true || m_bKnockBack == true || m_bDead == true))
	{
		pos.x = newPosition.x;
		pos.z = newPosition.z;
		pos.y = sakiPos.y;
		Collision(pos, move);

		// 前回は乗ってたことにする
		m_bLandOld = true;
	}
	else
	{
		D3DXVECTOR3 posOld = GetOldPosition();
		pos.x = posOld.x;
		pos.z = posOld.z;
		pos = posOld;
		move.x = 0.0f;
		pos.y -= mylib_const::GRAVITY * 7.0f;

		if (m_bJump == false)
		{
			m_bJump = true;
		}

		Collision(pos, move);
	}

	// 位置更新
	//pos.y += move.y;

	// 慣性補正
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DMG && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		move.x += (0.0f - move.x) * 0.25f;
		move.z += (0.0f - move.z) * 0.25f;
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);


	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetTargetPosition(pos);
	pCamera->SetTargetRotation(rot);

	// 目標の向き設定
	//SetRotDest(m_fRotDest);

}

//==========================================================================
// パーツのコントロール処理(バーチャル)
//==========================================================================
void CPlayerUnion::ControllParts(void)
{
	// 各部位の操作	
	for (int i = 0; i < PARTS_MAX; i++)
	{
		int nPartsIdx = CManager::GetInstance()->GetByPlayerPartsType(i);
		switch (nPartsIdx)
		{
		case PARTS_BODY:
			ControllBody(nPartsIdx);
			break;

		case PARTS_LEG:
			ControllLeg(nPartsIdx);
			break;

		case PARTS_L_ARM:
			ControllLeftArm(nPartsIdx);
			break;

		case PARTS_R_ARM:
			ControllRightArm(nPartsIdx);
			break;
		}
	}
}

//==========================================================================
// 胴操作
//==========================================================================
void CPlayerUnion::ControllBody(int nIdx)
{
	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

}

//==========================================================================
// 脚操作
//==========================================================================
void CPlayerUnion::ControllLeg(int nIdx)
{
	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// カメラの向き取得
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// 移動量取得
	float fMove = 2.5f;

	int nLeftArmIdx = CManager::GetInstance()->GetByPlayerPartsType(PARTS_L_ARM);
	int nRightArmIdx = CManager::GetInstance()->GetByPlayerPartsType(PARTS_R_ARM);

	if (m_state != STATE_DEAD &&
		m_state != STATE_FADEOUT)
	{// 移動可能モーションの時

		// 移動操作
		if (ControllMove(nIdx))
		{
			// 移動中にする
			for (int i = 0; i < PARTS_MAX; i++)
			{
				m_sMotionFrag[i].bMove = true;
				if (m_sMotionFrag[nRightArmIdx].bCharge == true)
				{
					m_sMotionFrag[nRightArmIdx].bMove = false;
				}
				if (m_sMotionFrag[nLeftArmIdx].bCharge == true)
				{
					m_sMotionFrag[nLeftArmIdx].bMove = false;
				}
			}
		}
		else
		{
			for (int i = 0; i < PARTS_MAX; i++)
			{
				m_sMotionFrag[i].bMove = false;
			}
		}


		// 移動量取得
		D3DXVECTOR3 move = GetMove();

		if (m_bJump == false &&
			pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LB, nIdx))
		{//SPACEが押された,ジャンプ

			m_bJump = true;
			move.y += 17.0f;

			// ジャンプ中にする
			for (int i = 0; i < PARTS_MAX; i++)
			{
				m_sMotionFrag[i].bJump = true;
				if (m_sMotionFrag[nRightArmIdx].bCharge == true)
				{
					m_sMotionFrag[nRightArmIdx].bJump = false;
				}
				if (m_sMotionFrag[nLeftArmIdx].bCharge == true)
				{
					m_sMotionFrag[nLeftArmIdx].bJump = false;
				}
			}

			// サウンド再生
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_JUMP);
		}

		// 移動量設定
		SetMove(move);
	}

}

//==========================================================================
// 右腕操作
//==========================================================================
void CPlayerUnion::ControllRightArm(int nIdx)
{
	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	
	if ((pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, nIdx)))
	{// 攻撃

		// チャージ判定
		m_sMotionFrag[nIdx].bCharge = true;
	}

	if (m_sMotionFrag[nIdx].bCharge == true &&
		pInputGamepad->GetRelease(CInputGamepad::BUTTON_A, nIdx))
	{// チャージ中に攻撃ボタンを離したら

		// 攻撃中
		m_sMotionFrag[nIdx].bCharge = false;
		m_sMotionFrag[nIdx].bATK = true;
	}
}

//==========================================================================
// 左腕操作
//==========================================================================
void CPlayerUnion::ControllLeftArm(int nIdx)
{
	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	
	if ((pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, nIdx)))
	{// 攻撃

		// チャージ判定
		m_sMotionFrag[nIdx].bCharge = true;
	}

	if (m_sMotionFrag[nIdx].bCharge == true &&
		pInputGamepad->GetRelease(CInputGamepad::BUTTON_A, nIdx))
	{// チャージ中に攻撃ボタンを離したら

		// 攻撃中
		m_sMotionFrag[nIdx].bCharge = false;
		m_sMotionFrag[nIdx].bATK = true;
	}
}

//==========================================================================
// 移動操作
//==========================================================================
bool CPlayerUnion::ControllMove(int nIdx)
{

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// カメラの向き取得
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// 移動量取得
	float fMove = 2.5f;

	bool bMove = true;

	if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(nIdx).x < 0)
	{//←キーが押された,左移動

		if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(nIdx).y > 0)
		{//A+W,左上移動

			move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
			move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
			m_fRotDest = D3DX_PI * 0.75f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(nIdx).y < 0)
		{//A+S,左下移動

			move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
			move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
			m_fRotDest = D3DX_PI * 0.25f + Camerarot.y;
		}
		else
		{//A,左移動

			move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
			m_fRotDest = D3DX_PI * 0.5f + Camerarot.y;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(nIdx).x > 0)
	{//Dキーが押された,右移動

		if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(nIdx).y > 0)
		{//D+W,右上移動

			move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
			m_fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(nIdx).y < 0)
		{//D+S,右下移動

			move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
			m_fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
		}
		else
		{//D,右移動

			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			m_fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(nIdx).y > 0)
	{//Wが押された、上移動

		move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
		move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
		m_fRotDest = D3DX_PI * 1.0f + Camerarot.y;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(nIdx).y < 0)
	{//Sが押された、下移動

		move.x += sinf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
		move.z += cosf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
		m_fRotDest = D3DX_PI * 0.0f + Camerarot.y;
	}
	else
	{
		bMove = false;
	}

	// 移動量設定
	SetMove(move);

	return bMove;
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayerUnion::MotionSet(int nIdx)
{
	if (m_pMotion[nIdx] == NULL)
	{// モーションがNULLだったら
		return;
	}

	if (m_pMotion[nIdx]->IsFinish() == true)
	{// 終了していたら

		// 現在の種類取得
		int nType = m_pMotion[nIdx]->GetType();
		int nOldType = m_pMotion[nIdx]->GetOldType();

		if (m_sMotionFrag[nIdx].bMove == true &&
			m_sMotionFrag[nIdx].bATK == false &&
			m_bKnockBack == false &&
			m_bDead == false &&
			m_sMotionFrag[nIdx].bCharge == false &&
			m_bJump == false)
		{// 移動していたら

			// 移動モーション
			m_pMotion[nIdx]->Set(MOTION_WALK);
			if (nIdx == PARTS_R_ARM || nIdx == PARTS_L_ARM)
			{// 腕パーツ
				m_pMotion[nIdx]->SetNowPattern(m_pMotion[PARTS_BODY]->GetNowPattern());
				m_pMotion[nIdx]->SetFrameCount(m_pMotion[PARTS_BODY]->GetFrameCount());
			}
		}
		else if (m_sMotionFrag[nIdx].bJump == true &&
			m_sMotionFrag[nIdx].bATK == false &&
			m_sMotionFrag[nIdx].bCharge == false &&
			m_bKnockBack == false &&
			m_bDead == false)
		{// ジャンプ中

			// ジャンプのフラグOFF
			m_sMotionFrag[nIdx].bJump = false;

			// ジャンプモーション
			m_pMotion[nIdx]->Set(MOTION_JUMP);
		}
		else if (m_bJump == true &&
			m_sMotionFrag[nIdx].bJump == false &&
			m_sMotionFrag[nIdx].bATK == false &&
			m_sMotionFrag[nIdx].bCharge == false &&
			m_bKnockBack == false &&
			m_bDead == false)
		{// ジャンプ中&&ジャンプモーションが終わってる時

			// 落下モーション
			m_pMotion[nIdx]->Set(MOTION_FALL);
		}
		else if (m_sMotionFrag[nIdx].bCharge == true)
		{// チャージ中だったら

			// チャージモーション
			m_pMotion[nIdx]->Set(MOTION_CHARGE);
		}
		else if (m_bKnockBack == true)
		{// やられ中だったら

			// やられモーション
			m_pMotion[nIdx]->Set(MOTION_KNOCKBACK);
		}
		else if (m_bDead == true)
		{// 死亡中だったら

			// やられモーション
			m_pMotion[nIdx]->Set(MOTION_DEAD);
		}
		else if (m_sMotionFrag[nIdx].bATK == true)
		{// 攻撃していたら

			m_sMotionFrag[nIdx].bATK = false;		// 攻撃判定OFF
			m_pMotion[nIdx]->Set(MOTION_ATK, true);
		}
		else
		{
			// ニュートラルモーション
			m_pMotion[nIdx]->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// 攻撃
//==========================================================================
void CPlayerUnion::Atack(int nIdx)
{
	if (m_pMotion[nIdx] == NULL)
	{// モーションがNULLだったら
		return;
	}

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// カメラの向き取得
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion[nIdx]->GetInfo(m_pMotion[nIdx]->GetType());

	// 攻撃情報の総数取得
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == NULL)
		{// NULLだったら
			continue;
		}

		if (m_pMotion[nIdx]->GetAllCount() == aInfo.AttackInfo[nCntAttack]->nInpactCnt)
		{// 衝撃のカウントと同じとき

			// 攻撃情報保存
			CMotion::AttackInfo atkInfo = *aInfo.AttackInfo[nCntAttack];

			// 攻撃時処理
			AttackAction(nIdx, atkInfo.nCollisionNum, atkInfo);
		}

		// モーションカウンター取得
		if (m_pMotion[nIdx]->GetAllCount() > aInfo.AttackInfo[nCntAttack]->nMinCnt && m_pMotion[nIdx]->GetAllCount() < aInfo.AttackInfo[nCntAttack]->nMaxCnt)
		{// 攻撃判定中

			// 武器の位置
			D3DXVECTOR3 weponpos = m_pMotion[nIdx]->GetAttackPosition(m_pObjChara[nIdx]->GetModel(), *aInfo.AttackInfo[nCntAttack]);

			CEffect3D::Create(weponpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), aInfo.AttackInfo[nCntAttack]->fRangeSize, 10, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

#if 1
			// 敵取得
			CEnemy **ppEnemy = CGame::GetEnemyManager()->GetEnemy();

			// 総数取得
			int nNumAll = CGame::GetEnemyManager()->GetNumAll();
			int i = -1, nCntEnemy = 0;

			while (1)
			{
				if (nCntEnemy >= nNumAll)
				{// 総数超えたら終わり
					break;
				}

				// インデックス加算
				i++;
				if (ppEnemy[i] == NULL)
				{
					continue;
				}

				// 敵の位置取得
				D3DXVECTOR3 TargetPos = ppEnemy[i]->GetPosition();

				// 判定サイズ取得
				float fTargetRadius = ppEnemy[i]->GetRadius();

				if (SphereRange(weponpos, TargetPos, aInfo.AttackInfo[nCntAttack]->fRangeSize, fTargetRadius))
				{// 球の判定

					if (ppEnemy[i]->Hit(aInfo.AttackInfo[nCntAttack]->nDamage) == true)
					{// 当たってたら

					}
				}

				// 敵の数加算
				nCntEnemy++;
			}
#else

			// 敵のリスト取得
			CListManager *pEnemyList = CEnemy::GetEnemyList();

			pEnemyList->GetTop();

			// 先頭を保存
			CList *pList = pEnemyList->GetTop();

			while (pList != NULL)
			{// NULLが来るまで無限ループ

				// 次のオブジェクトを一時保存
				CList *pListNext = pList->GetNext();

				// 死亡の判定
				if (pList->IsDeath() == true)
				{// 死亡フラグが立っていたら

					// 次のオブジェクトを代入
					pList = pListNext;
					continue;
				}

				// 敵の位置取得
				D3DXMATRIX mtxOrigin = pList->GetObjectChara()->GetModel()[0]->GetWorldMtx();
				D3DXVECTOR3 TargetPos = D3DXVECTOR3(mtxOrigin._41, mtxOrigin._42, mtxOrigin._43);

				// 判定サイズ取得
				float fRadius = pList->GetObjectChara()->RADIUS;

				if (SphereRange(weponpos, TargetPos, aInfo.AttackInfo[nCntAttack]->fRangeSize, fRadius))
				{// 球の判定

					if (pList->Hit(aInfo.AttackInfo[nCntAttack]->nDamage) == true)
					{// 死んでたら

						my_particle::Create(TargetPos, my_particle::TYPE_OFFSETTING);
					}
				}

				// 次のオブジェクトを代入
				pList = pListNext;
			}

#endif
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print(
		"モーションカウンター：%d\n", m_pMotion[PARTS_BODY]->GetAllCount());
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CPlayerUnion::AttackAction(int nIdx, int nModelNum, CMotion::AttackInfo ATKInfo)
{

	// 武器の位置
	D3DXVECTOR3 weponpos = m_pMotion[nIdx]->GetAttackPosition(m_pObjChara[nIdx]->GetModel(), ATKInfo);

	// 種類別
	switch (m_pMotion[nIdx]->GetType())
	{
	case MOTION_ATK:
		//// パーティクル生成
		//my_particle::Create(weponpos, my_particle::TYPE_SUPERATTACK);

		//// チャージカウントリセット
		////CGame::GetPowerGauge()->SetChargeCount(0);

		//// 衝撃波生成
		//CImpactWave::Create
		//(
		//	D3DXVECTOR3(pos.x, pos.y + 80.0f, pos.z),	// 位置
		//	D3DXVECTOR3(0.0f, 0.0f, 0.0f),				// 向き
		//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),			// 色
		//	100.0f,										// 幅
		//	20.0f,										// 高さ
		//	20,											// 寿命
		//	28.0f,										// 幅の移動量
		//	CImpactWave::TYPE_BLACK2,					// テクスチャタイプ
		//	true										// 加算合成するか
		//);

		//CImpactWave::Create
		//(
		//	D3DXVECTOR3(pos.x, pos.y + 150.0f, pos.z),	// 位置
		//	D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),				// 向き
		//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f),			// 色
		//	180.0f,										// 幅
		//	150.0f,										// 高さ
		//	14,											// 寿命
		//	4.0f,										// 幅の移動量
		//	CImpactWave::TYPE_GIZAWHITE,				// テクスチャタイプ
		//	false										// 加算合成するか
		//);

		// 振動
		//CManager::GetInstance()->GetCamera()->SetShake(20, 10.0f, 0.0f);

		// 斬撃生成
		//CSlash::Create
		//(
		//	D3DXVECTOR3(pos.x, pos.y + 50.0f, pos.z),	// 位置
		//	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 向き
		//	D3DXVECTOR3(m_fAtkStickRot, D3DX_PI + fRotY, 0.0f),		// 向き
		//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		//	200.0f,								// 幅
		//	50.0f,								// 中心からの間隔
		//	10,									// 寿命
		//	40.0f,								// 幅の移動量
		//	CImpactWave::TYPE_PURPLE4,			// テクスチャの種類
		//	true,								// 加算合成するかどうか
		//	GetMoveAngle()
		//);

		// 歩行音再生
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_SWING);
		break;
	}
}

//==========================================================================
// 当たり判定
//==========================================================================
bool CPlayerUnion::Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &move)
{

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 着地したかどうか
	bool bLand = false;
	float fHeight = 0.0f;
	m_bLandField = false;
	m_bHitWall = false;			// 壁の当たり判定

								// 高さ取得
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DMG && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		fHeight = CManager::GetInstance()->GetScene()->GetElevation()->GetHeight(pos, bLand);
	}
	else
	{
		fHeight = pos.y;
	}

	if (fHeight > pos.y)
	{// 地面の方が自分より高かったら

	 // 地面の高さに補正
		pos.y = fHeight;
		m_bLandField = true;

		if (bLand == true)
		{// 着地してたら

		 // ジャンプ使用可能にする
			m_bJump = false;
			move.y = 0.0f;
			m_bLandOld = true;
		}
	}


	// Xファイルとの判定
	CStage *pStage = CGame::GetStage();
	if (pStage == NULL)
	{// NULLだったら
		return false;
	}

	bool bNowLand = false;

	// ステージに当たった判定
	m_bHitStage = false;
	for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
	{
		// オブジェクト取得
		CObjectX *pObjX = pStage->GetObj(nCntStage);

		if (pObjX == NULL)
		{// NULLだったら
			continue;
		}

		// 高さ取得
		bool bLand = false;
		fHeight = pObjX->GetHeight(pos, bLand);

		if (bLand == true && fHeight > pos.y)
		{// 地面の方が自分より高かったら

		 // 地面の高さに補正
			if (pos.y + 50.0f <= fHeight)
			{// 自分より壁が高すぎる
				m_bHitWall = true;
			}
			else
			{
				pos.y = fHeight;
			}

			m_bHitStage = true;
			m_bLandField = false;

			if (bLand == true)
			{// 着地してたら

				if ((m_bKnockBack == true || m_bJump == true) && GetPosition().y >= fHeight)
				{
					m_bLandOld = true;
				}

				if (m_bJump == true)
				{// ジャンプ中だったら
					m_pMotion[PARTS_BODY]->ToggleFinish(true);
				}

				// ジャンプ使用可能にする
				m_bJump = false;
				move.y = 0.0f;
				bNowLand = true;

				for (int i = 0; i < PARTS_MAX; i++)
				{
					m_sMotionFrag[i].bJump = false;
				}
			}
		}
	}




	// オブジェクト取得
	CObjectX *pObjX = pStage->GetInjectionTable();

	// 高さ取得
	bLand = false;
	fHeight = pObjX->GetHeight(pos, bLand);

	if (bLand == true && fHeight > pos.y)
	{// 地面の方が自分より高かったら

		// 地面の高さに補正
		if (pos.y + 50.0f <= fHeight)
		{// 自分より壁が高すぎる
			m_bHitWall = true;
		}
		else
		{
			pos.y = fHeight;
		}

		m_bHitStage = true;
		m_bLandField = false;

		if (bLand == true)
		{// 着地してたら

			if ((m_bKnockBack || m_bJump == true) && GetPosition().y >= fHeight)
			{
				m_bLandOld = true;
			}

			if (m_bJump == true)
			{// ジャンプ中だったら
				m_pMotion[PARTS_BODY]->ToggleFinish(true);
			}

			// ジャンプ使用可能にする
			m_bJump = false;
			move.y = 0.0f;
			bNowLand = true;
			
			for (int i = 0; i < PARTS_MAX; i++)
			{
				m_sMotionFrag[i].bJump = false;
			}

			// 射出台の着地判定
			m_bLandInjectionTable[m_nMyPlayerIdx] = true;

		}
	}

	int nLandNum = 0;
	for (int i = 0; i < CManager::GetInstance()->GetNumPlayer(); i++)
	{
		// 射出台の着地確認
		if (m_bLandInjectionTable[i] == true)
		{
			nLandNum++;
		}
	}

	// 全員着地状態設定
	if (nLandNum >= CManager::GetInstance()->GetNumPlayer())
	{
		m_bAllLandInjectionTable = true;
	}
	else
	{
		m_bAllLandInjectionTable = false;
	}

	if (CGame::GetGameManager()->GetType() == CGameManager::SCENE_MAINCLEAR &&
		pStage->GetInjectionTable()->GetState() != CInjectionTable::STATE_UP)
	{// 通常クリアだったら

		if (m_bAllLandInjectionTable == true)
		{// 全員射出台に乗っている

			// チャージ状態
			pStage->GetInjectionTable()->SetState(CInjectionTable::STATE_CHARGE);
		}
		else
		{
			// リセット
			pStage->GetInjectionTable()->SetState(CInjectionTable::STATE_NONE, 0);
		}
	}





	// 位置取得
	D3DXVECTOR3 posOld = GetPosition();

	//// 箱
	//float fLen = CGame::GetElevation()->GetWidthLen();
	//int nBlock = CGame::GetElevation()->GetWidthBlock();
	//nBlock /= 2;
	//if (pos.x + RADIUS >= fLen * nBlock) { pos.x = fLen * nBlock - RADIUS; }
	//if (pos.x - RADIUS <= -fLen * nBlock) { pos.x = -fLen * nBlock + RADIUS; }
	//if (pos.z + RADIUS >= fLen * nBlock) { pos.z = fLen * nBlock - RADIUS; }
	//if (pos.z - RADIUS <= -fLen * nBlock) { pos.z = -fLen * nBlock + RADIUS; }

	// 向き設定
	SetRotation(rot);

	return bNowLand;
}

//==========================================================================
// ヒット処理
//==========================================================================
bool CPlayerUnion::Hit(const int nValue)
{
	// 体力取得
	int nLife = 50;

	//if (nLife <= 0)
	//{
	//	// 死んだ
	//	return true;
	//}

	if (m_state != STATE_DMG && m_state != STATE_KNOCKBACK && m_state != STATE_INVINCIBLE && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{// ダメージ受付状態の時

		// 体力減らす
		nLife -= nValue;


		// ゲームパッド情報取得
		CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
		pInputGamepad->SetVibration(CInputGamepad::VIBRATION_STATE_DMG, 0);

		m_KnokBackMove.y += 18.0f;
		m_bHitStage = false;

		// 体力設定
		//SetLife(nLife);

		if (nLife <= 0)
		{// 体力がなくなったら

			// 死状態
			m_state = STATE_DEAD;

			m_KnokBackMove.y = 8.0f;

			// 遷移カウンター設定
			m_nCntState = DEADTIME;

			// 体力設定
			//SetLife(0);

			// ノックバック判定にする
			m_bKnockBack = true;

			// やられモーション
			m_pMotion[PARTS_BODY]->Set(MOTION_KNOCKBACK);

			// ノックバックの位置更新
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 rot = GetRotation();
			m_posKnokBack = pos;

			// 衝撃波生成
			CImpactWave::Create
			(
				D3DXVECTOR3(pos.x, pos.y + 80.0f, pos.z),	// 位置
				D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// 向き
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// 色
				80.0f,										// 幅
				80.0f,										// 高さ
				0.0f,										// 中心からの間隔
				20,											// 寿命
				10.0f,										// 幅の移動量
				CImpactWave::TYPE_GIZAWHITE,				// テクスチャタイプ
				false										// 加算合成するか
			);

			CManager::GetInstance()->SetEnableHitStop(18);

			// 振動
			CManager::GetInstance()->GetCamera()->SetShake(21, 30.0f, 0.0f);

			// 死んだ
			return true;
		}

		// 過去の状態保存
		m_Oldstate = m_state;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// ダメージ状態にする
		m_state = STATE_DMG;

		// 遷移カウンター設定
		m_nCntState = 0;

		// ノックバックの位置更新
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 rot = GetRotation();
		m_posKnokBack = pos;

		// ノックバック判定にする
		m_bKnockBack = true;

		// やられモーション
		m_pMotion[PARTS_BODY]->Set(MOTION_KNOCKBACK);

		// 衝撃波生成
		CImpactWave::Create
		(
			D3DXVECTOR3(pos.x, pos.y + 80.0f, pos.z),	// 位置
			D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// 向き
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// 色
			80.0f,										// 幅
			80.0f,										// 高さ
			0.0f,										// 中心からの間隔
			20,											// 寿命
			10.0f,										// 幅の移動量
			CImpactWave::TYPE_GIZAWHITE,				// テクスチャタイプ
			false										// 加算合成するか
		);

		CManager::GetInstance()->SetEnableHitStop(12);

		// 振動
		CManager::GetInstance()->GetCamera()->SetShake(12, 25.0f, 0.0f);

		// サウンド再生
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_PLAYERDMG);
	}

	// 死んでない
	return false;
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CPlayerUnion::UpdateState(void)
{
	switch (m_state)
	{
	case STATE_NONE:
		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case STATE_INVINCIBLE:
		Invincible();
		break;

	case STATE_DMG:
		Damage();
		break;

	case STATE_DEAD:
		Dead();
		break;

	case STATE_FADEOUT:
		FadeOut();
		break;

	case STATE_KNOCKBACK:
		KnockBack();
		break;
	}
}

//==========================================================================
// 無敵
//==========================================================================
void CPlayerUnion::Invincible(void)
{
	// 状態遷移カウンター減算
	m_nCntState--;

	if (m_nCntState % INVINCIBLE_INT == 0)
	{// 規定間隔
		
		// 色設定
		if (m_mMatcol.a == 1.0f)
		{
			m_mMatcol.a = 0.4f;
		}
		else
		{
			m_mMatcol.a = 1.0f;
		}
	}

	if (m_nCntState <= 0)
	{// 遷移カウンターが0になったら

		// なにもない状態にする
		m_state = STATE_NONE;
		m_nCntState = 0;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

//==========================================================================
// ダメージ
//==========================================================================
void CPlayerUnion::Damage(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 状態遷移カウンター減算
	m_nCntState++;

	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// 位置更新
	pos.y = (-0.4f * (float)(m_nCntState * m_nCntState) + m_KnokBackMove.y * (float)m_nCntState) + m_posKnokBack.y;
	pos.x += move.x;
	/*pos.x += move.x;
	pos.z += move.z;*/

	// 起伏との判定
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage) && m_nCntState >= 10)
	{// 地面と当たっていたら
		m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;
		m_KnokBackMove.y = 0.0f;	// 移動量ゼロ
		m_bLandOld = true;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ノックバック判定消す
		m_bKnockBack = false;
		m_pMotion[PARTS_BODY]->ToggleFinish(true);


		// Xファイルとの判定
		CStage *pStage = CGame::GetStage();
		if (pStage == NULL)
		{// NULLだったら
			return;
		}

		// ステージに当たった判定
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// オブジェクト取得
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == NULL)
			{// NULLだったら
				continue;
			}

			// 高さ取得
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}


	}
	else if (m_nCntState >= 30)
	{// 遷移カウンターが30になったら

		// ノックバック状態にする
		m_state = STATE_KNOCKBACK;
	}


	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// 死亡
//==========================================================================
void CPlayerUnion::Dead(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 位置更新
	if (m_nCntState > 0)
	{
		int nCnt = DEADTIME - m_nCntState;
		pos.y = (-0.1f * (float)(nCnt * nCnt) + m_KnokBackMove.y * (float)nCnt) + m_posKnokBack.y;
		pos.x += move.x;
	}

	// 状態遷移カウンター減算
	m_nCntState--;

	// 起伏との判定
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage) && m_nCntState >= 10)
	{// 地面と当たっていたら

		//// フェードを設定する
		//CManager::GetInstance()->GetInstantFade()->SetFade();

		//if (CManager::GetInstance()->GetInstantFade()->GetState() == CInstantFade::STATE_FADECOMPLETION)
		//{// フェード完了した時



		//}

		m_state = STATE_FADEOUT;	// フェードアウト
		m_nCntState = FADEOUTTIME;
		m_KnokBackMove.y = 0.0f;	// 移動量ゼロ
		m_bLandOld = true;
		move.x = 0.0f;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ノックバック判定消す
		m_bKnockBack = false;
		m_bDead = true;
		//m_pMotion->ToggleFinish(true);

		// ぶっ倒れモーション
		m_pMotion[PARTS_BODY]->Set(MOTION_DEAD);

		// Xファイルとの判定
		CStage *pStage = CGame::GetStage();
		if (pStage == NULL)
		{// NULLだったら
			return;
		}

		// ステージに当たった判定
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// オブジェクト取得
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == NULL)
			{// NULLだったら
				continue;
			}

			// 高さ取得
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// フェードアウト
//==========================================================================
void CPlayerUnion::FadeOut(void)
{

	// 状態遷移カウンター減算
	m_nCntState--;

	// 色設定
	m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	if (m_nCntState <= 0)
	{// 遷移カウンターが0になったら

		// モード設定
		//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);

		// 死亡処理
		Kill();

		// 終了処理
		Uninit();
		return;
	}
}

//==========================================================================
// ノックバック
//==========================================================================
void CPlayerUnion::KnockBack(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 現在と目標の差分
	float fRotDiff = 0.0f;

	// 距離の判定
	bool bLen = false;


	// 状態遷移カウンター減算
	m_nCntState++;

	// 位置更新
	pos.y = (-0.4f * (float)(m_nCntState * m_nCntState) + m_KnokBackMove.y * (float)m_nCntState) + m_posKnokBack.y;
	pos.x += move.x;
	/*pos.x += move.x;
	pos.z += move.z;*/

	// 起伏との判定
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage))
	{// 地面と当たっていたら
		m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;
		m_KnokBackMove.y = 0.0f;	// 移動量ゼロ
		m_bLandOld = true;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		// ノックバック判定消す
		m_bKnockBack = false;
		//m_pMotion[PARTS_BODY]->ToggleFinish(true);

		// Xファイルとの判定
		CStage *pStage = CGame::GetStage();
		if (pStage == NULL)
		{// NULLだったら
			return;
		}

		// ステージに当たった判定
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// オブジェクト取得
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == NULL)
			{// NULLだったら
				continue;
			}

			// 高さ取得
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}
	}


	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerUnion::Draw(void)
{

	/*if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE || m_state == STATE_FADEOUT)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else
	{
		CObjectChara::Draw();
	}*/

	for (int i = 0; i < PARTS_MAX; i++)
	{
		if (m_pObjChara[i] == NULL)
		{
			continue;
		}

		// 攻撃処理
		//m_pObjChara[i]->Draw();		// 胴体
	}

	// HPゲージ
	if (m_pHPGauge != NULL)
	{
		m_pHPGauge->Draw();
	}
}

//==========================================================================
// 複数キャラクター読み込み
//==========================================================================
void CPlayerUnion::ReadMultiCharacter(const char *pTextFile)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pTextFile, "r");

	if (pFile == NULL)
	{//ファイルが開けた場合
		return;
	}

	char aComment[MAX_COMMENT];	// コメント

	std::string CharacterFile[mylib_const::MAX_PLAYER];
	int nCntFileName = 0;
	int nNumModel = 0;

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル数の設定
		if (strcmp(aComment, "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &nNumModel);	// モデル数
		}

		while (nCntFileName != nNumModel)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(aComment, "MOTION_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// ファイル名保存
				CharacterFile[nCntFileName] = aComment;


				//**********************************
				// キャラクター生成
				//**********************************
				m_pObjChara[nCntFileName] = CObjectChara::Create(CharacterFile[nCntFileName]);
				if (m_pObjChara[nCntFileName] == NULL)
				{// 失敗していたら
					return;
				}
				m_pObjChara[nCntFileName]->SetType(CObject::TYPE_OBJECTX);

				// モーションの生成処理
				m_pMotion[nCntFileName] = CMotion::Create(CharacterFile[nCntFileName]);

				// オブジェクトキャラクターの情報取得
				CObjectChara *pObjChar = m_pObjChara[nCntFileName]->GetObjectChara();

				// モーションの設定
				m_pMotion[nCntFileName]->SetModel(pObjChar->GetModel(), pObjChar->GetNumModel(), pObjChar);

				// ポーズのリセット
				m_pMotion[nCntFileName]->ResetPose(MOTION_DEF);


				nCntFileName++;	// ファイル数加算
			}
		}

		// 各パーツの設定
		if (strcmp(aComment, "PARENTSET") == 0)
		{// 親設定の読み込みを開始

			int nFileNumber = -1, nModelIdx = -1, nParentFileNumber = -1, nParentModelIdx = -1;

			while (strcmp(aComment, "END_PARENTSET") != 0)
			{// END_PARENTSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	//確認する

				if (strcmp(aComment, "FILENUMBER") == 0)
				{// FILENUMBERで設定するキャラクターファイル番号読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &nFileNumber);	// キャラクターファイル番号
				}

				if (strcmp(aComment, "MODELINDEX") == 0)
				{// MODELINDEXで設定するモデル番号読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &nModelIdx);	// 設定するモデル番号
				}

				if (strcmp(aComment, "PARENT_FILENUMBER") == 0)
				{// PARENT_FILENUMBERで親にするキャラクターファイル番号読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &nParentFileNumber);	// キャラクターファイル番号
				}

				if (strcmp(aComment, "PARENT_MODELINDEX") == 0)
				{// PARENT_MODELINDEXで親にするモデル番号読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &nParentModelIdx);	// 設定するモデル番号
				}
			}// END_PARENTSETのかっこ

			// 原点設定
			m_pObjChara[nFileNumber]->GetModel()[nModelIdx]->SetParent(m_pObjChara[nParentFileNumber]->GetModel()[nParentModelIdx]);
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

}

//==========================================================================
// プレイヤー毎のパーツインデックス番号設定
//==========================================================================
void CPlayerUnion::SetPlayerByPartsIdx(int nPartsIdx, int nPlayerIdx)
{
	m_nPartsIdx[nPartsIdx] = nPlayerIdx;
	m_nInputSuperAtkIdx = nPlayerIdx;
}

//==========================================================================
// 移動の操作するインデックス番号設定
//==========================================================================
void CPlayerUnion::SetControllMoveIdx(int nIdx)
{
	m_nControllMoveIdx = nIdx;
}

//==========================================================================
// 状態取得
//==========================================================================
int CPlayerUnion::GetState(void)
{
	return m_state;
}