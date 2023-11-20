//==========================================================================
//
// ���X�g�\���w�b�_�[[list.h]
// Author�F���n�Ή�
//
//==========================================================================
#ifndef _LIST_H_
#define _LIST_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// ���X�g�\���N���X�̒�`
//==========================================================================
class CList : public CObject
{
public:

	CList();	// �R���X�g���N�^
	~CList();	// �f�X�g���N�^

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Release(void);	// �J������

	void SetNext(CList* pNext);	// ���̃I�u�W�F�N�g�ݒ�
	CList *GetNext(void);		// ���̃I�u�W�F�N�g�擾
	void SetPrev(CList* pPrev);	// �O�̃I�u�W�F�N�g�ݒ�
	CList *GetPrev(void);		// �O�̃I�u�W�F�N�g�擾
	bool IsDeath(void);			// ���S����擾

	static CList *Create(void);	// ��������
private:

	CList *m_pPrev;		// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CList *m_pNext;		// ���̃I�u�W�F�N�g�ւ̃|�C���^
	bool m_bDeath;		// ���S�t���O
};




#endif