//=============================================================================
// 
//  �ˏo��w�b�_�[ [injectiontable.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _INJECTIONTABLE_H_
#define _INJECTIONTABLE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "objectX.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ˏo��N���X��`
class CInjectionTable : public CObjectX
{
public:

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_CHARGE,	// �`���[�W
		STATE_UP,		// �㏸
		STATE_MAX
	};

	CInjectionTable(int nPriority = 6);
	~CInjectionTable();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	void SetState(STATE state, int nCntState);	// ��Ԑݒ�
	void SetState(STATE state);					// ��Ԑݒ�
	STATE GetState(void);	// ��Ԏ擾

	static CInjectionTable *Create(const D3DXVECTOR3 pos);	// ��������

private:
	void StateNone(void);	// �����Ȃ����
	void StateCharge(void);	// �`���[�W���
	void StateUP(void);		// �㏸���

	STATE m_state;			// ���
	int m_nCntState;		// ��ԑJ�ڃJ�E���^�[
	int m_nModelIdx;		// ���f���̃C���f�b�N�X�ԍ�

	typedef void(CInjectionTable::*STATE_FUNC)(void);
	static STATE_FUNC m_StateFuncList[];
};


#endif