//=============================================================================
// 
//  �e�}�l�[�W���w�b�_�[ [bulletmanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BULLETMANAGER_H_
#define _BULLETMANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"
#include "bullet.h"

// �O���錾
class CBullet;

//==========================================================================
// �N���X��`
//==========================================================================
// �e�̃}�l�[�W���N���X��`
class CBulletManager
{
public:

	CBulletManager();
	~CBulletManager();

	HRESULT Init(void);
	void Uninit(void);

	static CBulletManager *Create(void);

	int Regist(CBullet *pBullet);	// ���蓖��
	void Delete(int nIdx);			// �폜
	int GetNumAll(void);			// �e�̑����擾
	CBullet **GetBullet(void);		// �e�̃I�u�W�F�N�g�擾
protected:


private:

	CBullet *m_apBullet[mylib_const::MAX_BULLET];	// �e�̃I�u�W�F�N�g
	int m_nNumAll;		// �e�̑���

};



#endif