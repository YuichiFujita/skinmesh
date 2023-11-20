//==========================================
//
//  ���J�j�I���̃��S(logo_mekanion.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MEKANION_H_
#define _MEKANION_H_
#include "logo.h"

//==========================================
//  �N���X��`
//==========================================
class CLogo_Meka : public CLogo
{
public:

	//�����o�֐�
	CLogo_Meka();
	~CLogo_Meka();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�ÓI�����o�֐�
	static CLogo_Meka* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:

	//�����o�ϐ�
	D3DXVECTOR3 m_posVirtual;

};

#endif
