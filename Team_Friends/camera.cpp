//=============================================================================
// 
// �J�������� [camera.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "calculation.h"
#include "player.h"
#include "elevation.h"
#include "enemymanager.h"
#include "title.h"
#include "instantfade.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MOVE			(2.5f)				// �ړ���
#define MAX_LENGTH		(50000.0f)			// �ő勗��
#define MIN_LENGTH		(10.0f)				// �ŏ�����
#define START_CAMERALEN	(1300.0f)			// ���̋���
#define ROT_MOVE_MOUSE	(0.01f)				// ��]�ړ���
#define ROT_MOVE_STICK	(0.0015f)			// ��]�ړ���
#define ROT_MOVE		(0.025f)			// ��]�ړ���
#define MIN_ROT			(-D3DX_PI * 0.49f)	// �J�����Œ�p
#define MAX_ROT			(D3DX_PI * 0.49f)	// �J�����Œ�p
#define BACKFOLLOW_TIME	(20)				// �w�ʕ␳�܂ł̎���
#define TITLE_POSR_DEST	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define TITLE_LEN_DEST	(3660.0f)
#define TITLECAMERAROT_NONE		(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.01f))
#define TITLECAMERAROT_ENEMY	(D3DXVECTOR3(0.0f, -0.79f, -0.12f))
#define TITLESTATE_CHANGE	(60 * 14)
#define TITLESTATE_CHASE	(60 * 20)
#define RESULT_LEN	(280.0f)
//#define RESULT_LEN	(1000.0f)
#define RANKINGROT_NONE		(D3DXVECTOR3(0.0f, -0.79f + D3DX_PI, -0.30f))

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCamera::CCamera()
{
	m_viewport.X = 0;							// �`�悷���ʂ̍���X���W
	m_viewport.Y = 0;							// �`�悷���ʂ̍���Y���W
	m_viewport.Width = 0;						// �`�悷���ʂ̕�
	m_viewport.Height = 0;						// �`�悷���ʂ̍���
	m_viewport.Width = 0;						// �`�悷���ʂ̕�
	m_viewport.Height = 0;						// �`�悷���ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;
	m_posR = mylib_const::DEFAULT_VECTOR3;		// �����_(�������ꏊ)
	m_posV = mylib_const::DEFAULT_VECTOR3;		// ���_(�J�����̈ʒu)
	m_posVDest = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̎��_
	m_posRDest = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̒����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_move = mylib_const::DEFAULT_VECTOR3;		// �ړ���
	m_rot = mylib_const::DEFAULT_VECTOR3;		// ����
	m_rotVDest = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̎��_�̌���
	m_TargetPos = mylib_const::DEFAULT_VECTOR3;	// �Ǐ]�ڕW�̈ʒu
	m_TargetRot = mylib_const::DEFAULT_VECTOR3;	// �Ǐ]�ڕW�̈ʒu
	m_fDistance = 0.0f;							// ����
	m_fDestDistance = 0.0f;						// �ڕW�̋���
	m_fOriginDistance = 0.0f;					// ���̋���
	m_nShakeLength = 0.0f;						// �h��̒���
	m_nShakeLengthY = 0.0f;						// Y�̗h��̒���
	m_fMoveShake = 0.0f;						// �h��̈ړ���
	m_fMoveShakeY = 0.0f;						// Y�̗h��̈ړ���
	m_fHeightMaxDest = 0.0f;					// �J�����̍ő卂���̖ڕW
	m_fHeightMax = 0.0f;			// �J�����̍ő卂��
	m_fDiffHeight = 0.0f;						// �����̍���
	m_fDiffHeightSave = 0.0f;					// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;					// �ڕW�̍����̍���
	m_bFollow = false;							// �Ǐ]���邩�ǂ���
	m_state = CAMERASTATE_NONE;					// ���
	m_nCntState = 0;							// ��ԃJ�E���^�[
	m_nCntDistance = 0;							// �����J�E���^�[
	m_nOriginCntDistance = 0;					// ���̋����J�E���^�[
	m_fDistanceCorrection = 0.0f;				// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;			// �����̌����W��
	m_nChasePlayerIndex = 0;					// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCamera::~CCamera()
{

}

//==================================================================================
// �J�����̏���������
//==================================================================================
HRESULT CCamera::Init(void)
{
	
	// �r���[�|�[�g�̐ݒ�
	SetViewPort(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// ���Z�b�g
	m_bFollow = true;	// �Ǐ]���邩�ǂ���
	Reset(CScene::MODE_GAME);

	// ���_�̑������
	SetCameraV();

	return S_OK;
}

//==================================================================================
// �r���[�|�[�g�̐ݒ�
//==================================================================================
void CCamera::SetViewPort(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_viewport.X = (DWORD)pos.x;			// �`�悷���ʂ̍���X���W
	m_viewport.Y = (DWORD)pos.y;			// �`�悷���ʂ̍���Y���W
	m_viewport.Width = (DWORD)size.x;		// �`�悷���ʂ̕�
	m_viewport.Height = (DWORD)size.y;		// �`�悷���ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

//==================================================================================
// �J�����̏I������
//==================================================================================
void CCamera::Uninit(void)
{

}

//==================================================================================
// �J�����̍X�V����
//==================================================================================
void CCamera::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//if (m_state == CAMERASTATE_NONE)
	{
		// �����̍������Z�b�g
		m_fDiffHeightSave = 0.0f;

		// ���_�E�����_�ړ�
		MoveCameraFollow();
		MoveCameraInput();
		MoveCameraVR();
		MoveCameraR();
		MoveCameraV();
		MoveCameraDistance();
		MoveCameraDistance();
		//UpdateByMode();

		if (m_state == CAMERASTATE_SHAKE)
		{
			UpdateState();
		}
	}

//#ifdef _DEBUG
	
	if (pInputKeyboard->GetTrigger(DIK_F7) == true)
	{// F7�������ꂽ,�Ǐ]�؂�ւ�

		m_bFollow = m_bFollow ? false : true;
	}

	if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{// F7�������ꂽ,�Ǐ]�؂�ւ�
		//m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);				// �����_(�������ꏊ)
		//m_posV = D3DXVECTOR3(0.0f, 200.0f, m_posR.z + -500.0f);	// ���_(�J�����̈ʒu)
		//m_posVDest = m_posV;									// �ڕW�̎��_
		//m_posRDest = m_posR;									// �ڕW�̒����_
		//m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					// ������x�N�g��
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ړ���
		m_rot = D3DXVECTOR3(m_rot.x, 0.0f, m_rot.z);					// ����
		m_rotVDest = m_rot;										// �ڕW�̎��_�̌���
		m_fOriginDistance = START_CAMERALEN;					// ���̋���
		m_fDestDistance = m_fOriginDistance;
		m_fDistance = m_fOriginDistance;
		//m_fDistance = START_CAMERALEN;							// ����
		//m_state = CAMERASTATE_NONE;								// ���

		//// ���_�̑������
		//SetCameraV();
	}
//#endif

	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- �J������� ----------------\n"
		"�y�����z[X�F%f Y�F%f Z�F%f]\n"
		"�y�����z[%f]\n"
		"�y���_�z[X�F%f Y�F%f Z�F%f]\n"
		"�y�����_�z[X�F%f Y�F%f Z�F%f]\n",
		m_rot.x, m_rot.y, m_rot.z,
		m_fDistance,
		m_posV.x, m_posV.y, m_posV.z,
		m_posR.x, m_posR.y, m_posR.z);
}

//==================================================================================
// ���[�h�ʍX�V����
//==================================================================================
void CCamera::UpdateByMode(void)
{
	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
		break;

	default:
		break;
	}
}

