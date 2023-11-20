//=============================================================================
// 
//  �I�u�W�F�N�g3D�w�b�_�[ [object3D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g3D�N���X��`
class CObject3D : public CObject
{
public:

	CObject3D(int nPriority = 5);
	virtual ~CObject3D();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void SetVtx(void);
	virtual void BindTexture(int nIdx);

	void SetWorldMtx(const D3DXMATRIX mtx);		// �}�g���b�N�X�ݒ�
	D3DXMATRIX GetWorldMtx(void) const;			// �}�g���b�N�X�擾
	void SetOriginPosition(const D3DXVECTOR3 pos);	//	���̈ʒu�ݒ�
	D3DXVECTOR3 GetOriginPosition(void) const;		//	���̈ʒu�擾
	void SetOriginRotation(const D3DXVECTOR3 rot);	// ���̌����ݒ�
	D3DXVECTOR3 GetOriginRotation(void) const;		// ���̌����擾
	void SetColor(const D3DXCOLOR col);			// �F�ݒ�
	D3DXCOLOR GetColor(void) const;				// �F�擾
	void SetSize(const D3DXVECTOR3 size);		// �T�C�Y�̐ݒ�
	D3DXVECTOR3 GetSize(void) const;			// �T�C�Y�̎擾

	static CObject3D *Create();
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	CObject3D *GetObject3D(void);
	virtual CObject3DMesh *GetObject3DMesh(void);
protected:

private:
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_posOrigin;	// ���̈ʒu
	D3DXVECTOR3 m_rotOrigin;	// ���̌���
	D3DXCOLOR m_col;			// �F
	D3DXVECTOR3 m_fSize;		// �T�C�Y
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	int m_nTexIdx;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif