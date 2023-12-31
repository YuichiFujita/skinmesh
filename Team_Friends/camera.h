//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERA_H_		// このマクロ定義がされていなかったら
#define _CAMERA_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラクラス定義
class CCamera
{
private:

	// 列挙型定義
	enum STATE
	{
		CAMERASTATE_NONE = 0,	// 通常
		CAMERASTATE_SHAKE,		// 振動
	};

public:

	// 追従の種類
	enum CHASETYPE
	{
		CHASETYPE_NORMAL = 0,	// 通常
		CHASETYPE_MAP,			// マップに追従
		CHASETYPE_NONE,			// 追従なし
		CHASETYPE_MAX
	};

	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetRotation(const D3DXVECTOR3 rot);	// 向き設定
	D3DXVECTOR3 GetRotation(void) const;		// 向き取得
	D3DXVECTOR3 GetPositionV(void) const;		// カメラの位置取得
	D3DXVECTOR3 GetPositionR(void) const;		// カメラの注視点取得
	void SetOriginDistance(float fDistance);	// 元になるカメラの距離設定
	float GetOriginDistance(void);				// 元になるカメラの距離取得
	void SetDestRotation(const D3DXVECTOR3 rot);	// 目標の向き設定
	D3DXVECTOR3 GetDestRotation(void);				// 目標の向き取得
	void SetTargetPosition(const D3DXVECTOR3 pos);	// 追従目標の位置設定
	D3DXVECTOR3 GetTargetPosition(void);			// 追従目標の位置取得
	void SetTargetRotation(const D3DXVECTOR3 rot);	// 追従目標の向き設定
	D3DXVECTOR3 GetTargetRotation(void);			// 追従目標の向き取得

	void SetShake(int nTime, float fLength, float fLengthY);	// 振動設定
	void SetLenDest(float fLength, int nCntTime = 120, float DecrementValue = 2.0f, float fCorrection = 0.1f);	// 目標の長さ設定
	void SetPlayerChaseIndex(int nIdx);				// 追従するプレイヤーのインデックス番号設定
	int GetPlayerChaseIndex(void);					// 追従するプレイヤーのインデックス番号取得
	void SetEnableFollow(bool bFollow);	// 追従の判定設定
	bool IsFollow(void);				// 追従状態取得
	void SetViewPort(D3DXVECTOR3 pos, D3DXVECTOR2 size);		// ビューポートの設定
	void Reset(CScene::MODE mode);	// リセット
	void ResetBoss(void);	// リセット
private:

	// メンバ変数
	void UpdateByMode(void);	// モード別更新処理
	void MoveCameraInput(void);
	void MoveCameraStick(void);
	void MoveCameraMouse(void);
	void MoveCameraVR(void);
	void MoveCameraV(void);
	void MoveCameraR(void);
	void MoveCameraDistance(void);
	void SetCameraV(void);
	void SetCameraVTitle(void);
	void SetCameraVGame(void);
	void SetCameraVResult(void);
	void SetCameraR(void);
	void SetCameraRTitle(void);
	void SetCameraRGame(void);
	void SetCameraRResult(void);
	void MoveCameraFollow(void);
	void Shake(void);
	void UpdateState(void);
	void UpdateSpotLightVec(void);

	// リセット
	void ResetGame(void);
	void ResetTitle(void);
	void ResetResult(void);
	void ResetRanking(void);

	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	D3DVIEWPORT9 m_viewport;		// ビューポート
	D3DXVECTOR3 m_posV;				// 視点(カメラの位置)
	D3DXVECTOR3 m_posR;				// 注視点(見たい場所)
	D3DXVECTOR3 m_posVDest;			// 目標の視点
	D3DXVECTOR3 m_posRDest;			// 目標の注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_rotVDest;			// 目標の視点の向き
	D3DXVECTOR3 m_TargetPos;		// 追従目標の位置
	D3DXVECTOR3 m_TargetRot;		// 追従目標の向き
	float m_fDistance;				// 距離
	float m_fDestDistance;			// 目標の距離
	float m_fOriginDistance;		// 元の距離
	float m_nShakeLength;			// 揺れの長さ
	float m_nShakeLengthY;			// Yの揺れの長さ
	float m_fMoveShake;				// 揺れの移動量
	float m_fMoveShakeY;			// Yの揺れの移動量
	float m_fHeightMaxDest;			// カメラの最大高さの目標
	float m_fHeightMax;				// カメラの最大高さ
	float m_fDiffHeight;			// 高さの差分
	float m_fDiffHeightSave;		// 高さの差分保存用
	float m_fDiffHeightDest;		// 目標の高さの差分
	float m_fChaseLerpStart;		// 追従補正の初期値
	float m_fChaseDistance;			// 追従の間隔
	float m_fDestChaseDistance;		// 目標の追従の間隔
	bool m_bFollow;					// 追従するかどうか
	int m_nCntState;				// 状態カウンター
	int m_nCntDistance;				// 距離カウンター
	int m_nOriginCntDistance;		// 元の距離カウンター
	float m_fDistanceCorrection;	// 距離の慣性補正係数
	float m_fDistanceDecrementValue;// 距離の減少係数
	STATE m_state;					// 状態
	CHASETYPE m_ChaseType;			// 追従の種類
	CHASETYPE m_OldChaseType;		// 前回の追従の種類
	int m_nCntChaseType;			// 追従のカウンター
	int m_nChasePlayerIndex;		// 追従するプレイヤーのインデックス番号
};

#endif