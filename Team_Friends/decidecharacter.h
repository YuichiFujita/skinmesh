//=============================================================================
// 
//  �L�����N�^�[���߉�ʃw�b�_�[ [decidecharacter.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DECIDECHARACTER_H_
#define _DECIDECHARACTER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D;
class CCursor;

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �L�����N�^�[���߉�ʃN���X��`
class CDecideCharacter : public CObject
{
public:

	CDecideCharacter(int nPriority = 8);
	~CDecideCharacter();

	static CDecideCharacter *Create(void);

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
	enum VTXCHARACTER
	{
		VTXCHARACTER_1P = 0,	// 1P
		VTXCHARACTER_2P,		// 2P
		VTXCHARACTER_3P,		// 3P
		VTXCHARACTER_4P,		// 4P
		VTXCHARACTER_MAX
	};

	// �����o�֐�
	bool CollisionSelect(int nCntSelect);	// �I�����̍X�V����

	// �����o�ϐ�
	int m_nTexIdx[VTX_MAX];						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nTexIdx_Select[VTXCHARACTER_MAX];						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	CObject2D *m_pObj2D[VTX_MAX];					// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	CObject2D *m_pSelect2D[VTXCHARACTER_MAX];				// �I�����̃I�u�W�F�N�g
	CCursor *m_apCursor[mylib_const::MAX_PLAYER];	// �J�[�\���̃I�u�W�F�N�g
	bool m_bDecide[mylib_const::MAX_PLAYER];		// ���肵����
	bool m_bAllDecide;								// �S�Č��肵����
	static const char *m_apTextureFile[VTX_MAX];	// �e�N�X�`���̃t�@�C��
	static const char *m_apTextureFile_Select[VTXCHARACTER_MAX];	// �e�N�X�`���̃t�@�C��
};



#endif