//=============================================================================
// 
//  ����(�r���{�[�h)�w�b�_�[ [number_Billboard.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _NUMBER_BILLBOARD_H_
#define _NUMBER_BILLBOARD_H_	// ��d�C���N���[�h�h�~

#include "number.h"
#include "objectBillboard.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectBillboard;

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CNumberBillboard : public CNumber
{
public:

	CNumberBillboard(int nPriority = 6);
	~CNumberBillboard();

	// �����o�֐�
	HRESULT Init(int nPriority) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;	// �J������

	void SetPosition(const D3DXVECTOR3 pos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetPosition(void) const override;		// �ʒu�擾
	void SetMove(const D3DXVECTOR3 move) override;		// �ړ��ʐݒ�
	D3DXVECTOR3 GetMove(void) const override;			// �ړ��ʎ擾

	void SetColor(const D3DXCOLOR col) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾
	void SetSize(const D3DXVECTOR2 size) override;		// �T�C�Y�̐ݒ�
	D3DXVECTOR2 GetSize(void) const override;			// �T�C�Y�̎擾
	void SetSizeOrigin(const D3DXVECTOR2 size) override;	// ���̃T�C�Y�̐ݒ�
	D3DXVECTOR2 GetSizeOrigin(void) const override;		// ���̃T�C�Y�̎擾
	void SetTex(D3DXVECTOR2 *tex) override;				// �e�N�X�`�����W�̐ݒ�
	D3DXVECTOR2 *GetTex(void) override;					// �e�N�X�`�����W�̎擾

	void SetVtx(void) override;
	void BindTexture(int nIdx) override;
	void SetType(const CObject::TYPE type) override;
	CObjectBillboard *GetObjectBillboard(void) override;

private:
	CObjectBillboard *m_pObjBillboard;	// �I�u�W�F�N�g�r���{�[�h�̃I�u�W�F�N�g
};

#endif