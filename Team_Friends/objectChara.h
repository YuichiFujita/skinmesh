//=============================================================================
// 
//  �I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

class CModel;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�N���X��`
class CObjectChara : public CObject
{
public:

	// �\���̒�`
	struct LoadData
	{
		std::string pModelFile;	// ���f���t�@�C����
		int nType;				// ���f�����
		int nParent;			// �e�̃C���f�b�N�X
		int nStart;				// �ŏ����炠�邩�ǂ���
		int nSwitchType;		// �؂�ւ��̎��
		int nIDSwitchModel;		// �؂�ւ����f����ID
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
	};

	struct Load
	{
		std::string sTextFile;	// �e�L�X�g�t�@�C����
		int nNumModel;			// ���f����
		float fVelocity;		// �ړ����x
		float fRadius;			// ���a
		int nLife;				// �̗�
		int nMotionStartIdx;	// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
		int nAddScore;			// �X�R�A���Z��
		D3DXVECTOR3 posOrigin;	// �ŏ��̈ʒu
		LoadData LoadData[mylib_const::MAX_MODEL];
	};

	// �񋓌^��`
	typedef enum
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_MAX
	}STATE;

	CObjectChara(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CObjectChara();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);

	void SetmtxWorld(const D3DXMATRIX mtxWorld);
	D3DXMATRIX GetmtxWorld(void) const;			// ���[���h�}�g���b�N�X�擾
	D3DXVECTOR3 GetCenterPosition(void) const;		// ���S�̈ʒu�擾
	void SetOriginPosition(const D3DXVECTOR3 pos);	// �ŏ��̈ʒu�ݒ�
	D3DXVECTOR3 GetOriginPosition(void) const;		// �ŏ��̈ʒu�擾
	void SetVelocity(const float fVelocity);	// �ړ����x�ݒ�
	float GetVelocity(void) const;				// �ړ����x�擾
	void SetRadius(const float fRadius);		// ���a�ݒ�
	float GetRadius(void) const;				// ���a�擾
	void SetRotDest(const float fRotDest);		// �ڕW�̌����ݒ�
	float GetRotDest(void) const;				// �ڕW�̌����擾
	void SetLife(const int nLife);				// �̗͐ݒ�
	int GetLife(void) const;					// �̗͎擾
	int GetLifeOrigin(void) const;				// ���̗͎̑擾
	int GetMotionStartIdx(void) const;			// ���[�V�����J�n�̃C���f�b�N�X�ԍ��擾
	int GetAddScoreValue(void) const;			// �X�R�A���Z�ʎ擾
	void SetEnableWaveHit(void);				// �g�̓����蔻��ON
	void SetDisableWaveHit(void);				// �g�̓����蔻��OFF
	bool IsWaveHit(void);						// �g�̔����Ԏ擾

	virtual int GetState(void);					// ��Ԏ擾
	virtual void SetState(int state);			// ��Ԑݒ�
	virtual void SetState(int state, int nCntState);	// ��Ԑݒ�
	virtual int GetCharaType(void);	// ��ގ擾

	HRESULT ReadText(const std::string pTextFile);	// �O���t�@�C���ǂݍ��ݏ���
	HRESULT SetCharacter(const std::string pTextFile);

	void ChangeObject(int nDeleteParts, int nNewParts);
	void SetObject(int nNewParts);

	CObjectChara *GetObjectChara(void);
	int GetNumModel(void);		// ���f�����擾
	void SetNumModel(int nNumModel);		// ���f�����ݒ�
	int GetIdxFile(void);		// �t�@�C���̃C���f�b�N�X�ԍ��擾
	Load GetLoadData(int nIdx);	// �ǂݍ��ݏ��擾
	CModel **GetModel(void);
	static CObjectChara *Create(const std::string pTextFile);

private:

	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_posOrigin;	// �ŏ��̈ʒu
	float m_fVelocity;			// �ړ����x
	float m_fRadius;			// ���a
	float m_fRotDest;			// �ڕW�̌���
	int m_nLife;				// �̗�
	int m_nLifeOrigin;			// ���̗̑�
	int m_nMotionStartIdx;		// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
	int m_nAddScore;			// �X�R�A���Z��
	int m_nNumModel;			// ���f���̐�
	int m_nIdxFile;				// �t�@�C���̃C���f�b�N�X�ԍ�
	bool m_bWaveHit;			// �g�ɓ������Ă��邩�̔���
	CModel *m_apModel[mylib_const::MAX_MODEL];	// ���f��(�p�[�c)�̃|�C���^

	static int m_nNumLoad;	// �ǂݍ��񂾐�
	static Load m_aLoadData[mylib_const::MAX_MODEL];
};



#endif