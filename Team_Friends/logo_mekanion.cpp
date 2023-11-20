//==========================================
//
//  ���J�j�I���̃��S(logo_mekanion.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo_mekanion.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "debugproc.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CLogo_Meka::CLogo_Meka()
{
	m_posVirtual = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================
//  �f�X�g���N�^
//==========================================
CLogo_Meka::~CLogo_Meka()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CLogo_Meka::Init(void)
{
	//����������
	HRESULT hr = CLogo::Init();

	// �|������Ԃ������l�ɂ���
	D3DXVECTOR3 rot = GetRotation(); // �p�x���擾
	rot.x = D3DX_PI * -0.5f; // �|��
	SetRotation(rot);

	//���z���S�̐ݒ�
	D3DXVECTOR3 pos = GetPosition(); // ���S���W���擾
	D3DXVECTOR3 size = GetSize(); // �|���S���̑傫�����擾
	m_posVirtual = pos; // ���z���S��������
	m_posVirtual.y = pos.y - size.y; // ���W��␳����

	// ���z���S�ɍ��킹�Ď��ۂ̒��S���W��␳����
	pos.z = sinf(rot.x) * size.y + m_posVirtual.z; // z
	pos.y = cosf(rot.x) * size.y + m_posVirtual.y; // y
	SetPosition(pos);  //�ݒ�

	//�e�N�X�`���̊��蓖��
	this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[1]));

	return hr;
}

//==========================================
//  �I������
//==========================================
void CLogo_Meka::Uninit(void)
{
	//�I��
	CLogo::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CLogo_Meka::Update(void)
{
	//��]
	D3DXVECTOR3 rot = GetRotation();
	rot.x += 0.02f;
	if (rot.x >= 0.0f)
	{
		rot.x = 0.0f;
		SetComplete(true);
	}
	SetRotation(rot);

	// ���z���S�ɍ��킹�Ď��ۂ̒��S���W��␳����
	D3DXVECTOR3 pos = GetPosition(); // ���S���W���擾
	D3DXVECTOR3 size = GetSize(); // �|���S���̑傫�����擾
	pos.z = sinf(rot.x) * size.y + m_posVirtual.z; // z
	pos.y = cosf(rot.x) * size.y + m_posVirtual.y; // y
	SetPosition(pos);  //�ݒ�

#ifdef _DEBUG

	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_6))
	{
		//�e�N�X�`���̊��蓖��
		this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[1]));
	}
	if (pInputKeyboard->GetTrigger(DIK_7))
	{
		//�e�N�X�`���̊��蓖��
		this->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[4]));
	}

#endif

	//�X�V
	CLogo::Update();
}

//==========================================
//  �`�揈��
//==========================================
void CLogo_Meka::Draw(void)
{
	//�`��
	CLogo::Draw();
}

//==========================================
//  ��������
//==========================================
CLogo_Meka* CLogo_Meka::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//�C���X�^���X����
	CLogo_Meka* pLogo = DEBUG_NEW CLogo_Meka;

	//�l��ݒ�
	pLogo->SetPosition(pos);

	//����������
	pLogo->Init();

	return pLogo;
}
