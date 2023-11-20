//==========================================
//
//  ����̃��S(logo_squadron.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _SQUADRON_H_
#define _SQUADRON_H_
#include "logo.h"

//==========================================
//  �N���X��`
//==========================================
class CLogo_Sqou : public CLogo
{
public:

	//�����o�֐�
	CLogo_Sqou();
	~CLogo_Sqou();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�ÓI�����o�֐�
	static CLogo_Sqou* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:

	//�����o�ϐ�
	int m_nCntFlash;
	float m_fTime;

	//�ÓI�����o�ϐ�
	static const float m_fFlashTime;
	static const int m_nFlashNum;

};

#endif