//==================================================================================
// ���͋@��̃J�����ړ�
//==================================================================================
void CCamera::MoveCameraInput(void)
{
#ifdef _DEBUG
	// �}�E�X�ł̈ړ�����
	MoveCameraMouse();
#endif

	// �X�e�B�b�N�ł̈ړ�����
	MoveCameraStick();
}

//==================================================================================
// �R���g���[���[�ł̈ړ�����
//==================================================================================
void CCamera::MoveCameraStick(void)
{
	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

#if 0
	m_rot.y += pInputGamepad->GetStickMoveR(0).x * ROT_MOVE_STICK;
	m_rot.z += pInputGamepad->GetStickMoveR(0).y * ROT_MOVE_STICK;
#endif
	// �p�x�̐��K��
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// ���_�̑������
	SetCameraV();
}

//==================================================================================
// �}�E�X�ł̈ړ�����
//==================================================================================
void CCamera::MoveCameraMouse(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �L�[�{�[�h���擾
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) == true &&
		pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT) == true)
	{// ���E��������

//#if _DEBUG
		m_move.x += (pInputMouse->GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) -
			(pInputMouse->GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);

		m_move.z += (pInputMouse->GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) +
			(pInputMouse->GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);
//#endif

		// �ړ��ʕ␳
		MoveCameraVR();

		// �p�x�̐��K��
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// �����_�ݒ�
		SetCameraR();
	}
	else if (pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) == true)
	{// ���N���b�N���Ă�Ƃ�,���_��]

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;

//#if _DEBUG

		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;	
//#endif

		// �p�x�̐��K��
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// �l�̐��K��
		ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// ���_�̑������
		SetCameraV();

	}
	else if (pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT) == true)
	{// �E�N���b�N���Ă�Ƃ�,�����_��]

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;

//#if _DEBUG
		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;
//#endif

		// �p�x�̐��K��
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// �l�̐��K��
		ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// �����_�̈ʒu�X�V
		SetCameraR();
	}

	// �}�E�X�z�C�[���ŋ�������
	m_fDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);
	m_fDestDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);
	m_fOriginDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);

	// ���_�̑������
	SetCameraV();
}

