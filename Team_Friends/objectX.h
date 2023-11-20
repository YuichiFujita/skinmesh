//=============================================================================
// 
//  �I�u�W�F�N�gX�w�b�_�[ [objectX.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "Xload.h"

// �O���錾
class CShadow;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�gX�N���X��`
class CObjectX : public CObject
{
public:

	// �}�N����`
#define MAX_MAT				(512)		// �}�e���A���T�C�Y
#define MAX_TX				(96)		// �e�N�X�`���T�C�Y

	CObjectX(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CObjectX();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(const char *pFileName);
	HRESULT Init(int nIdxXFile);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);
	void BindTexture(int *nIdx);
	void BindXData(int nIdxXFile);

	void SetWorldMtx(const D3DXMATRIX mtx);		// �}�g���b�N�X�ݒ�
	D3DXMATRIX GetWorldMtx(void) const;			// �}�g���b�N�X�擾
	void SetScale(const D3DXVECTOR3 scale);		// �X�P�[���ݒ�
	D3DXVECTOR3 GetScale(void) const;			// �X�P�[���擾
	void SetColor(const D3DXCOLOR col);			// �F�ݒ�
	D3DXCOLOR GetColor(void) const;				// �F�擾
	void SetSize(const D3DXVECTOR3 size);		// �T�C�Y�̐ݒ�
	D3DXVECTOR3 GetSize(void) const;			// �T�C�Y�̎擾
	D3DXVECTOR3 GetVtxMax(void) const;			// ���_�̍ő�l�擾
	D3DXVECTOR3 GetVtxMin(void) const;			// ���_�̍ŏ��l�擾
	int GetIdxXFile(void) const;				// X�t�@�C���̃C���f�b�N�X�擾
	bool GetUseShadow(void) const;				// �e���g���Ă��邩�ǂ���

	float GetHeight(D3DXVECTOR3 pos, bool &bLand);	// �����擾

	void Kill(void);
	static CObjectX *Create();
	static CObjectX *Create(const char *pFileName);
	static CObjectX *Create(const char *pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), bool bShadow = false);
	static CObjectX *Create(int nIdxXFile, D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), bool bShadow = false);
	CObjectX *GetObjectX(void);
	static int GetNumAll(void);
protected:

private:
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_scale;	// �X�P�[��
	D3DXCOLOR m_col;		// �F
	D3DXVECTOR3 m_fSize;	// �T�C�Y
	bool m_bShadow;			// �e���g�����ǂ���
	int m_nIdxTexure;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxXFile;		// X�t�@�C���̃C���f�b�N�X�ԍ�
	static int m_nNumAll;	// ����
	CShadow *m_pShadow;		// �e�̏��
};



#endif