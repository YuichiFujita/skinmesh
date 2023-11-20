//=============================================================================
// 
//  �{�X�w�b�_�[ [enemy_boss.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENEMYBOSS_H_
#define _ENEMYBOSS_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "enemy.h"

// �O���錾

//==========================================================================
// �N���X��`
//==========================================================================
// �{�X�N���X��`
class CEnemyBoss : public CEnemy
{
public:

	CEnemyBoss(int nPriority = mylib_const::ENEMY_PRIORITY);
	~CEnemyBoss();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Kill(void) override;
	bool Hit(const int nValue) override;

	CEnemyBoss *GetEnemy(void);
protected:

private:
	typedef enum
	{
		MOTION_DEF = 0,		// �j���[�g�������[�V����
		MOTION_WALK,		// �ړ����[�V����
		MOTION_BULLETATK,	// �U��
		MOTION_ASSULTATK,	// �U��
		MOTION_CHILDSPAWN,	// �q���o��
		MOTION_STUN,		// �X�^��
		MOTION_KNOCKBACK,	// ���ꃂ�[�V����
		MOTION_FADEOUT,		// �A�҃��[�V����
		MOTION_APPEARANCE,	// �o�ꃂ�[�V����
		MOTION_MAX
	}MOTION;

	enum ATKTYPE
	{
		ATKTYPE_BULLET = 0,		// �e���ˍU��
		ATKTYPE_ASSULT,			// �ːi
		ATKTYPE_ENEMYSPAWN,		// �G�o��
		ATKTYPE_STUNKNOCKBACK,	// �X�^���m�b�N�o�b�N
		ATKTYPE_STUN,			// �X�^��
		ATKTYPE_APPEARANCE,		// �o��
		ATKTYPE_MAX
	};

	// �s���̍\���̒�`
	struct BOSSACT
	{
		int nBulletCnt;		// �e�̃J�E���^�[
		int nKnockBackCnt;	// �m�b�N���̃J�E���^
		int nAssultAngle;	// �ːi�̌���
		ATKTYPE AtkType;	// �U���̎��
		D3DXVECTOR3 StunPosDest;	// �X�^�����̖ڕW�̈ʒu
	};

	// �I�[�o�[���C�h���ꂽ�֐�
	void UpdateByType(void) override;	// ��ޕʍX�V����
	void ProcessLanding(void) override;	// ���n������
	void AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo) override;	// �U��������
	void StateAttack(void) override;	// �U������
	void MotionSet(void) override;		// ���[�V�����̐ݒ�
	void StateWait(void) override;		// �ҋ@����
	void PlayerChase(void) override;			// �v���C���[�Ǐ]
	void ChangeBase(void) override;			// ���_�؂�ւ�
	void CollisionPlayer(void) override;	// �v���C���[�Ƃ̓����蔻��

	// ��ԍX�V�n
	void Damage(void) override;		// �_���[�W
	void Spawn(void) override;		// �X�|�[��

	// �����o�֐�
	void ChaseMove(float fMove);		// �ǂ��|���ړ�
	void ChangeToAttackState(void);		// �U����Ԉڍs����
	void UpdateByAttack(void);			// �U���ʏ���
	void UpdateAppearance(void);		// �o��
	void UpdateAttackBullet(void);		// �e�U��
	void UpdateAttackAssult(void);		// �ːi�U��
	void UpdateChildSpawn(void);		// �q���o���U��
	void UpdateStun(void);				// �s���s���U��(?)
	void UpdateKnockBackStun(void);		// �X�^���̃m�b�N�o�b�N
	void DrawingACT(void);				// �s�����I
	void RotPlayer(void);				// �v���C���[�̕�������

	// �����o�ϐ�
	BOSSACT m_sAct;	// �{�X�̍s��
	eBaseType m_BaseType;	// ���_�̎��
	eBaseType m_BaseTypeDest;	// �ڕW�̋��_���
	int m_nCntDamage;			// �_���[�W�J�E���^�[
	int m_nCntEmission;			// �������̃J�E���^�[
};



#endif