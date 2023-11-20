//=============================================================================
// 
//  ���[�V�����w�b�_�[ [motion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

class CModel;
class CObjectChara;

#define MAX_MOTION	(16)	//���[�V�����̍ő吔
#define MAX_KEY		(24)	//�L�[�̍ő吔
#define MAX_PARTS	(64)	//�p�[�c�̍ő吔

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�V�����N���X��`
class CMotion
{
public:

	// ����̍\����
	typedef struct
	{
		int nCollisionNum;		// �����蔻��̃p�[�c�ԍ�
		float fRangeSize;		// ����̃T�C�Y
		D3DXVECTOR3 Offset;		// ����̃I�t�Z�b�g
		int nMinCnt;			// ����̍Œ�J�E���g
		int nMaxCnt;			// ����̍ő�J�E���g
		int nDamage;			// �_���[�W
		int nInpactCnt;			// �Ռ��̃J�E���g
	}AttackInfo;

	// ���[�V�����̍\����
	typedef struct
	{
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 rotDest;	// �ڕW�̌���
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 posDest;	// �ڕW�̈ʒu
		D3DXVECTOR3 posOrigin;	// �ʒu�̌��_
	}Parts;

	typedef struct
	{
		Parts aParts[MAX_PARTS];
		int nFrame;				// �Đ��t���[��
		float fRotMove;			// �ړ��̌���
	}Key;

	typedef struct
	{
		Key aKey[MAX_KEY];
		int nNumKey;			// �L�[�̐�
		int nLoop;				// ���[�v����
		int nMove;				// �ړ�����
		int nNumAttackInfo;		// �U�����̐�
		AttackInfo *AttackInfo[MAX_PARTS];	// �����蔻��p
	}Info;

	CMotion();
	~CMotion();

	HRESULT Init(void);
	void Uninit(void);
	void Update(float fBuff = 1.0f);
	static CMotion *Create(void);
	static CMotion *Create(const std::string pTextFile);
	void Set(int nType, bool bBlend = true);	// ���[�V�����̐ݒ菈��
	int GetType(void);		// ���݂̃��[�V�����^�C�v�擾
	int GetOldType(void);		// �O��̃��[�V�����^�C�v�擾
	bool IsFinish(void);	// �I�����Ă��邩�̔���
	void ToggleFinish(bool bFinish);	// �I�����Ă��邩�̔���؂�ւ�
	void SetSlowFactor(float fFactor);	// �x���W���̐ݒ�

	D3DXVECTOR3 GetAttackPosition(CModel **ppModel, AttackInfo attackInfo);	// �U���̈ʒu�擾
	float GetAllCount(void);	// �S�ẴJ�E���g�擾
	float GetMaxAllCount(void) { return m_fMaxAllFrame; }
	int GetMaxAllCount(int nType);	// �S�ẴJ�E���g�擾
	void SetFrameCount(float fCnt);					// �t���[���J�E���g�ݒ�
	float GetFrameCount(void) { return m_fCntFrame; }	// �t���[���J�E���g�擾
	int GetNumAll(void) { return m_nNumModel; }		// �p�[�c���擾
	int GetNumMotion(void) { return m_nNumMotion; }	// ���[�V�����̑����擾
	void SetNowPattern(int nPattern) { m_nPatternKey = nPattern; }		// ���݂̃p�^�[���ݒ�
	int GetNowPattern(void) { return m_nPatternKey; }	// ���݂̃p�^�[���擾
	void AddNumAttackInfo(int nType);	// �U�����̑������Z
	void SubNumAttackInfo(int nType);	// �U�����̑������Z

	void ReadText(const std::string pTextFile);
	void SetInfo(Info info);	// ���[�V�������̓o�^
	void SetAttackInfo(AttackInfo info);	// �U�����̓o�^
	Info GetInfo(int nType);	// ���[�V�������̎擾
	void SetInfoSave(int nType, int nKey, int nParts, D3DXVECTOR3 rot);	// ���[�V�������̓o�^
	void ChangeKeyNum(int nType, int nNum);	// �L�[�����ύX
	void ChangeKeyFrame(int nType, int nKey, int nNum);	// �L�[�t���[���ύX
	void ChangeAttackInfo(int nType, int nIdx, AttackInfo info);	// �U�����ύX
	int IsGetMove(int nType);	// �ړ��̔���擾
	void SetModel(CModel **pModel, int nNumModel, CObjectChara *pObjChara);	// ���[�V���������郂�f���̓o�^
	void ResetPose(int nType);	// �|�[�Y�̃��Z�b�g
private:
	// �񋓌^��`
	typedef enum
	{
		LOOP_OFF = 0,	// ���[�v����
		LOOP_ON,		// ���[�v����
		LOOP_MAX
	}LOOP;

	Info m_aInfo[MAX_MOTION];	// ���[�V�����̏��
	Parts aPartsOld[MAX_PARTS];	// �ߋ��̏��
	int m_nNumAll;				// ���[�V�����̑���
	int m_nType;				// ���݂̃��[�V�����̎��
	int m_nOldType;				// �O��̃��[�V�����̎��
	bool m_bLoop;				// ���[�v���邩�ǂ���
	int m_nPatternKey;			// ���ڂ̃L�[��
	float m_fCntFrame;			// �t���[���̃J�E���g
	float m_fCntAllFrame;		// �S�ẴJ�E���g
	float m_fMaxAllFrame;		// �S�ẴJ�E���g�̍ő�l
	float m_fSlowFactor;		// �x���W��
	bool m_bFinish;				// �I���������ǂ���

	CObjectChara *m_pObjChara;	// �I�u�W�F�N�g�L�����N�^�[�̃|�C���^
	CModel **m_ppModel;			// ���f���̃|�C���^
	int m_nNumModel;			// �p�[�c�̑���
	int m_nNumMotion;			// ���[�V�����̑���
	static std::vector<std::string> m_sTextFile;	// �e�L�X�g�t�@�C����
	static int m_nNumLoad;	// �ǂݍ��񂾐�
	static std::vector<int> m_nNumLoadData;				// ���[�V�������̃f�[�^��
	static std::vector<Info> m_aLoadData[MAX_MOTION];	// ���[�V�����̓ǂݍ��ݏ��
	static std::vector<AttackInfo> m_aLoadAttackData[MAX_MOTION][MAX_PARTS];	// �U���̓ǂݍ��ݏ��
};

#endif