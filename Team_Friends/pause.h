//=============================================================================
// 
//  �|�[�Y�w�b�_�[ [pause.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PAUSE_H_
#define _PAUSE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �t�F�[�h�N���X��`
class CPause
{
public:

	CPause();
	~CPause();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPause(void);	// �g�p�󋵕ύX
	bool IsPause(void);		// �g�p�󋵎擾

	CObject2D **GetMyObject(void);
	static CPause *Create(void);

private:

	// �񋓌^��`
	enum VTX
	{
		VTX_FADE = 0,		// ������
		VTX_WINDOW,			// ���j���[�E�B���h�E
		VTX_CONTINUE,		// �Q�[���ɖ߂�
		VTX_RETRY,			// ���g���C
		VTX_QUIT,			// �^�C�g���ɖ߂�	
		VTX_MAX
	};

	// ���j���[
	typedef enum
	{
		MENU_RETURNGAME = 0,	// �|�[�Y�̉���
		MENU_RETRY,				// ���g���C
		MENU_RETURNTITLE,		// �^�C�g���ɖ߂�	
		MENU_MAX
	}MENU;

	void UpdateSelect(int nSelect);	// �I�����̍X�V

	CObject2D *m_aObject2D[VTX_MAX];				// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	int m_nTexIdx[VTX_MAX];							// �e�N�X�`���̃C���f�b�N�X�ԍ�
	bool m_bPause;									// �|�[�Y�̔���
	D3DXCOLOR m_col;								// �|�[�Y�̃J���[
	int m_nSelect;									// �I����
	static const char *m_apTextureFile[VTX_MAX];	// �e�N�X�`���̃t�@�C��
};

#endif