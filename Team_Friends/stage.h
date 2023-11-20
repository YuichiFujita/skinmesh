//=============================================================================
// 
//  �X�e�[�W�w�b�_�[ [stage.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STAGE_H_
#define _STAGE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

// �O���錾
class CInjectionTable;

//==========================================================================
// �N���X��`
//==========================================================================
// �X�e�[�W�N���X��`
class CStage
{
public:

	CStage();
	~CStage();

	HRESULT Init(void);
	void Uninit(void);
	void Release(void);
	void Update(void);
	void Draw(void);

	CObjectX *GetObj(int nIdx);
	CInjectionTable *GetInjectionTable(void);	// �ˏo��擾
	int GetNumAll(void);		// �����擾
	static CStage *Create(const char *pTextFile);

private:

	// �����o�֐�
	HRESULT ReadXFile(const char *pTextFile);	// ���ł�ǂݍ��ݏ���
	HRESULT ReadText(const char *pTextFile);		// �O���t�@�C���ǂݍ��ݏ���
	HRESULT SaveText(void);		// �O���t�@�C���������ݏ���

	// �����o�ϐ�
	int m_nNumAll;
	std::vector<std::string> ModelFile;		// ���f���t�@�C����
	CObjectX *m_pObjX[mylib_const::MAX_STAGE];	// �I�u�W�F�N�gX�̃|�C���^
	CInjectionTable *m_pInjectionTable;			// �ˏo��̃I�u�W�F�N�g

};


#endif