//==========================================
//
//  �G�̎���(enemy_test.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _ENEMY_TEST_H_
#define _ENEMY_TEST_H_
#include "enemy.h"

//==========================================
//  �N���X��`
//==========================================
class CEnemyTest : public CEnemy
{
public:

	//�����o�ϐ�
	CEnemyTest(int nPriority = mylib_const::ENEMY_PRIORITY);
	~CEnemyTest();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Kill(void) override;
	void MotionSet(void) override;		// ���[�V�����̐ݒ�

private:

	enum MOTION
	{
		MOTION_DEF = 0,		// �j���[�g�������[�V����
		MOTION_WALK,		// �ړ����[�V����
		MOTION_ATK,			// �U�����[�V����
		MOTION_KNOCKBACK,	// ���ꃂ�[�V����
		MOTION_FADEOUT,		// �A�҃��[�V����
		MOTION_MAX
	};

};

#endif
