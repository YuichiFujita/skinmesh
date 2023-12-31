//=============================================================================
// 
//  数字ヘッダー [multinumber.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MULTINUMBER_H_
#define _MULTINUMBER_H_	// 二重インクルード防止

#include "main.h"
#include "object2D.h"
#include "number.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CNumber;

//==========================================================================
// クラス定義
//==========================================================================
// 背景クラス定義
class CMultiNumber
{
public:

	// 寄せ種類
	enum AlignmentType
	{
		ALIGNMENT_RIGHT = 0,	// 右寄せ
		ALIGNMENT_MAX
	};

	CMultiNumber(int nPriority = 6);
	~CMultiNumber();

	static CMultiNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, bool bDigitDraw = false, int nPriority = 6);
	static CMultiNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, const char *pTextureFile, bool bDigitDraw = false, int nPriority = 6);

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void AddNumber(int nValue);
	void SetValue(void);		// 値の設定処理
	void SetValue(int nValue);	// 値の設定処理(オーバーロード)
	int GetValue(void);			// 値の取得処理
	void SetPosition(const D3DXVECTOR3 pos);	// 位置設定
	D3DXVECTOR3 GetPosition(void) const;		// 位置取得
	void SetColor(const D3DXCOLOR col);			// 色設定
	D3DXCOLOR GetColor(void) const;				// 色取得
	void Release(void);	// 開放処理

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;				// 位置
	D3DXCOLOR m_col;				// 色
	D3DXVECTOR2 size;				// 数字のサイズ
	int m_nNum;						// 数字
	int m_nNumNumber;				// 数字の数
	int m_nTexIdx;					// テクスチャのインデックス番号
	int m_nPriority;				// 優先順位
	bool m_bDigitDraw;				// 桁数描画
	CNumber **m_ppMultiNumber;		// 数字のオブジェクト
	CNumber::EObjectType m_objType;	// オブジェクトの種類

};

#endif