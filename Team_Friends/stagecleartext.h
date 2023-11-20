//=============================================================================
// 
// �X�e�[�W�N���A�e�L�X�g���� [stagecleartext.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STAGECLEARTEXT_H_
#define _STAGECLEARTEXT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�[�\���N���X��`
class CStageClearText : public CObject2D
{
public:

	CStageClearText(int nPriority = 8);
	~CStageClearText();

	static CStageClearText *Create(const D3DXVECTOR3 pos);
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

private:

	// ��ԗ�
	enum eState
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_SCALEUP,		// �g��
		STATE_SCALEDOWN,	// �k��
		STATE_SCALENONE,	// ����
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	// �����o�֐�
	void StateNone(void);		// �����Ȃ����
	void StateScaleUP(void);	// �g����
	void StateScaleDOWN(void);	// �k�����
	void StateScaleNone(void);	// �������
	void StateFadeOut(void);	// �t�F�[�h�A�E�g���

	// �����o�ϐ�
	int m_nTexIdx;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nCntState;	// ��ԑJ�ڃJ�E���^�[
	eState m_state;		// ���

	typedef void(CStageClearText::*STATE_FUNC)(void);	// ��ԏ����̃��X�g
	static STATE_FUNC m_StateFuncList[];
};

#endif