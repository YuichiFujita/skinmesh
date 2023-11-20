//=============================================================================
// 
//  ���b�V���V�����_�[�w�b�_�[ [meshcylinder.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���V�����_�[�N���X��`
class CMeshCylinder : public CObject3DMesh
{
public:
	CMeshCylinder(int nPriority = 2);
	~CMeshCylinder();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	static CMeshCylinder *Create();
	static CMeshCylinder *Create(const char *aFileName = NULL);
	CMeshCylinder *GetMyObject(void);

private:
	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif