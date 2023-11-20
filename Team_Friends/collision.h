//==========================================================================
//
// �R���W�����w�b�_�[[collision.h]
// Author�F���n�Ή�
//
//==========================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_	// ��d�C���N���[�h�h�~

#include "object.h"
#include "listmanager.h"

//==========================================================================
// ���X�g�\���N���X�̒�`
//==========================================================================
class CCollision
{
public:

	CCollision();	// �R���X�g���N�^
	~CCollision();	// �f�X�g���N�^

	enum eCollisionType
	{
		TYPE_SQUARE2D = 0,	// 2D��`
		TYPE_SQUARE3D,		// 3D��`
		TYPE_CIRCLE2D,		// 2D�~
		TYPE_CIRCLE3D,		// 3D�~
		TYPE_SPHERE,		// ��
		TYPE_CYLINDER,		// ��
		TYPE_MAX
	};


	static CCollision *Create(eCollisionType collisionType);	// ��������

private:
	CObject *m_pCollision;	// �����蔻��̃I�u�W�F�N�g
	//CListManager *m_pCollisionList[CObject::TYPE_MAX];	// �����蔻��̃I�u�W�F�N�g
};

//class CCollisionSquare2D : public CCollision
//{
//public:
//
//	CCollisionSquare2D();	// �R���X�g���N�^
//	~CCollisionSquare2D();	// �f�X�g���N�^
//
//
//	static CCollisionSquare2D *Create(void);	// ��������
//
//private:
//};



#endif