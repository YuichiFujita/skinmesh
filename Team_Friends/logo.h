//==========================================
//
//  ロゴクラス(logo.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _LOGO_H_
#define _LOGO_H_
#include "object3D.h"

//==========================================
//  クラス定義
//==========================================
class CLogo : public CObject3D
{
public:

	//メンバ関数
	CLogo();
	~CLogo();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool GetComplete() const { return m_bComplete; } //完了状態の取得

protected:

	//メンバ関数
	void SetComplete(bool bComp) { m_bComplete = bComp; };

	//静的メンバ変数
	static const char* m_apTextureFile[]; // テクスチャのファイル

private:

	//メンバ変数
	bool m_bComplete;

};

#endif
