//=============================================================================
// 
//  �V�[���w�b�_�[ [scene.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "elevation.h"

//==========================================================================
// �O���錾
//==========================================================================
class CXLoad;
class CPlayer;
class CCamera;

//==========================================================================
// �N���X��`
//==========================================================================
// �V�[���N���X��`
class CScene
{
public:
	// ���[�h�̗񋓌^��`
	enum MODE
	{
		MODE_NONE = 0,		// �Ȃ�
		MODE_TITLE,			// �^�C�g��
		MODE_DECIDEPLAYER,	// �v���C���[����
		MODE_TUTORIAL,		// �`���[�g���A��
		MODE_GAME,			// �Q�[��
		MODE_RESULT,		// ���U���g
		MODE_RANKING,		// �����L���O
		MODE_MAX
	};

	CScene();
	~CScene();

	// �I�[�o�[���C�h����֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void ResetScene(void);	// �V�[���̃��Z�b�g
	static CScene *Create(MODE mode);
	static CXLoad *GetXLoad(void);			// X�t�@�C���̃f�[�^�擾
	static CElevation *GetElevation(void);	// �I�u�W�F�N�g3D�̎擾
	CPlayer **GetPlayer(void);				// �v���C���[�̎擾
	CPlayer *GetPlayer(int nIdx);			// �v���C���[�̎擾
	void UninitPlayer(int nIdx);			// �v���C���[�̏��擾
	MODE GetMode(void);						// ���݂̃��[�h�擾

protected:

private:

	MODE m_mode;				// ���݂̃��[�h
	CPlayer *m_pPlayer[mylib_const::MAX_PLAYER];		// �v���C���[�̃I�u�W�F�N�g
	static CXLoad *m_pXLoad;							// X�t�@�C���̃I�u�W�F�N�g
	static CElevation *m_pObject3DMesh;					// �I�u�W�F�N�g3D���b�V���̃I�u�W�F�N�g
};



#endif