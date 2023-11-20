//=============================================================================
// 
//  �����L���O�}�l�[�W���w�b�_�[ [rankingmanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RANKINGMANAGER_H_
#define _RANKINGMANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define RANKINGNUM_PLAYER	(5) // �v���C���[�̐�

// �O���錾

//==========================================================================
// �N���X��`
//==========================================================================
// �����L���O�}�l�[�W���N���X��`
class CRankingManager
{
public:

	CRankingManager();
	~CRankingManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void Reset(void);				// ���Z�b�g
	void SetNowScore(int nScore, int nPlayer);	// ����̃X�R�A�ݒ�
	int GetNowScore(int nPlayer);			// ����̃X�R�A�擾
	static CRankingManager *Create(void);	// ��������
	
private:
	int m_nNowScore[RANKINGNUM_PLAYER];	// ����̃X�R�A
};



#endif