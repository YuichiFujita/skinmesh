//==========================================
//
//  完成ロゴ(logo_complete.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _LOGO_COMPLETE_H_
#define _LOGO_COMPLETE_H_
#include "logo.h"

//==========================================
//  クラス定義
//==========================================
class CLogo_Comp : public CLogo
{
public:

	//メンバ関数
	CLogo_Comp();
	~CLogo_Comp();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	//静的メンバ関数
	static CLogo_Comp* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

};

#endif
