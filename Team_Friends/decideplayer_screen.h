//=============================================================================
// 
//  �v���C���[���߉�ʃw�b�_�[ [decideplayer_screen.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DECIDEPLAYER_SCREEN_H_
#define _DECIDEPLAYER_SCREEN_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D;

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[���߉�ʃN���X��`
class CDecidePlayerScreen : public CObject
{
public:

	CDecidePlayerScreen(int nPriority = 8);
	~CDecidePlayerScreen();

	static CDecidePlayerScreen *Create(void);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Delete(void);
private:
	// �񋓌^��`
	enum VTX
	{
		VTX_TEXT = 0,	// �e�L�X�g
		VTX_MAX
	};

	// �񋓌^��`
	enum VTXSELECT
	{
		VTXSELECT_SELECT1P = 0,	// 1P
		VTXSELECT_SELECT2P,		// 2P
		VTXSELECT_SELECT3P,		// 3P
		VTXSELECT_SELECT4P,		// 4P
		VTXSELECT_MAX
	};

	// �����o�֐�
	void UpdateSelect(int nCntSelect);	// �I�����̍X�V����

	// �����o�ϐ�
	int m_nCntAlpha;		// �s�����x�̃J�E���^�[
	int m_nNowSelect;		// ���݂̑I����
	int m_nTexIdx[VTX_MAX];						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nTexIdx_Select[VTXSELECT_MAX];						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	CObject2D *m_pObj2D[VTX_MAX];					// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	CObject2D *m_pSelect2D[VTXSELECT_MAX];				// �I�����̃I�u�W�F�N�g
	static const char *m_apTextureFile[VTX_MAX];	// �e�N�X�`���̃t�@�C��
	static const char *m_apTextureFile_Select[VTXSELECT_MAX];	// �e�N�X�`���̃t�@�C��
};



#endif