//==================================================================================
// �J�����̎��_�ړ�
//==================================================================================
void CCamera::MoveCameraV(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

#ifdef _DEBUG
	// ���_�ړ�
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{// Y�L�[�������ꂽ,���_��ړ�

		m_rot.z -= ROT_MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_N) == true)
	{// N�L�[�������ꂽ,���_���ړ�

		m_rot.z += ROT_MOVE;
	}

	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{// Z�L�[�������ꂽ

		m_rot.y += ROT_MOVE;
	}
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{// C�L�[�������ꂽ

		m_rot.y -= ROT_MOVE;
	}
#endif

#if 0
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{// Z�L�[�������ꂽ

		m_rot.y += ROT_MOVE * 1.8f;
	}
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{// C�L�[�������ꂽ

		m_rot.y -= ROT_MOVE * 1.8f;
	}
#endif


	// �p�x�̐��K��
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// �l�̐��K��
	ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// ���_�̑������
	SetCameraV();

}

//==================================================================================
// �J�����̒����_�ړ�
//==================================================================================
void CCamera::MoveCameraR(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

#ifdef _DEBUG
	// ����
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{// Q�L�[�������ꂽ,������


		m_rot.y -= ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_E) == true)
	{// E�L�[�������ꂽ,������

		m_rot.y += ROT_MOVE;
	}

	// �㏸���~
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{// T�L�[�������ꂽ

		m_rot.z += ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_B) == true)
	{// B�L�[�������ꂽ

		m_rot.z -= ROT_MOVE;

	}
#endif


	// �p�x�̐��K��
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// �l�̐��K��
	ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// �����_�̈ʒu�X�V
	SetCameraR();
}

//==================================================================================
// �J�����̎��_�E�����_�ړ�
//==================================================================================
void CCamera::MoveCameraVR(void)
{

	// �ړ��ʕ������Z
	m_posV.x += m_move.x;
	m_posV.z += m_move.z;

	// �ړ��ʂ����Z�b�g
	m_move.x = 0.0f;
	m_move.z = 0.0f;

	// �����_�̈ʒu�X�V
	SetCameraR();
}

//==================================================================================
//  ���_�E�����_�ԋ���
//==================================================================================
void CCamera::MoveCameraDistance(void)
{
	// �����J�E���^�[���Z
	m_nCntDistance--;

	if (m_nCntDistance <= 0)
	{
		m_nCntDistance = 0;

		if (m_fDestDistance >= m_fOriginDistance)
		{// �J�������߂Â��Ă���
			m_fDestDistance -= m_fDistanceDecrementValue;
		}

		if (m_fDestDistance <= m_fOriginDistance)
		{// �␳����������߂�
			m_fDestDistance = m_fOriginDistance;
		}
	}

	// ���E�l�␳
	if (m_fDistance >= MAX_LENGTH)
	{
		m_fDistance = MAX_LENGTH;
	}
	else if (m_fDistance <= MIN_LENGTH)
	{
		m_fDistance = MIN_LENGTH;
	}

	// �ڕW�̋����֕␳
	InertiaCorrection(m_fDistance, m_fDestDistance, m_fDistanceCorrection);

}

