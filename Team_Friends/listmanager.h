//==========================================================================
//
// ���X�g�}�l�[�W���w�b�_�[[listmanager.h]
// Author�F���n�Ή�
//
//==========================================================================
#ifndef _LISTMANAGER_H_
#define _LISTMANAGER_H_	// ��d�C���N���[�h�h�~

#include "list.h"
#include "object.h"

//==========================================================================
// ���X�g�\���N���X�̒�`
//==========================================================================
class CListManager
{
public:

	CListManager();			// �R���X�g���N�^
	~CListManager();		// �f�X�g���N�^

	void ReleaseAll(void);	// �S�Ă̊J������
	void Push(CList *pObj);			// �I�u�W�F�N�g�ǉ�
	void Pop(CList *pObj);			// �I�u�W�F�N�g�����o��
	CList *GetTop(void);			// �擪�̃I�u�W�F�N�g�擾
	static CListManager *Create(void);	// ��������
private:

	CList *m_pTop;	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	CList *m_pCur;	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	int m_nNumAll;	// ����
};




#endif