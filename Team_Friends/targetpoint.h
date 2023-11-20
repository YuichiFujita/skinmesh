//=============================================================================
// 
//  �ڕW�n�_�w�b�_�[ [targetpoint.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TARGETPOINT_H_
#define _TARGETPOINT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ڕW�n�_�N���X��`
class CTargetPoint : public CObject3DMesh
{
public:
	CTargetPoint(int nPriority = 6);
	~CTargetPoint();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	void Control(void);	// ����
	static CTargetPoint *Create(D3DXVECTOR3 pos, float fWidthLen, float fHeightLen);
	CTargetPoint *GetMyObject(void);

private:

	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nAlphaCnt;	// �s�����x�̃J�E���g
};



#endif