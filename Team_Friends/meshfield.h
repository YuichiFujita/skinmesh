//=============================================================================
// 
//  ���b�V���t�B�[���h�w�b�_�[ [meshfield.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���t�B�[���h�N���X��`
class CMeshField : public CObject3DMesh
{
public:
	typedef enum
	{
		TYPE_CONCREAT = 0,
		TYPE_LINE,
		TYPE_LONGLINE,
		TYPE_LINE_L,
		TYPE_LINE_R,
		TYPE_ODAN,
		TYPE_WALKLOAD_0,
		TYPE_WALKLOAD_1,
		TYPE_TILE_0,
		TYPE_TILE_1,
		TYPE_WATER,
		TYPE_MAX,
	}TYPE;

	CMeshField(int nPriority = 1);
	~CMeshField();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void SetVtx(void);

	static CMeshField *Create();
	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidthLen, float fHeightLen, int nWidth = 1, int nHeight = 1, TYPE type = TYPE_CONCREAT, const char *aFileName = NULL, int nPriority = mylib_const::DEFAULT_PRIORITY);
	virtual CMeshField *GetMyObject(void);
protected:

private:
	TYPE m_type;	// ���
	static const char *m_apFilename[TYPE_MAX];		//�t�@�C���ǂݍ���
	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif