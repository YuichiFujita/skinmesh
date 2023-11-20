//=============================================================================
// 
//  �r���{�[�h�w�b�_�[ [objectBillboard.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "constans.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �r���{�[�h�N���X��`
class CObjectBillboard : public CObject
{
public:

	CObjectBillboard(int nPriority = 7);
	~CObjectBillboard();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetVtx(void);
	void BindTexture(int nIdx);

	virtual void SetWorldMtx(const D3DXMATRIX mtx);		// �}�g���b�N�X�ݒ�
	virtual D3DXMATRIX GetWorldMtx(void) const;			// �}�g���b�N�X�擾
	virtual void SetColor(const D3DXCOLOR col);			// �F�ݒ�
	virtual D3DXCOLOR GetColor(void) const;				// �F�擾
	virtual void SetSize(const D3DXVECTOR2 size);		// �T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSize(void) const;			// �T�C�Y�̎擾
	virtual void SetSizeOrigin(const D3DXVECTOR2 size);	// ���̃T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSizeOrigin(void) const;		// ���̃T�C�Y�̎擾
	virtual void SetTex(D3DXVECTOR2 *tex);				// �e�N�X�`�����W�̐ݒ�
	virtual D3DXVECTOR2 *GetTex(void);					// �e�N�X�`�����W�̎擾

	static CObjectBillboard *Create();
	static CObjectBillboard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	CObjectBillboard *GetObjectBillBoard(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const;
protected:

private:
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXCOLOR m_col;		// �F
	D3DXVECTOR2 m_fSize;	// �T�C�Y
	D3DXVECTOR2 m_sizeOrigin;	// ���̃T�C�Y
	D3DXVECTOR2 m_fTex[32];	// �e�N�X�`�����W
	int m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
};



#endif