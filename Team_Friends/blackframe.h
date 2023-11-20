//=============================================================================
// 
//  ���t���[���w�b�_�[ [blackframe.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BLACKFRAME_H_
#define _BLACKFRAME_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D;

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// ���t���[���N���X��`
class CBlackFrame
{
public:

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,
		STATE_IN,				// �N��
		STATE_OUT,				// �J����
		STATE_INCOMPLETION,		// �N������
		STATE_OUTCOMPLETION,	// �J���銮��
		STATE_MAX
	};

	CBlackFrame(int nPriority = 8);
	~CBlackFrame();

	static CBlackFrame *Create(void);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state);	// ��Ԑݒ�
	void Reset(void);			// ���Z�b�g
private:
	// �񋓌^��`
	enum VTX
	{
		VTX_UP = 0,		// ��
		VTX_DOWN,		// ��
		VTX_MAX
	};

	// �����o�֐�
	void UpdateState(int nCntVtx);	// ��ԍX�V����
	void StateIn(int nCntVtx);	// �N��
	void StateOut(int nCntVtx);	// �J��

	// �����o�ϐ�
	STATE m_state;	// ���
	int m_nCntMove;	// �ړ��J�E���g
	CObject2D *m_pObj2D[VTX_MAX];	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	static const D3DXVECTOR3 m_DestPosition[VTX_MAX];	// �ڕW�̈ʒu
};



#endif