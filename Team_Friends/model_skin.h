//=============================================================================
//
//	3Dモデルクラス (model_skin.h)
//	Author : 唐�ｱ結斗
//	概要 : 3Dモデル生成を行う
//
//=============================================================================
//*****************************************************************************
//	二重インクルード防止
//*****************************************************************************
#ifndef _MODEL_SKIN_H_
#define _MODEL_SKIN_H_

//*****************************************************************************
//	インクルード
//*****************************************************************************
#include "object.h"
#include "main.h"

//*****************************************************************************
//	マクロ定義
//*****************************************************************************
#define SKIN_ANIME_SPEED	(60.0f / 4800.0f)	// モーションスピード

#define SAFE_RELEASE(p)			if (p) { (p)->Release(); (p) = nullptr; }	// Release関数のある場合の破棄マクロ
#define SAFE_DELETE(p)			if (p) { delete (p); (p) = nullptr; }		// 配列を使用しないdeleteを使用する破棄マクロ
#define SAFE_DELETE_ARRAY(p)	if (p) { delete[] (p); (p) = nullptr; }		// 配列を使用したdelete[]を使用する破棄マクロ

//*****************************************************************************
//	構造体定義
//*****************************************************************************
// 派生フレーム構造体 (それぞれのメッシュ用の最終ワールド行列を追加する）
struct MYFRAME : public D3DXFRAME
{
	// メンバ変数
	D3DXMATRIX CombinedTransformationMatrix;
	D3DXMATRIX OffsetMat;	// オフセット行列(インデックス付描画用)
	DWORD OffsetID;			// 行列テーブルのインデックス番号(インデックス付用)
};

// 派生メッシュコンテナー構造体
// (コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	// コンストラクタ
	MYMESHCONTAINER()
	{
		// メンバ変数をクリア
		ppBoneMatrix = NULL;		// 全てのボーンのワールド行列の先頭ポインター
		pBoneOffsetMatrices = NULL;	// フレームとしてのボーンのワールド行列のポインター
	}

	// メンバ変数
	LPDIRECT3DTEXTURE9* ppTextures;		// テクスチャへのポインタ
	DWORD dwWeight;						// 重みの個数（重みとは頂点への影響。）
	DWORD dwBoneNum;					// ボーンの数
	LPD3DXBUFFER pBoneBuffer;			// ボーン・テーブル
	D3DXMATRIX** ppBoneMatrix;			// 全てのボーンのワールド行列の先頭ポインター
	D3DXMATRIX* pBoneOffsetMatrices;	// フレームとしてのボーンのワールド行列のポインター
	LPD3DXMESH pOriMesh;				// オリジナルメッシュ用
	DWORD NumPaletteEntries;			// パレットサイズ

	// Work用
	std::vector<D3DXMATRIX> m_WorkBoneMatArray;

	// 影響するフレームへの参照配列。描画時にこのフレームの行列を使う。
	std::vector<MYFRAME*> BoneFrameArray;
};

//*****************************************************************************
//	クラス定義
//*****************************************************************************
// Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
// ID3DXAllocateHierarchyは派生すること想定して設計されている。
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	// コンストラクタ
	MY_HIERARCHY() {}

	STDMETHOD (CreateFrame) (THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD (CreateMeshContainer) (THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*, CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD (DestroyFrame) (THIS_ LPD3DXFRAME);
	STDMETHOD (DestroyMeshContainer) (THIS_ LPD3DXMESHCONTAINER);

private:
};

//=============================================================================
//	3Dモデルクラス
//	Author : 唐�ｱ結斗
//	概要 : 3Dモデル生成を行うクラス
//=============================================================================
// スキンメッシュクラス
class CSkinMesh
{
public:
	// メッシュクラスの初期化
	// VOID InitBase(SMESH_DATA_FILE* _pSMeshData);
	// メッシュの現在のMatrixデータ取得
	D3DXMATRIX GetMatrix();
	CSkinMesh();
	~CSkinMesh() {
		Release();
	}

	// スキンメッシュ内部処理
	HRESULT Init(LPDIRECT3DDEVICE9 lpD3DDevice, LPSTR pMeshPass);
	HRESULT AllocateBoneMatrix(LPD3DXFRAME pFrameRoot, LPD3DXMESHCONTAINER pMeshContainerBase);
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME pFrameRoot, LPD3DXFRAME pFrameBase);
	VOID RenderMeshContainer(LPDIRECT3DDEVICE9 lpD3DDevice, MYMESHCONTAINER*, MYFRAME*);
	VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	VOID DrawFrame(PDIRECT3DDEVICE9 lpD3DDevice, LPD3DXFRAME);

	// フレーム解放
	VOID FreeAnim(LPD3DXFRAME pFrame);

	// 生成処理
	CSkinMesh *Create(LPDIRECT3DDEVICE9 lpD3DDevice, LPSTR pMeshPass);

	// 解放処理
	VOID Release();

	// 更新処理
	VOID Update();

	// 描画処理
	VOID Draw();

	// オブジェクトのアニメーション変更( メッシュオブジェクトの操作用番号, 変更するアニメーション番号 )
	VOID ChangeAnim(DWORD NewAnimNum);

	// 現在のアニメーション番号取得
	DWORD GetAnimTrack() { return m_CurrentTrack; }

	// 現在のアニメーションタイム(アニメーション開始からの時間)を取得
	DWORD GetAnimTime() { return m_AnimeTime; }

	// アニメーション速度を取得
	FLOAT GetAnimSpeed() { return m_AnimSpeed; }

	// アニメーション速度を設定
	VOID SetAnimSpeed(FLOAT _AnimSpeed) { m_AnimSpeed = _AnimSpeed; }

private:

	// 対象のボーンを検索
	MYFRAME* SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame);

public:

	// ボーンのマトリックス取得 ( ボーンの名前 )
	D3DXMATRIX GetBoneMatrix(LPSTR _BoneName);

	// ボーンのマトリックスポインタ取得 ( ボーンの名前 )
	D3DXMATRIX* GetpBoneMatrix(LPSTR _BoneName);

private:

	// 追加
	// すべてのフレームポインタ格納処理関数
	VOID CreateFrameArray(LPD3DXFRAME _pFrame);

	// フレーム参照用配列
	std::vector<MYFRAME*> m_FrameArray;			// 全フレーム参照配列
	std::vector<MYFRAME*> m_IntoMeshFrameArray;	// メッシュコンテナありのフレーム参照配列

	// ボーン情報
	LPD3DXFRAME m_pFrameRoot;

	// アニメーションコントローラ
	LPD3DXANIMATIONCONTROLLER m_pAnimController;

	// ヒエラルキークラス変数
	MY_HIERARCHY m_cHierarchy;

	// アニメーションデータ格納用変数(ここは可変に変更したほうがいい)
	LPD3DXANIMATIONSET m_pAnimSet[20];

	// 現在のアニメーションが開始されてからの時間(1/60秒)
	DWORD m_AnimeTime;

	// アニメーションスピード
	FLOAT m_AnimSpeed;

	// 現在のアニメーショントラック
	DWORD m_CurrentTrack;

	// 現在のアニメーションデータトラック
	D3DXTRACK_DESC m_CurrentTrackDesc;

	// 進行方向
	D3DXMATRIX m_World;

	// メッシュのマテリアル関係
	// マテリアル変更フラグ
	BOOL m_MaterialFlg;

	// マテリアルデータ
	D3DMATERIAL9 m_Material;
};

#endif	// _MODEL_SKIN_H_
