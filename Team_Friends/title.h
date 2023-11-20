//=============================================================================
// 
//  �^�C�g���w�b�_�[ [title.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "manager.h"

//==========================================
//  �O���錾
//==========================================
class CTitleLogo;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g���N���X��`
class CTitle : public CScene
{
public:

	CTitle();
	~CTitle();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	// �����o�֐�
	void WhiteOut();

	// �����o�ϐ�
	D3DXCOLOR m_col;
	float m_fLength;

	int m_nCntSwitch;		// �؂�ւ��̃J�E���^�[
	CTitleLogo *m_pLogo;

};



#endif