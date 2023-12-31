//=============================================================================
// 
//  オブジェクト円ゲージ(2D)ヘッダー [object_circlegauge2D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT_CIRCLEGAUGE2D_H_
#define _OBJECT_CIRCLEGAUGE2D_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト円ゲージ(2D)クラス定義
class CObjectCircleGauge2D : public CObject
{
public:

	CObjectCircleGauge2D(int nPriority = 7);
	~CObjectCircleGauge2D();

	// オーバーライドされた関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetVtx(void);

	void SetColor(const D3DXCOLOR col);	// 色設定
	D3DXCOLOR GetColor(void) const;		// 色取得
	void SetRate(const float fRate);	// 割合設定
	float GetRate(void) const;			// 割合取得

	void BindTexture(int nIdx);	// テクスチャのインデックス番号割り当て
	static CObjectCircleGauge2D *Create(int nPolygon, float fSize);
protected:

private:

	D3DXCOLOR m_col;	// 色
	float m_fSize;		// サイズ
	float m_fRate;		// 割合
	int m_nNumVertex;	// 頂点数
	int m_nTexIdx;		// テクスチャのインデックス番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
};

#endif