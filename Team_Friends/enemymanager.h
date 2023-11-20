//=============================================================================
// 
//  �G�̃}�l�[�W���w�b�_�[ [enemymanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

// �O���錾
class CEnemy;

//==========================================================================
// �N���X��`
//==========================================================================
// �G�̃}�l�[�W���N���X��`
class CEnemyManager
{
public:

	// �\���̒�`
	struct EnemyData
	{
		int nType;	// �L�����N�^�[���
		int nStartFrame;	// �����t���[��
		D3DXVECTOR3 pos;		// �ʒu
	};

	struct Pattern
	{
		int nNumEnemy;	// �G�̐�
		int nFixedType;	// ���̓����̎��
		EnemyData EnemyData[mylib_const::MAX_PATTEN_ENEMY];
	};

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,		// �����Ȃ����
		STATE_COMBOANIM,	// �R���{���̃A�j���[�V����
		STATE_MAX
	};

	CEnemyManager();
	~CEnemyManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Kill(void);		// �j��

	static CEnemyManager *Create(const std::string pTextFile);
	HRESULT ReadText(const std::string pTextFile);	// �O���t�@�C���ǂݍ��ݏ���
	void SetStageEnemy(void);	// �X�e�[�W���̓G�z�u
	CEnemy **SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPattern);	// �G�z�u
	int GetPatternNum(void);
	void Release(int nIdx);		// �j��


	bool IsChangeStage(void) { return m_bChangeStage; }	// �X�e�[�W�ύX����
	void SetEnableChangeStage(bool bChange) { m_bChangeStage = bChange; }	// �X�e�[�W�ύX�̏�Ԑ؂�ւ�
	STATE GetState(void) { return m_state; }	// ��Ԏ擾
	int GetNumAll(void);		// �G�̑����擾
	Pattern GetPattern(int nPattern);	// �p�^�[���擾
	CEnemy **GetEnemy(void);	// �G�擾
	const char *GetMotionFilename(int nType);
protected:


private:

	CEnemy *m_pEnemy[mylib_const::MAX_OBJ];						// �G�ւ̃|�C���^
	Pattern m_aPattern[mylib_const::MAX_PATTEN_ENEMY];			// �z�u�̎��
	std::string sMotionFileName[mylib_const::MAX_PATTEN_ENEMY];	// ���[�V�����t�@�C����
	STATE m_state;			// ���
	int m_nCntSpawn;		// �o���J�E���g
	int m_nPatternNum;		// �o���p�^�[����
	int m_nNumChara;		// �G�̎�ނ̑���
	int m_nNumAll;			// �G�̑���
	bool m_bChangeStage;	// �X�e�[�W�ύX����
};



#endif