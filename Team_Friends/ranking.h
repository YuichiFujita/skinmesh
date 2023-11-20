//=============================================================================
// 
//  �����L���O�w�b�_�[ [ranking.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CRankingScore;

//==========================================================================
// �N���X��`
//==========================================================================
// �����L���O�N���X��`
class CRanking : public CScene
{
public:

	CRanking();
	~CRanking();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetEnableArrival(void);
	static int GetRandStage(void) { return m_nRandStage; }
private:
	int m_nCntSwitch;		// �؂�ւ��̃J�E���^�[
	static bool m_bAllArrival;		// �S�ē�����������
	static int m_nRandStage;	// �X�e�[�W�̃����_���C���f�b�N�X�ԍ�
	static CRankingScore *m_pRankingScore;	// �����L���O�X�R�A�̃I�u�W�F�N�g
};



#endif