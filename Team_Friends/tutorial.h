//=============================================================================
// 
//  �`���[�g���A���w�b�_�[ [tutorial.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CTutorialStep;
class CTutorialText;

//==========================================================================
// �N���X��`
//==========================================================================
// �`���[�g���A���N���X��`
class CTutorial : public CScene
{
public:

	CTutorial();
	~CTutorial();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

};



#endif