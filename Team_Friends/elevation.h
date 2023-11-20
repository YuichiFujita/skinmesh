//=============================================================================
// 
//  �N���w�b�_�[ [elevation.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ELEVATION_H_
#define _ELEVATION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

// �O���錾
class CTargetPoint;

//==========================================================================
// �N���X��`
//==========================================================================
// �N���N���X��`
class CElevation : public CObject3DMesh
{
private:

	struct Info
	{
		std::string TextureFileName;	// �e�N�X�`���t�@�C����
		D3DXVECTOR3 pos;		// �ʒu
		float fWidthLength;		// ������
		float fHeightLength;	// �c����
		int nWidthBlock;		// ��������
		int nHeightBlock;		// �c������
		D3DXVECTOR3 *pVtxPos;	// ���_���W
	};

public:

	CElevation(int nPriority = 1);
	~CElevation();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual HRESULT Init(const char *pText);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void SetVtx(void);

	void Release(void);
	bool IsHit(D3DXVECTOR3& pos);	// �����������̔���
	bool IsEdit(void);	// �G�f�B�b�g�̔���
	void Save(void);	// �Z�[�u
	HRESULT Load(const char *pText);	// ���[�h

	static CElevation *Create(const char *pText);
	CElevation *GetElevation(void);
	float GetHeight(D3DXVECTOR3& pos, bool &bLand);	// �����擾

protected:

private:

	void UpdateState(void);				// ��ԍX�V����
	void UPVtxField(D3DXVECTOR3 pos);	// ���_�グ����(�f�o�b�O)

	Info m_aInfo;	// �N���̏��
	float m_fBrushStrength;							// �u���V����
	float m_fBrushRange;							// �͈�
	bool m_bEdit;									// �G�f�B�b�gON/OFF
	CTargetPoint *m_pTargetP;						// �ڕW�̒n�_
	D3DXVECTOR3 m_VtxPos[mylib_const::MAX_VTX];		// ���_���W
	int m_nTexIdx;									// �e�N�X�`���̃C���f�b�N�X�ԍ�
	static bool m_bLoadInfo;						// ���ǂݍ��ݔ���
};



#endif