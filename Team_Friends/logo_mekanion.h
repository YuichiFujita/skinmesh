//==========================================
//
//  メカニオンのロゴ(logo_mekanion.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MEKANION_H_
#define _MEKANION_H_
#include "logo.h"

//==========================================
//  クラス定義
//==========================================
class CLogo_Meka : public CLogo
{
public:

	//メンバ関数
	CLogo_Meka();
	~CLogo_Meka();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	//静的メンバ関数
	static CLogo_Meka* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:

	//メンバ変数
	D3DXVECTOR3 m_posVirtual;

};

#endif
