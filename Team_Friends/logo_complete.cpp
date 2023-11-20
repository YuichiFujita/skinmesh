//==========================================
//
//  �������S(logo_complete.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_complete.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CLogo_Comp::CLogo_Comp()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CLogo_Comp::~CLogo_Comp()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CLogo_Comp::Init(void)
{
	//����������
	HRESULT hr = CLogo::Init();

	//�e�N�X�`���̊��蓖��
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[3]));

	return hr;
}

//==========================================
//  �I������
//==========================================
void CLogo_Comp::Uninit(void)
{
	//�I��
	CLogo::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CLogo_Comp::Update(void)
{
	//�X�V
	CLogo::Update();
}

//==========================================
//  �`�揈��
//==========================================
void CLogo_Comp::Draw(void)
{	
	//�`��
	CLogo::Draw();
}

//==========================================
//  ��������
//==========================================
CLogo_Comp* CLogo_Comp::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//�C���X�^���X����
	CLogo_Comp *pLogo = DEBUG_NEW CLogo_Comp;

	//����������
	pLogo->Init();

	//�l��ݒ�
	pLogo->SetPosition(pos);
	pLogo->SetRotation(rot);

	return pLogo;
}
