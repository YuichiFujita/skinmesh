//==========================================
//
//  ���S�N���X(logo.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _LOGO_H_
#define _LOGO_H_
#include "object3D.h"

//==========================================
//  �N���X��`
//==========================================
class CLogo : public CObject3D
{
public:

	//�����o�֐�
	CLogo();
	~CLogo();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool GetComplete() const { return m_bComplete; } //������Ԃ̎擾

protected:

	//�����o�֐�
	void SetComplete(bool bComp) { m_bComplete = bComp; };

	//�ÓI�����o�ϐ�
	static const char* m_apTextureFile[]; // �e�N�X�`���̃t�@�C��

private:

	//�����o�ϐ�
	bool m_bComplete;

};

#endif
