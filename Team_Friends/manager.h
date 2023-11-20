//=============================================================================
// 
//  �}�l�[�W���w�b�_�[ [manager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "scene.h"

// �}�N����`
#define MOVE_LRUP			(0.75f)	// �΂ߏ�ړ�
#define MOVE_LR				(0.5f)	// �����ړ�
#define MOVE_LRDW			(0.25f)	// �΂ߏ�ړ�

// �O���錾
class CRenderer;
class CLight;
class CCamera;
class CInputKeyboard;
class CInputGamepad;
class CInputMouse;
class CSound;
class CDebugProc;
class CPlayer;
class CScore;
class CTimer;
class CBG;
class CMap;
class CTexture;
class CXLoad;
class CEdit;
class CElevation;
class CEnemyManager;
class CFade;
class CInstantFade;
class CBlackFrame;
class CPause;
class CPowerGauge;
class CResultManager;
class CRankingManager;
class CEnemyFixedMoveManager;

//==========================================================================
// �N���X��`
//==========================================================================
// �}�l�[�W���N���X��`
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CManager *Create(void);		// ��������
	static CManager *GetInstance(void);	// �C���X�^���X�擾

	CRenderer *GetRenderer(void);
	CInputKeyboard *GetInputKeyboard(void);
	CInputGamepad *GetInputGamepad(void);
	CInputMouse *GetInputMouse(void);
	CSound *GetSound(void);
	CDebugProc *GetDebugProc(void);
	CLight *GetLight(void);
	CCamera *GetCamera(void);
	CTexture *GetTexture(void);
	CEdit *GetEdit(void);
	CFade *GetFade(void);
	CInstantFade *GetInstantFade(void);	// �J�ڂȂ��t�F�[�h�擾
	CBlackFrame *GetBlackFrame(void);	// ���t���[���擾
	CPause *GetPause(void);
	CResultManager *GetResultManager(void);	// ���U���g�}�l�[�W���擾
	CRankingManager *GetRankingManager(void);	// �����L���O�}�l�[�W���̃I�u�W�F�N�g
	CScene *GetScene(void);		// �V�[���擾
	CEnemyFixedMoveManager *GetFixedManager(void);	// ���̍s���}�l�[�W���擾

	int GetNumPlayer(void);			// �v���C���[�̐��擾
	void SetNumPlayer(int nNum);	// �v���C���[�̐��ݒ�
	int GetByPlayerPartsType(int nIdx);				// �v���C���[���̒S���p�[�c��ގ擾
	void SetByPlayerPartsType(int nIdx, int nType);	// �v���C���[���̒S���p�[�c��ސݒ�
	float GetDeltaTime(void);								// �o�ߎ��Ԏ擾
	void SetMode(CScene::MODE mode);					// ���̃��[�h�ݒ�
	CScene::MODE GetMode(void);							// ���݂̃��[�h�擾
	void SetEnableHitStop(int nCntHitStop);				// �q�b�g�X�g�b�v�̐ݒ�
	bool IsWireframe(void) { return m_bWireframe; }		// ���C���[�t���[��
	bool IsHitStop(void) { return m_bHitStop; }			// ���C���[�t���[��
	CScene::MODE GetOldMode(void) { return m_OldMode; }	// �O��̃��[�h�擾
private:

	CRenderer *m_pRenderer;				// �����_���[�̃I�u�W�F�N�g
	CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h�̃I�u�W�F�N�g
	CInputGamepad *m_pInputGamepad;		// �Q�[���p�b�h�̃I�u�W�F�N�g
	CInputMouse *m_pInputMouse;			// �}�E�X�̃I�u�W�F�N�g
	CSound *m_pSound;					// �T�E���h�̃I�u�W�F�N�g
	CDebugProc *m_pDebugProc;			// �f�o�b�O�\���̃I�u�W�F�N�g
	CLight *m_pLight;					// ���C�g�̃I�u�W�F�N�g
	CCamera *m_pCamera;					// �J�����̃I�u�W�F�N�g
	CTexture *m_pTexture;				// �e�N�X�`���̃I�u�W�F�N�g
	CEdit *m_pEdit;						// �G�f�B�b�g�̃I�u�W�F�N�g
	CScene *m_pScene;					// �V�[���̃I�u�W�F�N�g
	CFade *m_pFade;						// �t�F�[�h�̃I�u�W�F�N�g
	CInstantFade *m_pInstantFade;		// �J�ڂȂ��t�F�[�h�̃I�u�W�F�N�g
	CBlackFrame *m_pBlackFrame;			// ���t���[���̃I�u�W�F�N�g
	CPause *m_pPause;					// �|�[�Y�̃I�u�W�F�N�g
	CResultManager *m_pResultManager;	// ���U���g�}�l�[�W���̃I�u�W�F�N�g
	CRankingManager *m_pRankingManager;	// �����L���O�}�l�[�W���̃I�u�W�F�N�g
	CEnemyFixedMoveManager *m_pFixedMoveManager;	// ���̍s���}�l�[�W���̃I�u�W�F�N�g

	bool m_bWireframe;					// ���C���[�t���[��
	bool m_bHitStop;					// �q�b�g�X�g�b�v�̔���
	int m_nCntHitStop;					// �q�b�g�X�g�b�v�̃J�E���^�[
	CScene::MODE m_OldMode;				// �O��̃��[�h
	DWORD m_CurrentTime;				// ���ݎ���
	DWORD m_OldTime;					// �ߋ��̎���
	float m_fDeltaTime;					// �o�ߎ���
	int m_nNumPlayer;					// �v���C���[�̐�
	int m_nByPlayerPartsType[mylib_const::MAX_PLAYER];	// �v���C���[���̒S���p�[�c���

	static CManager *m_pManager;	// �}�l�[�W���̃I�u�W�F�N�g
};


#endif