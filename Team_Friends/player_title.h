//=============================================================================
// 
//  �v���C���[�w�b�_�[ [player_title.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYER_TITLE_H_
#define _PLAYER_TITLE_H_	// ��d�C���N���[�h�h�~

#include "player.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�N���X��`
class CPlayerTitle : public CPlayer
{
public:

	//�^�C�v
	enum MODEL
	{
		PLAYER_UNION = 0, //���̂�[��
		PLAYER_ARM, //�r�̂��
		PLAYER_LEG, //���̂��
		PLAYER_BODY, //���̂��
		MAX,
		NONE
	};

	CPlayerTitle(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CPlayerTitle();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �ÓI�����o�֐�
	static CPlayerTitle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type);

private:

	//�����o�ϐ�
	int m_nModelType;

};

#endif
