//==========================================
//
//  ����̃��S(logo_squadron.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_squadron.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
const float CLogo_Sqou::m_fFlashTime = 0.1f;
const int CLogo_Sqou::m_nFlashNum = 2;

//==========================================
//  �R���X�g���N�^
//==========================================
CLogo_Sqou::CLogo_Sqou()
{
	m_nCntFlash = 0;
	m_fTime = 0.0f;

}

//==========================================
//  �f�X�g���N�^
//==========================================
CLogo_Sqou::~CLogo_Sqou()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CLogo_Sqou::Init(void)
{
	//����������
	HRESULT hr = CLogo::Init();

	// ������Ԃŏo��
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	//�e�N�X�`���̊��蓖��
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[2]));

	return hr;
}

//==========================================
//  �I������
//==========================================
void CLogo_Sqou::Uninit(void)
{
	//�I��
	CLogo::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CLogo_Sqou::Update(void)
{
	// �o�ߎ��Ԃ��擾
	m_fTime += CManager::GetInstance()->GetDeltaTime();

	// �F�̕ύX����
	if (m_nCntFlash <= m_nFlashNum)
	{
		// ��莞�Ԍo��
		if (m_fFlashTime <= m_fTime)
		{
			m_fTime = 0.0f; // ���Ԃ̃��Z�b�g

			if (GetColor().a == 0.0f)
			{
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (GetColor().a == 1.0f)
			{
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				m_nCntFlash++; // �񐔂����Z
			}
		}
	}
	else if(GetColor().a < 1.0f)
	{
		// �s�������甭�F����
		if (m_fTime >= 1.0f)
		{
			m_fTime = 1.0f;
		}
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fTime));
	}
	else
	{
		SetComplete(true);
	}

	//�X�V
	CLogo::Update();
}

//==========================================
//  �`�揈��
//==========================================
void CLogo_Sqou::Draw(void)
{
	//�`��
	CLogo::Draw();
}

//==========================================
//  ��������
//==========================================
CLogo_Sqou* CLogo_Sqou::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//�C���X�^���X����
	CLogo_Sqou* pLogo = DEBUG_NEW CLogo_Sqou;

	//����������
	pLogo->Init();

	//�l��ݒ�
	pLogo->SetPosition(pos);
	pLogo->SetRotation(rot);

	return pLogo;
}
