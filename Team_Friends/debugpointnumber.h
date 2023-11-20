//=============================================================================
// 
//  �|�C���g�̐����w�b�_�[ [debugpointnumber.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DEBUG_POINTNUMBER_H_
#define _DEBUG_POINTNUMBER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �G�f�B�b�g�N���X��`
class CDebugPointNumber : public CObject
{
public:

	CDebugPointNumber(int nPriority = 7);
	~CDebugPointNumber();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CDebugPointNumber *Create(int nIdx);

private:
	int m_nIdxParent;	// �e�̔ԍ�
	CMultiNumber *m_pMultiNumber;	// �����̃I�u�W�F�N�g
};



#endif