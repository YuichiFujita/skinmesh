//==========================================
//
//  �^�C�g�����S�Ǘ��N���X(title_logo.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _TITLE_LOGO_H_
#define _TITLE_LOGO_H_
#include "object.h"

//==========================================
//  �O���錾
//==========================================
class CLogo_Mech;
class CLogo_Meka;
class CLogo_Sqou;
class CLogo_Comp;

//==========================================
//  �N���X��`
//==========================================
class CTitleLogo : public CObject
{
public:

	//�����o�֐�
	CTitleLogo();
	~CTitleLogo();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool GetComplete() { return m_bComplete; }

	//�ÓI�����o�֐�
	static CTitleLogo* Create();

private:

	//���S�̍X�V��ԗ�
	enum STATE
	{
		DEFAULT = 0, //�A���t�@�x�b�g�݂̂�������
		WAKE, //�J�^�J�i���N���オ��
		FLASH, //������_�ł���
		COMPLETE, //�����I�I
		MAX, //����
		NONE //NULLLLLLLLL
	};

	//�����o�ϐ�
	CLogo_Mech* m_pMech;
	CLogo_Meka* m_pMeka;
	CLogo_Sqou* m_pSqou;
	CLogo_Comp* m_pComp;
	STATE m_State;
	bool m_bComplete;

	//�����o�֐�
	void UpdateState();

};

#endif
