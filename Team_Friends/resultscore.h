//=============================================================================
// 
//  ���U���g�X�R�A�w�b�_�[ [resultscore.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define RESULTSCORE_DIGIT	(7)	// ����

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CResultScore : public CObject
{
public:

	CResultScore(int nPriority = 7);
	~CResultScore();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAllArrival(void);	// �S�Ă̓����ݒ�
	void SetValue(int nScore);	// �l�̐ݒ菈��
	static CResultScore *Create(void);
	bool IsSetToatal(void) { return m_bSetToatal; }
private:
	
	void CreateToatalScore(void);	// �ŏI�X�R�A�̐���
	void SetToatalValue(void);		// �ŏI�X�R�A�̒l�ݒ菈��
	void MoveToatalScore(void);		// �ŏI�X�R�A�̈ړ�����

	int m_nToatalNum;				// �l
	int m_nToatalNumDest;			// �ڕW�̒l
	int m_nTexIdx;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nTexIdxNumber;			// �����e�N�X�`���̃C���f�b�N�X�ԍ�
	float m_fToatalPosDest_X;		// �ŏI�X�R�A�̖ڕW�ʒu
	bool m_bArrivalToatal;			// �ŏI�X�R�A�̓�������
	bool m_bSetToatal;				// �ŏI�X�R�A�̐ݒ蔻��
	CObject2D *m_pToatalObj2D;		// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	CMultiNumber *m_pToatalScore;	// �����̃I�u�W�F�N�g
};



#endif