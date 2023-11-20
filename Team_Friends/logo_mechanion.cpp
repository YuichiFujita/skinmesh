//==========================================
//
//  MECHANION�̃��S(logo_mechanion.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_mechanion.h"
#include "manager.h"
#include "texture.h"
#include "input.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CLogo_Mech::CLogo_Mech()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CLogo_Mech::~CLogo_Mech()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CLogo_Mech::Init(void)
{
	//����������
	HRESULT hr = CLogo::Init();

	//�e�N�X�`���̊��蓖��
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[0]));

	//������ԂŐ�������
	SetComplete(true);

	return hr;
}

//==========================================
//  �I������
//==========================================
void CLogo_Mech::Uninit(void)
{
	//�I��
	CLogo::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CLogo_Mech::Update(void)
{
	//�X�V
	CLogo::Update();
}

//==========================================
//  �`�揈��
//==========================================
void CLogo_Mech::Draw(void)
{
	//�`��
	CLogo::Draw();
}

//==========================================
//  ��������
//==========================================
CLogo_Mech* CLogo_Mech::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//�C���X�^���X����
	CLogo_Mech* pLogo = DEBUG_NEW CLogo_Mech;

	//����������
	pLogo->Init();

	//�l��ݒ�
	pLogo->SetPosition(pos);
	pLogo->SetRotation(rot);

	return pLogo;
}
