//=============================================================================
// 
//  �G�f�B�b�g�w�b�_�[ [edit.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_H_
#define _EDIT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// �G�f�B�b�g�N���X��`
class CEdit : public CObject
{
public:

	CEdit(int nPriority = 0);
	~CEdit();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(const char *pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEdit *Create();
	void Release(void);
	static int GetNumAll(void);
protected:

private:
	HRESULT ReadText(void);
	void Control(CObjectX *pObjX);
	void ChangeType(void);
	void GrabModel(void);
	void DeleteModel(void);

	static CObjectX *m_pObjX;
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	static int m_nNumAll;	// ����
	static int m_nType;		// �^�C�v
	static bool m_bShadow;	// �e���g�����ǂ���
};



#endif