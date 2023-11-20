//=============================================================================
// 
//  �J�ڂȂ��t�F�[�h�w�b�_�[ [instantfade.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _INSTANTFADE_H_
#define _INSTANTFADE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �J�ڂȂ��t�F�[�h�N���X��`
class CInstantFade
{
public:

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,			// �������Ă��Ȃ�
		STATE_FADEOUT,			// �t�F�[�h�A�E�g
		STATE_FADEIN,			// �t�F�[�h�C��
		STATE_FADECOMPLETION,	// �t�F�[�h����
		STATE_MAX
	};

	CInstantFade();
	~CInstantFade();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CObject2D *GetMyObject(void);
	static CInstantFade *Create(void);
	void SetFade(D3DXCOLOR FadeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), int nDuration = 30);	// �t�F�[�h�ݒ�
	STATE GetState(void);	// ��Ԏ擾

private:

	int m_nDuration;			// �t�F�[�h�܂ł̎���
	CObject2D *m_aObject2D;		// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	STATE m_state;				// ���
};

#endif