//==================================================================================
// �J�����̎��_�������
//==================================================================================
void CCamera::SetCameraV(void)
{

	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		SetCameraVTitle();
		break;

	case CScene::MODE_TUTORIAL:
		SetCameraVGame();
		break;

	case CScene::MODE_GAME:
		SetCameraVGame();
		break;

	case CScene::MODE_RESULT:
		SetCameraVResult();
		break;

	default:
		SetCameraVResult();
		break;
	}

}

//==================================================================================
// �J�����̎��_�������(�^�C�g��)
//==================================================================================
void CCamera::SetCameraVTitle(void)
{
	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else if (m_bFollow == true)
	{// �Ǐ]ON

		// ���_�̑������
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		float fDistance = 0.0f;
		m_fHeightMaxDest = m_posVDest.y;
		// �ڕW�̊p�x�����߂�
		float fRotDest = atan2f((m_posVDest.x - m_posR.x), (m_posVDest.z - m_posR.z));
		while (1)
		{
			
			// ���z�̒e�̈ʒu
			float fPosBulletX = m_TargetPos.x + cosf(m_rot.z) * sinf(m_rot.y) * -fDistance;
			float fPosBulletZ = m_TargetPos.z + cosf(m_rot.z) * cosf(m_rot.y) * -fDistance;

			// �����擾
			bool bLand = false;
			float fHeight = CGame::GetElevation()->GetHeight(D3DXVECTOR3(fPosBulletX, 0.0f, fPosBulletZ), bLand);

			if (m_fHeightMaxDest <= fHeight)
			{// �ő�̍������X�V������

				// �����̉����������ۑ�
				float fDistanceRatio = fDistance / (m_fDistance);

				// �ڕW�̍ő卂���ۑ�
				m_fHeightMaxDest = fHeight * (fDistanceRatio + 1.0f);
			}

			// �������Z
			fDistance += 10.0f;

			if (fDistance >= m_fDistance)
			{// �����𒴂�����I���
				break;
			}
		}

		// �ڕW�̎��_�X�V
		if (m_fHeightMaxDest > m_posVDest.y)
		{
			// �ő�̍����␳
			m_fHeightMax += (m_fHeightMaxDest - m_fHeightMax) * 0.08f;

			m_posVDest.y = m_fHeightMax;

			// �����̍���
			m_fDiffHeightSave += m_fHeightMax - m_posV.y;
		}

		// �␳����
		m_posV += (m_posVDest - m_posV) * 0.12f;
	}
}

//==================================================================================
// �J�����̎��_�������(�Q�[��)
//==================================================================================
void CCamera::SetCameraVGame(void)
{
	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else if (m_bFollow == true)
	{// �Ǐ]ON


		float fYcamera = 100.0f;

		// ���_�̑������
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		float fDistance = 0.0f;
		m_fHeightMaxDest = m_posVDest.y;
		// �ڕW�̊p�x�����߂�
		float fRotDest = atan2f((m_posVDest.x - m_posR.x), (m_posVDest.z - m_posR.z));
		while (1)
		{

			// ���z�̒e�̈ʒu
			float fPosBulletX = m_TargetPos.x + cosf(m_rot.z) * sinf(m_rot.y) * -fDistance;
			float fPosBulletZ = m_TargetPos.z + cosf(m_rot.z) * cosf(m_rot.y) * -fDistance;

			// �����擾
			bool bLand = false;
			float fHeight = CGame::GetElevation()->GetHeight(D3DXVECTOR3(fPosBulletX, 0.0f, fPosBulletZ), bLand);

			if (m_fHeightMaxDest <= fHeight)
			{// �ő�̍������X�V������

				// �����̉����������ۑ�
				float fDistanceRatio = fDistance / (m_fDistance);

				// �ڕW�̍ő卂���ۑ�
				m_fHeightMaxDest = fHeight * (fDistanceRatio + 1.0f);
			}

			// �������Z
			fDistance += 10.0f;

			if (fDistance >= m_fDistance)
			{// �����𒴂�����I���
				break;
			}
		}

		// �ڕW�̎��_�X�V
		if (m_fHeightMaxDest > m_posVDest.y)
		{
			// �ő�̍����␳
			m_fHeightMax += (m_fHeightMaxDest - m_fHeightMax) * 0.08f;

			m_posVDest.y = m_fHeightMax;

			// �����̍���
			m_fDiffHeightSave += m_fHeightMax - m_posV.y;
		}

		// �␳����
		m_posV += (m_posVDest - m_posV) * 0.12f;
	}
}

