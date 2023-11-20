//=============================================================================
// 
//  �e�����w�b�_�[ [bulletexplosion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BULLETEXPLOSION_H_
#define _BULLETEXPLOSION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "meshsphere.h"

class CThunderRing;
class CObjectBillboard;

//==========================================================================
// �N���X��`
//==========================================================================
// �e�����N���X��`
class CBulletExplosion : public CMeshSphere
{
public:

	CBulletExplosion(int nPriority = 6);
	~CBulletExplosion();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void SetVtx(void);

	static int GetNumAll(void);
	static CBulletExplosion *Create(const D3DXVECTOR3 pos, const float fSize);

private:
	void UpdatePos(void);		// �ړ�
	void CollisionEnemy(void);	// �G�Ƃ̔���

	int m_nLife;			// ����
	int m_nLifeMax;			// �����̍ő�l
	int m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	float m_fDestSize;		// �ڕW�̃T�C�Y
	D3DXVECTOR3 rotmove;	// �����̈ړ���
	CThunderRing *m_pEffectThunderRing;	// ���̃����O�̃G�t�F�N�g
	CObjectBillboard *m_pBulletAppearance[mylib_const::BULLETAPPEARANCE_NUM];	// �����ڂ����̒e
	static int m_nNumAll;	// �e�̑���
};


#endif