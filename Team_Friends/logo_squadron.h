//==========================================
//
//  戦隊のロゴ(logo_squadron.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _SQUADRON_H_
#define _SQUADRON_H_
#include "logo.h"

//==========================================
//  クラス定義
//==========================================
class CLogo_Sqou : public CLogo
{
public:

	//メンバ関数
	CLogo_Sqou();
	~CLogo_Sqou();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	//静的メンバ関数
	static CLogo_Sqou* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:

	//メンバ変数
	int m_nCntFlash;
	float m_fTime;

	//静的メンバ変数
	static const float m_fFlashTime;
	static const int m_nFlashNum;

};

#endif
