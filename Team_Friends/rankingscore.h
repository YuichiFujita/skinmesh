//=============================================================================
// 
//  �����L���O�X�R�A�w�b�_�[ [rankingscore.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RANKINGSCORE_H_
#define _RANKINGSCORE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define RANKINGSCORE_DIGIT	(6)	// ����
#define RANKINGNUM	(5)	// ����
#define RANKINGNUM_PLAYER	(5) // �v���C���[�̐�

//==========================================================================
// �O���錾
//==========================================================================
class CNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �����L���O�X�R�A�N���X��`
class CRankingScore : public CObject
{
public:

	CRankingScore(int nPriority = 7);
	~CRankingScore();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//void SetAllArrival(void);	// �S�Ă̓����ݒ�
	void UpdateNewRecord(void);	// �j���[���R�[�h�̍X�V����
	void SetValue(int nCntRanking, int nCntPlayer);	// �l�̐ݒ菈��
	void Moving(int nCntRanking, int nCntPlayer);	// �ړ�����
	static CRankingScore *Create(void);
private:

	// �񋓌^��`
	enum VTX
	{
		VTX_NUM = 0,		// ���U���g���S
		VTX_LOGO,
		VTX_MAX
	};


	enum MOVETYPE
	{
		TYPE_NONE = 0,		//
		TYPE_RIGHT,
		TYPE_LEFT,
		TYPE_MAX
	};

	void Load(void);	// ���[�h
	void Save(void);	// �Z�[�u
	void Sort(void);	// �\�[�g
	
	int m_nNumRanking;				// �����L���O��
	int m_nScore[RANKINGNUM_PLAYER][RANKINGNUM];		// �X�R�A
	int m_nNowScore[RANKINGNUM_PLAYER];		// ����̃X�R�A
	int m_nTexIdx[VTX_MAX];					// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nTexIdxNumber;			// �����e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxNewRecord[RANKINGNUM_PLAYER];			// �j���[���R�[�h�̃C���f�b�N�X�ԍ�
	int m_nCntNewRecord;			// �j���[���R�[�h�̃J�E���^�[
	float m_fPosDestX[RANKINGNUM_PLAYER][RANKINGNUM];	// �ڕW�̈ʒu
	bool m_bNewRecord[RANKINGNUM_PLAYER];				// �j���[���R�[�h�̃t���O
	bool m_bArrival[RANKINGNUM_PLAYER][RANKINGNUM];	// ��������
	bool m_bMove;					// �����L���O�������Ă��邩�ǂ���
	bool m_bEnd;					// �����L���O���[���ǂ���
	CObject2D *m_pObj2D[VTX_MAX];	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	CNumber *m_pScore[RANKINGNUM_PLAYER][RANKINGNUM][RANKINGSCORE_DIGIT];			// �����̃I�u�W�F�N�g
	static const char *m_apTextureFile[VTX_MAX];	// �e�N�X�`���̃t�@�C��
	D3DXVECTOR3 PrePos[RANKINGNUM_PLAYER];			// �����̃I�u�W�F�N�g�̈ʒu�ێ�
	MOVETYPE m_nType;				// �����L���O���ǂ���ɓ����Ă��邩
};



#endif