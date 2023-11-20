//==========================================
//
//  敵の試験(enemy_test.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _ENEMY_TEST_H_
#define _ENEMY_TEST_H_
#include "enemy.h"

//==========================================
//  クラス定義
//==========================================
class CEnemyTest : public CEnemy
{
public:

	//メンバ変数
	CEnemyTest(int nPriority = mylib_const::ENEMY_PRIORITY);
	~CEnemyTest();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Kill(void) override;
	void MotionSet(void) override;		// モーションの設定

private:

	enum MOTION
	{
		MOTION_DEF = 0,		// ニュートラルモーション
		MOTION_WALK,		// 移動モーション
		MOTION_ATK,			// 攻撃モーション
		MOTION_KNOCKBACK,	// やられモーション
		MOTION_FADEOUT,		// 帰還モーション
		MOTION_MAX
	};

};

#endif
