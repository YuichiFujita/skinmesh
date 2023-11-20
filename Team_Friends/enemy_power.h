//=============================================================================
// 
//  �͂̓G�w�b�_�[ [enemy_power.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENEMYPOWER_H_
#define _ENEMYPOWER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "enemy.h"

// �O���錾

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�N���X��`
class CEnemyPower : public CEnemy
{
public:

	CEnemyPower(int nPriority = mylib_const::ENEMY_PRIORITY);
	~CEnemyPower();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Kill(void) override;

	CEnemyPower *GetEnemy(void);
protected:

private:
	virtual void ProcessLanding(void) override;	// ���n������
	virtual void AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo) override;	// �U��������
	virtual void ChaseMove(float fMove);	// �ǂ��|���ړ�
	void ChangeToAttackState(void);			// �U����Ԉڍs����
	void MotionSet(void) override;		// ���[�V�����̐ݒ�
	
	// ��ԍX�V�n
	void Spawn(void) override;		// �X�|�[��

	enum MOTION
	{
		MOTION_DEF = 0,	// �j���[�g�������[�V����
		MOTION_WALK,	// �ړ����[�V����
		MOTION_ATK,		// �U��
		MOTION_KNOCKBACK,		// ���ꃂ�[�V����
		MOTION_DIRECTATK,		// �U��
		MOTION_MAX
	};

	// �U���̎��
	enum ATTACKTYPE
	{
		ATTACKTYPE_IMPACTWAVE = 0,	// �~�`�g
		ATTACKTYPE_DIRECTWAVE,		// �����g
		ATTACKTYPE_MAX
	};

	// �����o�ϐ�
	ATTACKTYPE m_attackType;	// �U���̎��

};



#endif