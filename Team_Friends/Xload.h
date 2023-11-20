//=============================================================================
// 
//  X�t�@�C���w�b�_�[ [Xload.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _XLOAD_H_
#define _XLOAD_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// X�t�@�C���N���X��`
class CXLoad
{
public:

	// �}�N����`
#define MAX_MAT				(512)		// �}�e���A���T�C�Y

	struct SXFile
	{
		D3DXVECTOR3 vtxMin;						// ���f���̍ŏ��l
		D3DXVECTOR3	vtxMax;						// ���f���̍ő�l
		BYTE *pVtxBuff;							// ���_�o�b�t�@�̃|�C���^
		LPD3DXMESH pMesh;						// ���b�V��(���_���)�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;					// �}�e���A���ւ̃|�C���^
		DWORD dwNumMat;							// �}�e���A���̐�
		D3DXMATERIAL pMatData[MAX_MAT];			// �}�e���A���̃f�[�^
		int nVtxNum;							// ���_��
		int nFaceNum;							// �ʂ̐�
		D3DXVECTOR3 *pVtxPos;					// ���_���W
		float fMaxVtxDistance;					// ���_�Ԃ̍ő勗��
		int *nIdxTexture;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
		char acFilename[mylib_const::MAX_STRING];				// �t�@�C����
		int nFileNameLen;						// �t�@�C�����̕�����
	};

	CXLoad();
	~CXLoad();

	HRESULT Init(void);
	void Uninit(void);
	int XLoad(const char *pFileName);

	SXFile *GetMyObject(int nIdx);
	static int GetNumAll(void);		// ����
protected:

private:
	static HRESULT Load(const char *pFileName);
	static void Unload(void);

	static int m_nNumAll;				// �ǂݍ��񂾑���
	static SXFile m_pXFile[mylib_const::MAX_OBJ];	// X�t�@�C���̏��
};



#endif