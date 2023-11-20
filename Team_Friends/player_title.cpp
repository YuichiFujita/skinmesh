//=============================================================================
// 
//  �v���C���[���� [player_title.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "game.h"
#include "player_title.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "model.h"
#include "motion.h"
#include "objectChara.h"
#include "shadow.h"

//==========================================
//  �萔��` : �������
//==========================================
namespace
{
	const char* CHARAFILE[CPlayerTitle::MAX] =
	{
		"data\\TEXT\\sample\\motion_union_player.txt", // ���̃t�@�C���p�X
		"data\\TEXT\\sample\\motion_ArmToArm.txt", // �r�t�@�C���p�X
		"data\\TEXT\\character\\player\\motion_player.txt", // ���t�@�C���p�X
		"data\\TEXT\\character\\player\\motion_player.txt" // ���t�@�C���p�X
	};
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerTitle::CPlayerTitle(int nPriority) : CPlayer(nPriority),
m_nModelType(NONE)
{
	// �l�̃N���A
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerTitle::~CPlayerTitle()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerTitle::Init(void)
{
	// ��ނ̐ݒ�
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// ���
	m_bLandOld = true;		// �O��̒��n���

	// �L�����쐬
	HRESULT hr = SetCharacter(CHARAFILE[m_nModelType]);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���[�V�����̐�������
	m_pMotion = CMotion::Create(CHARAFILE[m_nModelType]);

	// �I�u�W�F�N�g�L�����N�^�[�̏��擾
	CObjectChara *pObjChar = GetObjectChara();

	// ���[�V�����̐ݒ�
	m_pMotion->SetModel(pObjChar->GetModel(), pObjChar->GetNumModel(), CObjectChara::GetObjectChara());

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �|�[�Y�̃��Z�b�g
	m_pMotion->ResetPose(MOTION_DEF);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerTitle::Uninit(void)
{
	// �I������
	CPlayer::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerTitle::Update(void)
{
	if (IsDeath() == true)
	{
		return;
	}

	// �ߋ��̈ʒu�ۑ�
	SetOldPosition(GetPosition());

	// ���[�V�����X�V
	if (m_pMotion != NULL)
	{
		m_pMotion->Update();
	}

	// �e�̈ʒu�X�V
	if (m_pShadow != NULL)
	{
		// �ʒu�擾
		D3DXVECTOR3 pos = GetPosition();

		m_pShadow->SetPosition(D3DXVECTOR3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayerTitle::Draw(void)
{
	// �`�揈��
	CPlayer::Draw();
}

//==========================================
//  �������� : �������
//==========================================
CPlayerTitle* CPlayerTitle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type)
{
	// �C���X�^���X����
	CPlayerTitle* pPlayer = DEBUG_NEW CPlayerTitle;

	//���f����ݒ�
	pPlayer->m_nModelType = (int)type;

	// ����������
	pPlayer->Init();

	// �l��ۑ�
	pPlayer->SetPosition(pos);
	pPlayer->SetRotation(rot);

	return pPlayer;
}
