//=============================================================================
// 
//  ���K�͍��̃v���C���[�w�b�_�[ [union_legtoarm.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _UNION_LEGTOARM_H_
#define _UNION_LEGTOARM_H_	// ��d�C���N���[�h�h�~

#include "player_union.h"

// �O���錾

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�N���X��`
class CUnion_LegtoArm : public CPlayerUnion
{
public:

	CUnion_LegtoArm(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CUnion_LegtoArm();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Hit(const int nValue) override;	// �q�b�g����

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

	virtual void AttackAction(int nIdx, int nModelNum, CMotion::AttackInfo ATKInfo);	// �U��������
	virtual void ControllParts(void);	// �p�[�c�̃R���g���[������
	virtual void MotionSet(int nIdx);	// ���[�V�����̐ݒ�
	virtual HRESULT CreateParts(void) override;	// �p�[�c�̐ݒ�
private:

	// �R���{�����U�̎��
	enum eSPSkill
	{
		SKILL_MAX
	};

	// �����o�֐�
	void ControllLeg(int nIdx);			// �r����
	void ControllATK(int nIdx);		// �U������
};



#endif