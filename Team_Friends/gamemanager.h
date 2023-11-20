//=============================================================================
// 
//  �Q�[���}�l�[�W���w�b�_�[ [gamemanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���}�l�[�W���N���X��`
class CGameManager
{
public:

	// �V�[���̎��
	enum SceneType
	{
		SCENE_MAIN = 0,			// ���C��
		SCENE_MAINCLEAR,		// ���C���N���A
		SCENE_RUSH,				// ���b�V��
		SCENE_BOSS,				// �{�X
		SCENE_TRANSITIONWAIT,	// �J�ڑҋ@
		SCENE_TRANSITION,		// �J��
		SCENE_MAX
	};

	// �X�e�[�^�X����
	enum eStatus
	{
		STATUS_POWER = 0,	// �Η�
		STATUS_SPEED,		// �쓮��
		STATUS_LIFE,		// �ϋv
		STATUS_MAX
	};

	CGameManager();
	~CGameManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetType(SceneType type);	// �V�[���̎�ސݒ�
	SceneType GetType(void);		// �V�[���̎�ގ擾
	void SetEnableRush(void) { m_bEndRush = true; }		// ���b�V���̏I���t���O��L���ɂ���
	bool IsRushEnd(void) { return m_bEndRush; }			// �I���̃t���O�擾
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// ����t���O��؂�ւ�����
	bool IsControll(void) { return m_bControll; }				// ����̃t���O�擾
	bool IsEndNormalStage(void) { return m_bEndNormalStage; }	// �ʏ�X�e�[�W�̃A�I������
	void AddNowStage(void);	// �X�e�[�W�̉��Z
	int GetNowStage(void);	// ���݂̃X�e�[�W�擾
	void SetNumStage(int nStage) { m_nNumStage = nStage; }	// �X�e�[�W�̑����ݒ�
	int GetNumStage(void) { return m_nNumStage; }			// �X�e�[�W�̑����擾

	static CGameManager *Create(void);	// ��������

private:

	// �����o�֐�
	void SetBoss(void);
	void SetEnemy(void);

	// �����o�ϐ�
	SceneType m_SceneType;	// �V�[���̎��
	bool m_bEndRush;		// ���b�V�����I��������
	bool m_bControll;		// ����ł��邩
	bool m_bEndNormalStage;	// �ʏ�X�e�[�W���I��������
	int m_nNowStage;		// ���݂̃X�e�[�W
	int m_nNumStage;		// �X�e�[�W�̑���
};



#endif