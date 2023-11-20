//=============================================================================
// 
//  �`���[�g���A���v���C���[���� [tutorialplayer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "tutorial.h"
#include "tutorialplayer.h"
#include "tutorialstep.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "motion.h"
#include "sound.h"
#include "particle.h"
#include "scene.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTutorialPlayer::CTutorialPlayer(int nPriority) : CPlayer(nPriority)
{
	// �l�̃N���A
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTutorialPlayer::~CTutorialPlayer()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTutorialPlayer::Init(void)
{
	// �e�̏���������
	CPlayer::Init();

	return S_OK;
}

//==========================================================================
// �X�V����
//==========================================================================
void CTutorialPlayer::Update(void)
{
	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// �e�̍X�V����
	CPlayer::Update();

	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// �X�e�b�v���Ƃ̍X�V
	UpdateByStep();
}

//==========================================================================
// �X�e�b�v���Ƃ̍X�V����
//==========================================================================
void CTutorialPlayer::UpdateByStep(void)
{
	// �X�e�b�v�̐ݒ�
	//CTutorial::GetStep()->SetStep(CTutorialStep::STEPFRAG_SPEEDUP);
}


//==========================================================================
// ���쏈��
//==========================================================================
void CTutorialPlayer::Controll(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̌����擾
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// ���݂̎�ގ擾
	int nType = m_pMotion->GetType();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	if (m_pMotion->IsGetMove(nType) == 1)
	{// �ړ��\���[�V�����̎�

#ifndef AUTO_MOVE
		// �ړ����ɂ���
		m_sMotionFrag.bMove = true;
#endif

		if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(0).x < 0)
		{//���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//A+W,����ړ�
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//A+S,�����ړ�
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,���ړ�
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(0).x > 0)
		{//D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//D+W,�E��ړ�
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//D+S,�E���ړ�
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,�E�ړ�
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
	}
	else
	{// �ړ��\���[�V��������Ȃ��ꍇ

		if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(0).x < 0)
		{//���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//A+W,����ړ�
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//A+S,�����ړ�
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,���ړ�
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(0).x > 0)
		{//D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//D+W,�E��ړ�
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//D+S,�E���ړ�
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,�E�ړ�
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
	}

	//�p�x�̐��K��
	RotNormalize(fRotDest);

	//���݂ƖڕW�̍��������߂�
	float fRotDiff = fRotDest - rot.y;

	//�p�x�̐��K��
	RotNormalize(fRotDiff);

	//�p�x�̕␳������
	rot.y += fRotDiff * 0.1f;

	//�p�x�̐��K��
	RotNormalize(rot.y);

#ifndef AUTO_MOVE
	move.x += sinf(D3DX_PI + rot.y) * fMove;
	move.z += cosf(D3DX_PI + rot.y) * fMove;
#endif

	// �d�͏���
	move.y -= mylib_const::GRAVITY;

	// �ʒu�X�V
	pos += move;

	// �����␳
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);

	//**********************************
	// �����蔻��
	//**********************************
	//Collision();

	if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0) == true) &&
		m_sMotionFrag.bATK == false)
	{// �U��

		// �U������ON
		m_sMotionFrag.bATK = true;
	}
}