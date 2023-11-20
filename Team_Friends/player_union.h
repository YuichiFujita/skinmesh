//=============================================================================
// 
//  ���̃v���C���[�w�b�_�[ [player_union.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYER_UNION_H_
#define _PLAYER_UNION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "objectChara.h"
#include "motion.h"

//==========================================================================
// �}�N����`
//==========================================================================

// �O���錾
class CShadow;
class CTargetPoint;
class CHP_GaugePlayer;

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�N���X��`
class CPlayerUnion : public CObject
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
		STATE_MAX
	};

	// ���̎��
	enum eType
	{
		TYPE_ALL = 0,	// �S������
		TYPE_BODYtoLEG,	// ���~�r
		TYPE_BODYtoARM,	// ���~�r
		TYPE_LEGtoARM,	// �r�~�r
		TYPE_ARMtoARM,	// �r�~�r
		TYPE_MAX
	};

	enum eParts
	{
		PARTS_BODY = 0,
		PARTS_LEG,
		PARTS_R_ARM,
		PARTS_L_ARM,
		PARTS_MAX
	};

	CPlayerUnion(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CPlayerUnion();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual bool Hit(const int nValue);	// �q�b�g����
	int GetState(void);

	static CPlayerUnion *Create(eType type);	// ����
	void UninitByMode(void);
	void Kill(void);	// ���S����
	void SetPlayerByPartsIdx(int nPartsIdx, int nPlayerIdx);	// �v���C���[���̃p�[�c�C���f�b�N�X�ԍ��ݒ�
	void SetControllMoveIdx(int nIdx);	// �ړ��̑��삷��C���f�b�N�X�ԍ��ݒ�

protected:

	// �񋓌^��`
	enum MOTION
	{
		MOTION_DEF = 0,			// �j���[�g�������[�V����
		MOTION_WALK,			// �ړ����[�V����
		MOTION_ATK,				// �U��
		MOTION_CHARGE,			// �`���[�W
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
		bool bCharge;		// �`���[�W�����ǂ���
		bool bMove;			// �ړ������ǂ���
	};

	void ReadMultiCharacter(const char *pTextFile);			// �����L�����N�^�[�ǂݍ���
	bool Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &move);	// �����蔻��
	virtual void AttackAction(int nIdx, int nModelNum, CMotion::AttackInfo ATKInfo);	// �U��������
	virtual void ControllParts(void);	// �p�[�c�̃R���g���[������
	virtual void MotionSet(int nIdx);	// ���[�V�����̐ݒ�
	bool ControllMove(int nIdx);	// �ړ�����
	virtual HRESULT CreateParts(void);	// �p�[�c�̐ݒ�

	bool m_bJump;				// �W�����v�����ǂ���
	bool m_bLandOld;			// �ߋ��̒��n���
	bool m_bHitStage;			// �X�e�[�W�̓����蔻��
	bool m_bLandField;			// �t�B�[���h�̒��n����
	bool m_bHitWall;			// �ǂ̓����蔻��
	bool m_bKnockBack;			// �m�b�N�o�b�N�����ǂ���
	bool m_bDead;			// ���S�����ǂ���

	int m_nPartsIdx[mylib_const::MAX_PLAYER];	// �v���C���[���̃p�[�c�C���f�b�N�X�ԍ�
	int m_nMyPlayerIdx;			// �v���C���[�C���f�b�N�X�ԍ�
	int m_nControllMoveIdx;		// �ړ����삷���̃C���f�b�N�X�ԍ�
	int m_nCntWalk;				// ���s�J�E���^�[
	int m_nUnionLife;			// ���̎���
	int m_nCntInputAtk;			// �U���̓��̓J�E���^�[

	int m_nInputSuperAtkIdx;	// �K�E�Z���͂̃C���f�b�N�X
	int m_nIntervalAlternately;	// ���ݓ��͂̃C���^�[�o��
	int m_nCntInputSuperATK;	// �K�E�Z�̓��͉�

	D3DXCOLOR m_mMatcol;		// �}�e���A���̐F
	STATE m_Oldstate;			// �O��̏��
	STATE m_state;			// ���
	int m_nCntState;			// ��ԑJ�ڃJ�E���^�[
	D3DXVECTOR3 m_posKnokBack;	// �m�b�N�o�b�N�̈ʒu
	D3DXVECTOR3 m_KnokBackMove;	// �m�b�N�o�b�N�̈ړ���
	CMotion *m_pMotion[PARTS_MAX];			// �p�[�c���̃��[�V�����|�C���^
	SMotionFrag m_sMotionFrag[PARTS_MAX];	// ���[�V�����̃t���O
	CObjectChara *m_pObjChara[PARTS_MAX];	// �p�[�c���̃I�u�W�F�N�g�L�����N�^�[�|�C���^
private:

	// �R���{�����U�̎��
	enum eSPSkill
	{
		SKILL_MAX
	};

	// �����o�֐�
	void UpdateState(void);	// ��ԍX�V����
	void KnockBack(void);	// �m�b�N�o�b�N
	void Damage(void);		// �_���[�W
	void Dead(void);		// ���S
	void FadeOut(void);		// �t�F�[�h�A�E�g
	void Invincible(void);	// ���G
	void Controll(void);		// ����
	void ControllBody(int nIdx);		// ������
	void ControllLeg(int nIdx);			// �r����
	void ControllRightArm(int nIdx);	// �E�r����
	void ControllLeftArm(int nIdx);		// ���r����

	void Atack(int nIdx);		// �U��


	int m_nTexIdx;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxXFile;			// X�t�@�C���̃C���f�b�N�X�ԍ�
	float m_fRotDest;			// �ڕW�̌���
	CShadow *m_pShadow;			// �e�̏��
	CTargetPoint *m_pTargetP;	// �ڕW�̒n�_
	CHP_GaugePlayer *m_pHPGauge;	// HP�Q�[�W�̏��
	static bool m_bAllLandInjectionTable;	// �S���̎ˏo�䒅�n����
	static bool m_bLandInjectionTable[mylib_const::MAX_PLAYER];	// �ˏo��̒��n����
};



#endif