//==================================================================================
// �J�����̎��_�������(���U���g)
//==================================================================================
void CCamera::SetCameraVResult(void)
{
	// ���_�̑������
	m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
}

//==================================================================================
// �J�����̒����_�������
//==================================================================================
void CCamera::SetCameraR(void)
{

	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		SetCameraRTitle();
		break;

	case CScene::MODE_TUTORIAL:
		SetCameraRGame();
		break;

	case CScene::MODE_GAME:
		SetCameraRGame();
		break;

	case CScene::MODE_RESULT:
		SetCameraRResult();
		break;

	default:
		SetCameraRResult();
		break;
	}

}

//==================================================================================
// �J�����̒����_�������(�^�C�g��)
//==================================================================================
void CCamera::SetCameraRTitle(void)
{
	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// �����_�̑������
		m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{// �Ǐ]ON

		float fYcamera = 100.0f;

		if (m_TargetPos.y >= 50.0f)
		{
			fYcamera = (m_TargetPos.y - 50.0f) + 100.0f;
		}
		else if (m_TargetPos.y <= -50.0f)
		{
			fYcamera = (m_TargetPos.y + 50.0f) + 100.0f;
		}

		fYcamera = m_TargetPos.y + 100.0f;

		if (fYcamera <= 0.0f)
		{
			m_fDiffHeightSave += -fYcamera;
		}

		// �ڕW�̍����̍�������
		m_fDiffHeightDest = m_fDiffHeightSave;

		// �����̍�����␳����
		m_fDiffHeight += (m_fDiffHeightDest - m_fDiffHeight) * 0.001f;


		// �����_�̑������
		m_posRDest.x = m_TargetPos.x;
		m_posRDest.z = m_TargetPos.z;
		m_posRDest.y = fYcamera - m_fDiffHeight;

		// �␳����
		m_posR += (m_posRDest - m_posR) * 0.08f;
	}
}

//==================================================================================
// �J�����̒����_�������(�Q�[��)
//==================================================================================
void CCamera::SetCameraRGame(void)
{
	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// �����_�̑������
		m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{// �Ǐ]ON

		float fYcamera = 100.0f;

		if (m_TargetPos.y >= 50.0f)
		{
			fYcamera = (m_TargetPos.y - 50.0f) + 100.0f;
		}
		else if (m_TargetPos.y <= -50.0f)
		{
			fYcamera = (m_TargetPos.y + 50.0f) + 100.0f;
		}

		fYcamera = m_TargetPos.y + 100.0f;

		if (fYcamera <= 0.0f)
		{
			m_fDiffHeightSave += -fYcamera;
		}

		// �ڕW�̍����̍�������
		m_fDiffHeightDest = m_fDiffHeightSave;

		// �����̍�����␳����
		m_fDiffHeight += (m_fDiffHeightDest - m_fDiffHeight) * 0.01f;

		// �����_�̑������
		m_posRDest.x = (m_TargetPos.x + sinf(D3DX_PI + m_TargetRot.y) * 150.0f);
		m_posRDest.z = (m_TargetPos.z + cosf(D3DX_PI + m_TargetRot.y) * 150.0f);
		m_posRDest.y = fYcamera - m_fDiffHeight;

		// �␳����
		m_posR += (m_posRDest - m_posR) * 0.08f;
	}
}

//==================================================================================
// �J�����̒����_�������(���U���g)
//==================================================================================
void CCamera::SetCameraRResult(void)
{
	// �����_�̑������
	m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
	m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
}

