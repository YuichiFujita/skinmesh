//=============================================================================
// 
//  �Q�̓G�w�b�_�[ [enemy_crowd.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENEMYCROWD_H_
#define _ENEMYCROWD_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "enemy.h"

// �O���錾

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�̓G�N���X��`
class CEnemyCrowd : public CEnemy
{
public:

	CEnemyCrowd(int nPriority = mylib_const::ENEMY_PRIORITY);
	~CEnemyCrowd();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Kill(void) override;

	CEnemyCrowd *GetEnemy(void);
protected:

private:
	virtual void ProcessLanding(void) override;	// ���n������
	virtual void AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo) override;	// �U��������
	virtual void ChaseMove(float fMove);	// �ǂ��|���ړ�
	void ChangeToAttackState(void);			// �U����Ԉڍs����
	void StateAttack(void) override;		// �U������
	void MotionSet(void) override;		// ���[�V�����̐ݒ�

	// ��ԍX�V�n
	void Spawn(void) override;		// �X�|�[��

	typedef enum
	{
		MOTION_DEF = 0,	// �j���[�g�������[�V����
		MOTION_WALK,	// �ړ����[�V����
		MOTION_ATK,		// �U��
		MOTION_KNOCKBACK,		// ���ꃂ�[�V����
		MOTION_FADEOUT,		// �A�҃��[�V����
		MOTION_MAX
	}MOTION;
};



#endif