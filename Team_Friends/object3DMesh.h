//=============================================================================
// 
//  �I�u�W�F�N�g3D�w�b�_�[ [object3DMesh.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3DMESH_H_
#define _OBJECT3DMESH_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g3D�N���X��`
class CObject3DMesh : public CObject3D
{
public:

	typedef enum
	{
		TYPE_FIELD = 0,	// �n��
		TYPE_WALL,		// ��
		TYPE_CYLINDER,	// �V�����_�[
		TYPE_DOME,		// �h�[��
		TYPE_DONUTS,	// �h�[�i�c
		TYPE_SPHERE,	// ��
		TYPE_MAX
	}TYPE;

	CObject3DMesh(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CObject3DMesh();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);
	void BindTexture(int nIdx);

	int GetNumIndex(void);		// �C���f�b�N�X��
	void SetNumIndex(int nWidth, int nHeight);
	int GetNumVertex(void);		// ���_��
	void SetNumVertex(int nWidth, int nHeight);
	D3DXVECTOR3 *GetVtxPos(void);		// ���_���W�擾
	void SetVtxPos(D3DXVECTOR3 *pos);	// ���_���W�擾
	D3DXVECTOR3 *GetVtxNor(void);		// ���_�x�N�g���擾
	void SetVtxNor(D3DXVECTOR3 *nor);	// ���_�x�N�g��
	D3DXCOLOR *GetVtxCol(void);			// ���_�J���[�擾
	void SetVtxCol(D3DXCOLOR *col);		// ���_�J���[�ݒ�
	D3DXVECTOR2 *GetVtxTex(void);		// ���_�e�N�X�`���擾
	void SetVtxTex(D3DXVECTOR2 *tex);	// ���_�e�N�X�`���ݒ�
	int GetWidthBlock(void);			// ��������
	void SetWidthBlock(int nWidth);		// ��������
	int GetHeightBlock(void);			// �c������
	void SetHeightBlock(int nHeight);	// �c������
	float GetWidthLen(void);			// ���̒���
	void SetWidthLen(float fLen);		// ���̒���
	float GetHeightLen(void);			// �c�̒���
	void SetHeightLen(float fLen);		// �c�̒���
	int GetIdxTex(void);				// �e�N�X�`���̃C���f�b�N�X�ԍ��擾

	static CObject3DMesh *Create();
	static CObject3DMesh *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = mylib_const::DEFAULT_PRIORITY);
	static CObject3DMesh *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidthLen, float fHeightLen, int nWidth, int nHeight, TYPE type, const char *pFileName, int nPriority = mylib_const::DEFAULT_PRIORITY);
	CObject3DMesh *GetObject3DMesh(void);
protected:

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
private:
	HRESULT CreateVertex(void);
	HRESULT CreateIndex(void);
	void SetInitVtxField(void);
	void SetVtxField(void);
	void SetVtxWall(void);
	void SetVtxCylinder(void);
	void SetVtxDome(void);
	void SetVtxDonuts(void);
	void SetVtxSphere(void);
	void UPVtxField(D3DXVECTOR3 pos);
	void CalWorldMtx(void);	// ���[���h�}�g���b�N�X�̌v�Z����

	D3DXVECTOR3 *m_pVtxPos;	// ���_���W
	D3DXVECTOR3 *m_pVtxNor;	// ���_�@��
	D3DXCOLOR *m_pVtxCol;	// ���_�J���[
	D3DXVECTOR2 *m_pVtxTex;	// ���_�e�N�X�`�����W
	int m_nNumIndex;		// �C���f�b�N�X��
	int m_nNumVertex;		// ���_��
	int m_nWidth;			// ��������
	int m_nHeight;			// �c������
	float m_fWidthLen;		// ���̒���
	float m_fHeightLen;		// �c�̒���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int m_nTexIdx;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	TYPE m_type;						// ���b�V���̃^�C�v
};



#endif