//==================================================================================
// �J�����̔w�ʎ����Ǐ]
//==================================================================================
void CCamera::MoveCameraFollow(void)
{

}

//==================================================================================
// �J�����̐ݒ菈��
//==================================================================================
void CCamera::SetCamera(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),	// ����p
								(float)m_viewport.Width / (float)m_viewport.Height,	// �A�X�y�N�g��
								10.0f,		// ���s���̐���
								200000.0f);	// ���s���̐���

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,	// ���_
		&m_posR,	// �����_
		&m_vecU);	// ������x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

}

//==================================================================================
// �ڕW�̒����ݒ�
//==================================================================================
void CCamera::SetLenDest(float fLength, int nCntTime, float DecrementValue, float fCorrection)
{
	// �ڕW�̋����ݒ�
	m_fDestDistance = fLength;
	m_nCntDistance = nCntTime;
	m_nOriginCntDistance = m_nCntDistance;	// ���̋����J�E���^�[
	m_fDistanceCorrection = fCorrection;	// �����␳�W��
	m_fDistanceDecrementValue = DecrementValue;	// ������
}

//==================================================================================
// �J�����̐U���ݒ菈��
//==================================================================================
void CCamera::SetShake(int nTime, float fLength, float fLengthY)
{
	// �U����Ԃɐݒ�
	m_state = CAMERASTATE_SHAKE;
	m_nCntState = nTime;		// ��ԑJ�ڃJ�E���^�[
	m_nShakeLength = fLength;	// �h��̑傫��
	m_nShakeLengthY = fLengthY;	// Y�̗h��̑傫��
}

//==================================================================================
// �J�����̐U������
//==================================================================================
void CCamera::Shake(void)
{
	// �����擾
	int nLength = (int)m_nShakeLength;
	int nLengthY = (int)m_nShakeLengthY;

	m_fMoveShake = (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);	// �h��̈ړ���
	m_fMoveShakeY = (float)Random(-nLengthY, nLengthY);						// Y�̗h��̈ړ���

	// ���_�̑������
	m_posV.x += (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);
	m_posV.y += (float)Random(-nLengthY, nLengthY);
	m_posV.z += (float)Random(-nLength, nLength) * cosf((float)Random(-314, 314) * 0.01f);

	// �����_�̑������
	m_posR.x += (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);
	m_posR.y += (float)Random(-nLengthY, nLengthY);
	m_posR.z += (float)Random(-nLength, nLength) * cosf((float)Random(-314, 314) * 0.01f);

	// �����␳
	InertiaCorrection(m_nShakeLength, 0.0f, 0.1f);
	InertiaCorrection(m_nShakeLengthY, 0.0f, 0.1f);
}

