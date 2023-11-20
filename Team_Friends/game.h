//=============================================================================
// 
//  �Q�[���w�b�_�[ [game.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "scene.h"
#include "gamemanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;
class CScore;
class CPowerGauge;
class CBulletManager;
class CEditEnemyBase;
class CStage;
class CHitScore;
class CStatusWindow;

class CEnemyManager;
class CPlayer;
class CEnemyBase;

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���N���X��`
class CGame : public CScene
{
public:

	// �G�f�B�b�g�^�C�v
	enum EEditType
	{
		EDITTYPE_OFF = 0,			// �I�t
		EDITTYPE_ENEMYBASE,			// �G���_
		EDITTYPE_MAX
	};

	CGame();
	~CGame();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Reset(void);
	static EEditType GetEditType(void) { return m_EditType; }
	static CScore *GetScore(void);
	static CEditEnemyBase *GetEditEnemyBase(void);
	static CBulletManager *GetBulletManager(void);
	static CStage *GetStage(void);
	static CGameManager *GetGameManager(void);
	static CStatusWindow *GetStatusWindow(int nIdx);

	static CEnemyManager *GetEnemyManager(void);
	static CEnemyBase *GetEnemyBase(void);		// �G�̋��_
	static bool IsEdit(void) { return m_bEdit; }	// �G�f�B�b�g�̔���

private:

	// �����o�֐�
	void EditReset(void);

	// �����o�ϐ�
	static CScore *m_pScore;					// �X�R�A�̃I�u�W�F�N�g
	static CBulletManager *m_pBulletManager;		// �e�}�l�[�W���̃I�u�W�F�N�g
	static CEditEnemyBase *m_pEditEnemyBase;		// �G�̋��_�G�f�B�^�[
	static CStage *m_pStage;	// �X�e�[�W�̃I�u�W�F�N�g
	static CGameManager *m_pGameManager;	// �Q�[���}�l�[�W���̃I�u�W�F�N�g
	static EEditType m_EditType;		// �G�f�B�b�g�̎��
	static CEnemyManager *m_pEnemyManager;		// �G�}�l�[�W���̃I�u�W�F�N�g
	static CEnemyBase *m_pEnemyBase;	// �G�̋��_
	static CStatusWindow *m_pStatusWindow[CGameManager::STATUS_MAX];	// �X�e�[�^�X�E�B���h�E�̃I�u�W�F�N�g
	static bool m_bEdit;				// �G�f�B�b�g�̔���
};



#endif