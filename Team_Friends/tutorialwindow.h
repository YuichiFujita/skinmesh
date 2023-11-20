//=============================================================================
// 
//  �`���[�g���A���E�B���h�E�w�b�_�[ [tutorialwindow.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TUTORIALWINDOW_H_
#define _TUTORIALWINDOW_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"
#include "tutorialstep.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CTutorialWindow : public CObject2D
{
public:

	CTutorialWindow(int nPriority = 7);
	~CTutorialWindow();

	static CTutorialWindow *Create(CTutorialStep::STEP step);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	
	// �����o�֐�

	// �����o�ϐ�
	int m_nCntWait;					// �ҋ@����
	CTutorialStep::STEP m_NowStep;	// ����̃X�e�b�v
	static const char *m_apTextureFile[CTutorialStep::STEP_MAX];	// �e�N�X�`���̃t�@�C��
	static int m_nTexIdx[CTutorialStep::STEP_MAX];	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif