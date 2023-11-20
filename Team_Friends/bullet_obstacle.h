//=============================================================================
// 
//  ��Q���e�w�b�_�[ [bullet_obstacle.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BULLET_OBSTACLE_H_
#define _BULLET_OBSTACLE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "objectX.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ��Q���e�N���X��`
class CBulletObstacle : public CObjectX
{
public:

	CBulletObstacle(int nPriority = 6);
	~CBulletObstacle();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CBulletObstacle *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const float fSize);

private:
	void UpdatePos(void);		// �ړ�
	void CollisionPlayer(void);		// �v���C���[�Ƃ̔���

	int m_nLife;			// ����
	int m_nLifeMax;			// �����̍ő�l
	int m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nCntEmission;	// �������̃J�E���^�[
	float m_fRadius;		// ���a

};


#endif