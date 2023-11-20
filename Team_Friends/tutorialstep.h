//=============================================================================
// 
//  �`���[�g���A���X�e�b�v�w�b�_�[ [tutorialstep.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TUTORIALSTEP_H_
#define _TUTORIALSTEP_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �`���[�g���A���X�e�b�v�N���X��`
class CTutorialStep
{
public:
	// �񋓌^��`
	enum STEP
	{
		STEP_WAIT = 0,
		STEP_SPEEDUP,
		STEP_SPEEDDOWN,
		STEP_IMPACTWAVE,
		STEP_DIRECTWAVE,
		STEP_MAX
	};

	enum STEPFRAG
	{
		STEPFRAG_SPEEDUP = 0,	// ����
		STEPFRAG_SPEEDDOWN,		// ����
		STEPFRAG_IMPACTWAVE,	// �~�`�g
		STEPFRAG_DIRECTWAVE,	// �����g
		STEPFRAG_MAX
	};

	CTutorialStep();
	~CTutorialStep();

	static CTutorialStep *Create(void);	// ��������

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void AddStep(void);	// �X�e�b�v��i�߂�
	void SetStep(STEP step);	// �X�e�b�v�̐ݒ菈��
	STEP GetNowStep(void);	// ���݂̃X�e�b�v�擾
	bool IsEndStep(void);	// �X�e�b�v�I���̔���擾
	void SetDisableOKSign(void);	// OK�T�C���̃t���OOFF�ɂ���
private:

	int m_nCntWait;			// �ҋ@�̃J�E���^�[
	int m_nCntSpeedUP;		// �X�s�[�h�A�b�v�̃J�E���^�[
	int m_nCntSpeedDOWN;	// �X�s�[�h�_�E���̃J�E���^�[
	int m_nCntImpactWave;	// �~�`�g�̉�
	int m_nCntDirectWave;	// �����g�̉�
	bool m_bEndStep;		// �X�e�b�v�̏I������
	bool m_bSetOK;			// OK�̐ݒ蔻��
	STEP m_step;			// ���݂̃X�e�b�v
};



#endif