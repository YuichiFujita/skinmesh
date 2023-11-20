//=============================================================================
// 
//  ����(2D)�w�b�_�[ [number_2D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _NUMBER_2D_H_
#define _NUMBER_2D_H_	// ��d�C���N���[�h�h�~

#include "number.h"
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D;

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CNumber2D : public CNumber
{
public:

	CNumber2D(int nPriority = 7);
	~CNumber2D();

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
	void SetRotation(const D3DXVECTOR3 rot) override;	// �����ݒ�
	D3DXVECTOR3 GetRotation(void) const override;		// �����擾

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
	CObject2D *GetObject2D(void) override;

private:
	CObject2D *m_aObject2D;				// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
};

#endif