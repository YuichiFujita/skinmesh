//==========================================
//
//  ���S�̃N���X(logo.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================
// �ÓI�����o�ϐ��錾
//==========================================
const char* CLogo::m_apTextureFile[] =
{
	"data\\TEXTURE\\title\\title_00.png",
	"data\\TEXTURE\\title\\title_01.png",
	"data\\TEXTURE\\title\\title_02.png",
	"data\\TEXTURE\\title\\title_03.png",
	"data\\TEXTURE\\title\\title_11.png"
};

//==========================================
//  �R���X�g���N�^
//==========================================
CLogo::CLogo()
{
	m_bComplete = false;
}

//==========================================
//  �f�X�g���N�^
//==========================================
CLogo::~CLogo()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CLogo::Init(void)
{
	//����������
	HRESULT hr = CObject3D::Init();

	//�^�C�v�̐ݒ�
	SetType(TYPE_OBJECT3D);

	//�T�C�Y��ݒ�
	SetSize(D3DXVECTOR3(24.0f, 6.0f, 0.0f));

	return hr;
}

//==========================================
//  �I������
//==========================================
void CLogo::Uninit(void)
{
	//�I��
	CObject3D::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CLogo::Update(void)
{
	//�X�V
	CObject3D::Update();
}

//==========================================
//  �`�揈��
//==========================================
void CLogo::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�`��
	CObject3D::Draw();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
