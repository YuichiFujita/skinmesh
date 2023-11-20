//=============================================================================
// 
//  ���f���w�b�_�[ [model.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// ���f���N���X��`
class CModel
{
public:

	CModel(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CModel();

	HRESULT Init(const char *pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);
	void BindXData(int nIdxXFile);

	void SetWorldMtx(const D3DXMATRIX mtx);		// �}�g���b�N�X�ݒ�
	D3DXMATRIX GetWorldMtx(void) const;			// �}�g���b�N�X�擾
	void SetPosition(const D3DXVECTOR3 pos);	// �ʒu�ݒ�
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	void SetOldPosition(const D3DXVECTOR3 posOld);	// �O��̈ʒu�ݒ�
	D3DXVECTOR3 GetOldPosition(void) const;		// �O��̈ʒu�擾
	void SetRotation(const D3DXVECTOR3 rot);	// �����ݒ�
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	void SetOriginRotation(const D3DXVECTOR3 rot);	// ���̌����ݒ�
	D3DXVECTOR3 GetOriginRotation(void) const;		// ���̌����擾
	D3DXVECTOR3 GetVtxMax(void) const;			// ���_�̍ő�l�擾
	D3DXVECTOR3 GetVtxMin(void) const;			// ���_�̍ŏ��l�擾
	void SetParent(CModel *pModel);				// �e�̃|�C���^�ݒ�

	static CModel *Create(const char *pFileName, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CModel *GetModel(void);
	static int GetNumAll(void);
protected:

private:

	void CalWorldMtx(void);	// ���[���h�}�g���b�N�X�̌v�Z����

	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_rotOrigin;// ����
	int m_nIdxXFile;		// X�t�@�C���̃C���f�b�N�X�ԍ�
	CModel *m_pParent;		// �e���f���̃|�C���^
	static int m_nNumAll;	// ����
};



#endif