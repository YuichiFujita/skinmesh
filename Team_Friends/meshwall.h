//=============================================================================
// 
//  ���b�V���E�H�[���w�b�_�[ [meshwall.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MESHWALL_H_
#define _MESHWALL_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���t�B�[���h�N���X��`
class CMeshWall : public CObject3DMesh
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_GRASS,
		TYPE_MAX,
	}TYPE;

	CMeshWall(int nPriority = 1);
	~CMeshWall();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void SetVtx(void);

	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidthLen, float fHeightLen, int nWidth = 1, int nHeight = 1, TYPE type = TYPE_NONE, const char *aFileName = NULL);
	virtual CMeshWall *GetMyObject(void);
protected:

private:
	TYPE m_type;	// ���
	static const char *m_apFilename[TYPE_MAX];		//�t�@�C���ǂݍ���
	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif