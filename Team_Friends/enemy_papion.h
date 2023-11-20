//=============================================================================
// 
//  ��s�G�w�b�_�[ [enemy_papion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENEMYPAPION_H_
#define _ENEMYPAPION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "enemy.h"

// �O���錾

//==========================================================================
// �N���X��`
//==========================================================================
// ��s�G�N���X��`
class CEnemyPapion : public CEnemy
{
public:

	CEnemyPapion(int nPriority = mylib_const::ENEMY_PRIORITY);
	~CEnemyPapion();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Kill(void) override;

	CEnemyPapion *GetEnemy(void);
protected:

private:
	typedef enum
	{
		MOTION_DEF = 0,	// �j���[�g�������[�V����
		MOTION_WALK,	// �ړ����[�V����
		MOTION_ATK,		// �U��
		MOTION_KNOCKBACK,		// ���ꃂ�[�V����
		MOTION_FADEOUT,		// �A�҃��[�V����
		MOTION_MAX
	}MOTION;

	void ProcessLanding(void) override;	// ���n������
	void AttackAction(int nModelNum, CMotion::AttackInfo ATKInfo) override;	// �U��������
	void ChaseMove(float fMove);	// �ǂ��|���ړ�
	void ChangeToAttackState(void);			// �U����Ԉڍs����
	void StateAttack(void) override;		// �U������
	void MotionSet(void) override;		// ���[�V�����̐ݒ�

	// ��ԍX�V�n
	void Spawn(void) override;		// �X�|�[��
};



#endif