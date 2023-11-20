//=============================================================================
// 
//  �`���[�g���A���v���C���[�w�b�_�[ [tutorialplayer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TUTORIALPLAYER_H_
#define _TUTORIALPLAYER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "player.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �`���[�g���A���v���C���[�N���X��`
class CTutorialPlayer : public CPlayer
{
public:

	CTutorialPlayer(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CTutorialPlayer();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Update(void) override;

	void UninitByMode(void);
private:

	// �I�[�o�[���C�h���ꂽ�֐�
	void Controll(void) override;	// ����

	// �����o�֐�
	void UpdateByStep(void);	// �X�e�b�v���Ƃ̍X�V����
	
};



#endif