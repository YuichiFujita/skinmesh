//==========================================
//
//  MECHANIONのロゴ(logo_mechanion.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MECHANION_H_
#define _MECHANION_H_
#include "logo.h"

//==========================================
//  クラス定義
//==========================================
class CLogo_Mech : public CLogo
{
public:

	//メンバ関数
	CLogo_Mech();
	~CLogo_Mech();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	//静的メンバ関数
	static CLogo_Mech* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

};

#endif
