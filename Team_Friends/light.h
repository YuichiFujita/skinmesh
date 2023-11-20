//=============================================================================
//
// ���C�g���� [light.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _LIGHT_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _LIGHT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���C�g�N���X��`
class CLight
{
public:

	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void UpdateSpotLightDirection(D3DXVECTOR3 vec);	// �X�|�b�g���C�g�̌����X�V
private:

	enum TYPE
	{
		TYPE_DIRECTIONAL_01 = 0,
		TYPE_DIRECTIONAL_02,
		TYPE_DIRECTIONAL_03,
		TYPE_SPOT_01,
		TYPE_MAX,
	};
	
	D3DLIGHT9 m_aLight[TYPE_MAX];		// ���C�g�\����
};

#endif