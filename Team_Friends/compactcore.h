//=============================================================================
// 
//  �R���p�N�g�R�A�w�b�_�[ [compactcore.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _COMPACTCORE_H_
#define _COMPACTCORE_H_	// ��d�C���N���[�h�h�~

#include "objectX.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �R���p�N�g�R�A�N���X��`
class CCompactCore : public CObjectX
{
public:

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_GET,		// �l��
		STATE_FADEOUT,	// �t�F�[�h�A�E�g���
		STATE_MAX
	};

	CCompactCore(int nPriority = 6);
	~CCompactCore();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetState(STATE state, int nCntState);	// ��Ԑݒ�
	STATE GetState(void);	// ��Ԏ擾

	static int GetNumAll(void);
	static CCompactCore *Create(const D3DXVECTOR3 pos);

private:
	void UpdatePos(void);			// �ړ�
	void CollisionPlayer(void);		// �v���C���[�Ƃ̔���
	
	void StateNone(void);		// �����Ȃ����
	void StateGet(void);		// �擾���
	void StateFadeOut(void);	// �t�F�[�h�A�E�g����


	STATE m_state;				// ���
	int m_nCntState;			// ��ԑJ�ڃJ�E���^�[
	int m_nLife;				// ����
	int m_nLifeMax;				// �����̍ő�l
	static int m_nNumAll;		// �e�̑���
	int m_nCntEmission;			// �������̃J�E���^�[
	int m_nNumGetPlayer;		// �擾���Ă�v���C���[�̐�

	typedef void(CCompactCore::*STATE_FUNC)(void);
	static STATE_FUNC m_StateFuncList[];
};


#endif