//=============================================================================
// 
//  ���U���g�w�b�_�[ [result.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CResultScore;

//==========================================================================
// �N���X��`
//==========================================================================
// ���U���g�N���X��`
class CResult : public CScene
{
public:

	CResult();
	~CResult();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultScore *GetResultScore(void);
	static void SetEnableArrival(void);

private:
	static bool m_bAllArrival;		// �S�ē�����������
	static CResultScore *m_pResultScore;
};



#endif