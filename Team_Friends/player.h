//=============================================================================
// 
//  �v���C���[�w�b�_�[ [player.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// ��d�C���N���[�h�h�~

#include "gamemanager.h"
#include "objectChara.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define SLIDEMOVE		(2.05f)			// ��̈ړ���

// �O���錾
class CMotion;
class CShadow;
class CTargetPoint;
class CHP_GaugePlayer;
class CObjectCircleGauge2D;

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�N���X��`
class CPlayer : public CObjectChara
{
public:

	// ��Ԓ�`
	enum STATE
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_INVINCIBLE,	// ���G
		STATE_DMG,			// �_���[�W
		STATE_KNOCKBACK,	// �m�b�N�o�b�N
		STATE_DEAD,			// ��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_ATTACK,		// �U������
		STATE_COMPACTUNION,	// �ȈՍ���
		STATE_RELEASEUNION,	// ���̉���
		STATE_MAX
	};

	CPlayer(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CPlayer();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	bool Hit(const int nValue);	// �q�b�g����
	virtual int GetState(void) override;

	void GiveStatus(CGameManager::eStatus status);	// �X�e�[�^�X�t�^
	void SetState(STATE state, int nCntState = 0);	// ��Ԑݒ�
	static CPlayer *Create(int nIdx);	// ����
	void UninitByMode(void);	// ���[�h�ʏI��
	virtual void Kill(void);	// ���S����

protected:
	// �񋓌^��`
	enum MOTION
	{
		MOTION_DEF = 0,			// �j���[�g�������[�V����
		MOTION_WALK,			// �ړ����[�V����
		MOTION_ATK,				// �U��
		MOTION_ATK2,			// �U��(�h��)
		MOTION_KNOCKBACK,		// ���ꃂ�[�V����
		MOTION_DEAD,			// ���S���[�V����
		MOTION_JUMP,			// �W�����v
		MOTION_FALL,			// ������
		MOTION_MAX
	};

	// ���[�V�����̔���
	struct SMotionFrag
	{
		bool bJump;			// �W�����v�����ǂ���
		bool bATK;			// �U�������ǂ���
		bool bKnockBack;	// �m�b�N�o�b�N�����ǂ���
		bool bDead;			// ���S�����ǂ���
		bool bMove;			// �ړ������ǂ���
	};

	// �X�e�[�^�X�̍\����
	struct sStatus
	{
		int nPower;			// �Η�
		int nSpeed;			// �쓮��
		int nLife;			// �ϋv
		float fPowerBuff;	// �Ἠo�t
		float fSpeedBuff;	// �쓮���o�t
		float fLifeBuff;	// �ϋv�o�t
	};

	bool Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &move);	// �����蔻��

	bool m_bJump;				// �W�����v�����ǂ���
	bool m_bLandOld;			// �ߋ��̒��n���
	bool m_bHitStage;			// �X�e�[�W�̓����蔻��
	bool m_bLandField;			// �t�B�[���h�̒��n����
	bool m_bHitWall;			// �ǂ̓����蔻��
	int m_nMyPlayerIdx;			// �v���C���[�C���f�b�N�X�ԍ�
	int m_nCntWalk;				// ���s�J�E���^�[
	int m_nCntInputAtk;			// �U���̓��̓J�E���^�[
	int m_nAtkLevel;			// �U���̒i�K
	STATE m_state;				// ���
	CMotion *m_pMotion;			// ���[�V�����̏��
	CShadow *m_pShadow;			// �e�̏��
	SMotionFrag m_sMotionFrag;	// ���[�V�����̃t���O
private:

	// �����o�֐�
	void UpdateState(void);	// ��ԍX�V����
	void KnockBack(void);	// �m�b�N�o�b�N
	void Damage(void);		// �_���[�W
	void Dead(void);		// ���S
	void FadeOut(void);		// �t�F�[�h�A�E�g
	void Invincible(void);	// ���G
	void StateCompactUnion(void);		// �ȈՍ���
	void StateReleaseUnion(void);		// ���̉���
	virtual void Controll(void);	// ����
	void MotionSet(void);	// ���[�V�����̐ݒ�
	void Atack(void);		// �U��

	sStatus m_sStatus;			// �X�e�[�^�X���
	STATE m_Oldstate;			// �O��̏��
	D3DXCOLOR m_mMatcol;		// �}�e���A���̐F
	D3DXVECTOR3 m_posKnokBack;	// �m�b�N�o�b�N�̈ʒu
	D3DXVECTOR3 m_KnokBackMove;	// �m�b�N�o�b�N�̈ړ���
	int m_nCntState;			// ��ԑJ�ڃJ�E���^�[
	int m_nTexIdx;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxXFile;			// X�t�@�C���̃C���f�b�N�X�ԍ�
	CTargetPoint *m_pTargetP;	// �ڕW�̒n�_
	static bool m_bAllLandInjectionTable;	// �S���̎ˏo�䒅�n����
	static bool m_bLandInjectionTable[mylib_const::MAX_PLAYER];	// �ˏo��̒��n����
	static const char *m_apModelFile[mylib_const::MAX_PLAYER];	// ���f���̃t�@�C��
	static int m_nChaseTopIdx;	// �Ǐ]�̐擪�C���f�b�N�X�ԍ�
};



#endif