//==================================================================================
// �J�����̏�ԍX�V����
//==================================================================================
void CCamera::UpdateState(void)
{
	switch (m_state)
	{
	case CAMERASTATE_NONE:
		break;

	case CAMERASTATE_SHAKE:

		if (m_nCntState % 3 == 0)
		{
			// ��ʗh��
			Shake();
		}
		else
		{
			// ���_�̑������
			m_posV.x += m_fMoveShake;
			m_posV.y += m_fMoveShakeY;
			m_posV.z += m_fMoveShake;

			// �����_�̑������
			m_posR.x += m_fMoveShake;
			m_posR.y += m_fMoveShakeY;
			m_posR.z += m_fMoveShake;

			// �����␳
			InertiaCorrection(m_nShakeLength, 0.0f, 0.01f);
			InertiaCorrection(m_nShakeLengthY, 0.0f, 0.01f);
		}

		// ��ԑJ�ڃJ�E���^�[���Z
		m_nCntState--;

		if (m_nCntState <= 0)
		{
			m_nCntState = 0;
			m_state = CAMERASTATE_NONE;
		}
		break;
	}
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCamera::Reset(CScene::MODE mode)
{
	
	m_bFollow = true;										// �Ǐ]���邩�ǂ���

	switch (mode)
	{
	case CScene::MODE_NONE:
		break;

	case CScene::MODE_TITLE:
		ResetTitle();
		break;

	case CScene::MODE_DECIDEPLAYER:
		ResetResult();
		break;

	case CScene::MODE_TUTORIAL:
		ResetGame();
		break;

	case CScene::MODE_GAME:
		ResetGame();
		break;

	case CScene::MODE_RESULT:
		ResetResult();
		break;

	case CScene::MODE_RANKING:
		ResetRanking();
		break;

	case CScene::MODE_MAX:
		break;

	default:
		break;
	}

	// ���_�̑������
	SetCameraV();
}

//==========================================================================
// �Q�[���̃��Z�b�g
//==========================================================================
void CCamera::ResetGame(void)
{
	m_posR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);				// �����_(�������ꏊ)
	m_posV = D3DXVECTOR3(0.0f, 300.0f, m_posR.z + -400.0f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;									// �ڕW�̎��_
	m_posRDest = m_posR;									// �ڕW�̒����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					// ������x�N�g��
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, -0.40f);					// ����
	m_rotVDest = m_rot;										// �ڕW�̎��_�̌���
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̈ʒu
	m_fDistance = START_CAMERALEN;							// ����
	m_fDestDistance = START_CAMERALEN;						// �ڕW�̋���
	m_fOriginDistance = START_CAMERALEN;					// ���̋���
	m_fDiffHeight = 0.0f;									// �����̍���
	m_fDiffHeightSave = 0.0f;								// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;								// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;								// ���
	m_nCntState = 0;							// ��ԃJ�E���^�[
	m_nCntDistance = 0;							// �����J�E���^�[
	m_nOriginCntDistance = 0;					// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;					// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;			// �����̌����W��
	m_fHeightMaxDest = 0.0f;					// �J�����̍ő卂���̖ڕW
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCamera::ResetBoss(void)
{
	ResetGame();
}

//==========================================================================
// �^�C�g���̃��Z�b�g
//==========================================================================
void CCamera::ResetTitle(void)
{
	m_posR = D3DXVECTOR3(0.0f, 300.0f, 30.0f);	// �����_(�������ꏊ)
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;								// �ڕW�̎��_
	m_posRDest = m_posR;								// �ڕW�̒����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���
	m_rot = TITLECAMERAROT_NONE;						// ����
	m_rotVDest = m_rot;									// �ڕW�̎��_�̌���
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̈ʒu
	m_fDistance = TITLE_LEN_DEST;						// ����
	m_fDestDistance = TITLE_LEN_DEST;					// �ڕW�̋���
	m_fOriginDistance = TITLE_LEN_DEST;					// ���̋���
	m_fDiffHeight = 0.0f;								// �����̍���
	m_fDiffHeightSave = 0.0f;							// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;							// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;							// ���
	m_nCntState = 0;									// ��ԃJ�E���^�[
	m_nCntDistance = 0;									// �����J�E���^�[
	m_nOriginCntDistance = 0;							// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;							// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;					// �����̌����W��
	m_fHeightMaxDest = 0.0f;							// �J�����̍ő卂���̖ڕW
	m_bFollow = false; // �Ǐ]���Ȃ��ł�
}

//==========================================================================
// ���U���g���Z�b�g
//==========================================================================
void CCamera::ResetResult(void)
{
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���
	//m_rot = D3DXVECTOR3(0.0f, 0.0f, -0.35f);			// ����
	m_rot = D3DXVECTOR3(0.0f, 0.03f, -0.15f);			// ����
	m_rotVDest = m_rot;									// �ڕW�̎��_�̌���
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̈ʒu
	m_fDistance = RESULT_LEN;						// ����
	m_fDestDistance = RESULT_LEN;					// �ڕW�̋���
	m_fOriginDistance = RESULT_LEN;					// ���̋���
	m_fDiffHeight = 0.0f;								// �����̍���
	m_fDiffHeightSave = 0.0f;							// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;							// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;							// ���
	m_nCntState = 0;									// ��ԃJ�E���^�[
	m_nCntDistance = 0;									// �����J�E���^�[
	m_nOriginCntDistance = 0;							// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;							// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;					// �����̌����W��
	m_fHeightMaxDest = 0.0f;							// �J�����̍ő卂���̖ڕW

	// �����_�̑��
	m_posR = D3DXVECTOR3(-57.76f, 312.0f, 76.42f);	// �����_(�������ꏊ)

	// ���_�̑��
	m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	m_posVDest = m_posV;								// �ڕW�̎��_
	m_posRDest = m_posR;								// �ڕW�̒����_
}

//==========================================================================
// �����L���O�̃��Z�b�g
//==========================================================================
void CCamera::ResetRanking(void)
{
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����_(�������ꏊ)
	m_posV = D3DXVECTOR3(2401.44f, 1542.82f, -2355.46f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;								// �ڕW�̎��_
	m_posRDest = m_posR;								// �ڕW�̒����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̈ʒu
	m_fDistance = TITLE_LEN_DEST;						// ����
	m_fDestDistance = TITLE_LEN_DEST;					// �ڕW�̋���
	m_fOriginDistance = TITLE_LEN_DEST;					// ���̋���
	m_fDiffHeight = 0.0f;								// �����̍���
	m_fDiffHeightSave = 0.0f;							// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;							// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;							// ���
	m_nCntState = 0;									// ��ԃJ�E���^�[
	m_nCntDistance = 0;									// �����J�E���^�[
	m_nOriginCntDistance = 0;							// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;							// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;					// �����̌����W��
	m_fHeightMaxDest = 0.0f;							// �J�����̍ő卂���̖ڕW

	m_rot = RANKINGROT_NONE;							// ����
	m_rotVDest = m_rot;									// �ڕW�̎��_�̌���

	// �����_�̑��
	m_posR = D3DXVECTOR3(0.0f, 400.0f, 0.0f);			// �����_(�������ꏊ)

	// ���_�̑��
	m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	m_posVDest = m_posV;								// �ڕW�̎��_
	m_posRDest = m_posR;								// �ڕW�̒����_
}

//==========================================================================
// �J�����̈ʒu�擾
//==========================================================================
D3DXVECTOR3 CCamera::GetPositionV(void) const
{
	return m_posV;
}

//==========================================================================
// �J�����̒����_�擾
//==========================================================================
D3DXVECTOR3 CCamera::GetPositionR(void) const
{
	return m_posR;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CCamera::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// �����擾
//==========================================================================
D3DXVECTOR3 CCamera::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
void CCamera::SetDestRotation(const D3DXVECTOR3 rot)
{
	m_rotVDest = rot;
}

//==========================================================================
// �ڕW�̌����擾
//==========================================================================
D3DXVECTOR3 CCamera::GetDestRotation(void)
{
	return m_rotVDest;
}

//==================================================================================
// �ڕW�̈ʒu�ݒ�
//==================================================================================
void CCamera::SetTargetPosition(const D3DXVECTOR3 pos)
{
	// �ڕW�̈ʒu
	m_TargetPos = pos;
}

//==================================================================================
// �ڕW�̈ʒu�擾
//==================================================================================
D3DXVECTOR3 CCamera::GetTargetPosition(void)
{
	return m_TargetPos;
}

//==================================================================================
// �Ǐ]�ڕW�̌����ݒ�
//==================================================================================
void CCamera::SetTargetRotation(const D3DXVECTOR3 rot)
{
	// �ڕW�̌���
	m_TargetRot = rot;
}

//==================================================================================
// �Ǐ]�ڕW�̌����擾
//==================================================================================
D3DXVECTOR3 CCamera::GetTargetRotation(void)
{
	return m_TargetRot;
}

//==========================================================================
// ���ɂȂ�J�����̋����ݒ�
//==========================================================================
void CCamera::SetOriginDistance(float fDistance)
{
	m_fOriginDistance = fDistance;
}

//==========================================================================
// ���ɂȂ�J�����̋����擾
//==========================================================================
float CCamera::GetOriginDistance(void)
{
	return m_fOriginDistance;
}

//==========================================================================
// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��ݒ�
//==========================================================================
void CCamera::SetPlayerChaseIndex(int nIdx)
{
	m_nChasePlayerIndex = nIdx;
}

//==========================================================================
// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CCamera::GetPlayerChaseIndex(void)
{
	return m_nChasePlayerIndex;
}

//==========================================================================
// �Ǐ]�̔���ݒ�
//==========================================================================
void CCamera::SetEnableFollow(bool bFollow)
{
	m_bFollow = bFollow;
}

//==========================================================================
// �Ǐ]��Ԏ擾
//==========================================================================
bool CCamera::IsFollow(void)
{
	return m_bFollow;
}
