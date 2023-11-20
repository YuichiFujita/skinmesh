//=============================================================================
// 
//  �������w�b�_�[ [meshsphere.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EXPLOSION_SPHERE_H_
#define _EXPLOSION_SPHERE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���X�t�B�A�N���X��`
class CMeshSphere : public CObject3DMesh
{
public:
	CMeshSphere(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CMeshSphere();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetSizeDest(float fSize);	// �ڕW�̃T�C�Y�ݒ�
	static CMeshSphere *Create(D3DXVECTOR3 pos, float fSize, const char *pFileName, int nPriority = mylib_const::DEF2D_PRIORITY);	// ��������
	static CMeshSphere *Create(D3DXVECTOR3 pos, float fSize, int nTexIdx, int nPriority = mylib_const::DEF2D_PRIORITY);	// ��������

private:

	// �����o�ϐ�
	int m_nTexIdx;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	float m_fSizeDest;	// �ڕW�̃T�C�Y

};



#endif