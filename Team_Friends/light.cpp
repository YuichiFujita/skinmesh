//=============================================================================
// 
//  ���C�g���� [light.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "light.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLight::CLight()
{
	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ���C�g�̏����N���A����
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLight::~CLight()
{

}

//==================================================================================
// ���C�g�̏���������
//==================================================================================
HRESULT CLight::Init(void)
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;	// �ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ���C�g�̏����N���A����
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		switch (nCntLight)
		{
		case TYPE_DIRECTIONAL_01:
			// ���C�g�̎�ނ�ݒ�
			m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;

		case TYPE_DIRECTIONAL_02:
			// ���C�g�̎�ނ�ݒ�
			m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			// ���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			break;

		case TYPE_DIRECTIONAL_03:
			// ���C�g�̎�ނ�ݒ�
			m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			// ���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			break;

		case TYPE_SPOT_01:
			// ���C�g�̎�ނ��X�|�b�g���C�g�ɐݒ�
			m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// �X�|�b�g���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

			// �X�|�b�g���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;
		}

		// �x�N�g���𐳋K������(1.0�ɂ���)
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_aLight[nCntLight].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//==================================================================================
// ���C�g�̏I������
//==================================================================================
void CLight::Uninit(void)
{

}

//==================================================================================
// ���C�g�̍X�V����
//==================================================================================
void CLight::Update(void)
{

}

//==================================================================================
// �X�|�b�g���C�g�̌����X�V
//==================================================================================
void CLight::UpdateSpotLightDirection(D3DXVECTOR3 vec)
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �X�|�b�g���C�g�̕�����ݒ�
	D3DXVECTOR3 vecDir = vec;

	// �x�N�g���𐳋K������(1.0�ɂ���)
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_aLight[TYPE_SPOT_01].Direction = vecDir;

	// ���C�g��ݒ肷��
	pDevice->SetLight(TYPE_SPOT_01, &m_aLight[TYPE_SPOT_01]);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(TYPE_SPOT_01, TRUE);
}
