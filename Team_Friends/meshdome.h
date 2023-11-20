//=============================================================================
// 
//  ���b�V���h�[���w�b�_�[ [meshdome.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MESHDOME_H_
#define _MESHDOME_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���h�[���N���X��`
class CMeshDome : public CObject3DMesh
{
public:
	CMeshDome(int nPriority = 2);
	~CMeshDome();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	static CMeshDome *Create();
	static CMeshDome *Create(const float fMove, const char *aFileName = NULL, const int nWidth = 16, const int nHeight = 8, int nPriority = mylib_const::DEFAULT_PRIORITY);
	CMeshDome *GetMyObject(void);

private:
	float m_fMove;	// ��]�ړ���
	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif