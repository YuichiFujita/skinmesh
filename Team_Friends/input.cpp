//=============================================================================
//
// ���͏��� [input.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"
#include "debugproc.h"
#include "manager.h"

//�}�N����`
#define SHOT_FPS	(15)	// �e�̊Ԋu
#define DMG_TIME	(30)	// �o�C�u�̎���
#define WIN_TIME	(15)	// �o�C�u�̎���
#define MOUSE_SENS	(0.5f)	// �}�E�X���x�̕␳

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;		// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
int CInput::nNumAll = 0;					// �f�o�C�X�̑���

//==========================================================================
//�O���[�o���ϐ��錾
//==========================================================================

//==================================================================================
// �R���X�g���N�^
//==================================================================================
CInput::CInput()
{
	// �������Z
	nNumAll++;
}

//==================================================================================
// �f�X�g���N�^
//==================================================================================
CInput::~CInput()
{
	// �������Z
	nNumAll--;
}

//==================================================================================
// ����������
//==================================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (m_pInput == NULL)
	{// �܂��������Ă��Ȃ�������

		// DirectInput�I�u�W�F�N�g�̐���
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==================================================================================
// �I������
//==================================================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// �A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if (nNumAll <= 1)
	{// �Ō�̃f�o�C�X�̎�

		// �I�u�W�F�N�g�̔j��
		if (m_pInput != NULL)
		{
			m_pInput->Release();
			m_pInput = NULL;
		}
	}
}

//--------------------------------------------------- [ ��������L�[�{�[�h ] ---------------------------------------------------//

//==================================================================================
// �R���X�g���N�^
//==================================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;		// �v���X���
		m_aKeyStateTrigger[nCntKey] = NULL;	// �g���K�[���
		m_aKeyStateRelease[nCntKey] = NULL;	// �����[�X���
		m_aKeyStateRepeat[nCntKey] = NULL;	// ���s�[�g���
		m_nCntKeyRepeat[nCntKey] = 0;		// ���s�[�g�p�J�E���g
		m_nPatternRepeat[nCntKey] = 1;		// ���s�\�g�̊Ԋu
	}
}

//==================================================================================
// �f�X�g���N�^
//==================================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//==================================================================================
// ����������
//==================================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ����������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//==================================================================================
// �I������
//==================================================================================
void CInputKeyboard::Uninit(void)
{
	// �I������
	CInput::Uninit();
}

//==================================================================================
// �X�V����
//==================================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// �g���K�[����ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// �����[�X����ۑ�
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];

			// �L�[�{�[�h�̃v���X����ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];

			if (GetPress(nCntKey))
			{
				// ���s�[�g�J�E���g���Z
				m_nCntKeyRepeat[nCntKey] = (m_nCntKeyRepeat[nCntKey] + 1) % m_nPatternRepeat[nCntKey];

				if ((m_nCntKeyRepeat[nCntKey] % m_nPatternRepeat[nCntKey]) == 0)
				{
					// �����[�X����ۑ�
					m_nCntKeyRepeat[nCntKey] = (m_nCntKeyRepeat[nCntKey] + 1) % m_nPatternRepeat[nCntKey];

					// �v���X����ۑ�
					m_aKeyStateRepeat[nCntKey] = m_aKeyState[nCntKey];
				}
				else
				{
					// ���s�[�g�Ƀ����[�X�̏��ۑ�
					m_aKeyStateRepeat[nCntKey] = m_aKeyStateRelease[nCntKey];
				}
			}
			else
			{
				// ���s�[�g�Ƀ����[�X�̏��ۑ�
				m_aKeyStateRepeat[nCntKey] = m_aKeyStateRelease[nCntKey];
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//==================================================================================
// �L�[�{�[�h�̃v���X�����擾
//==================================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true: false;
}

//==================================================================================
// �L�[�{�[�h�̃g���K�[�����擾
//==================================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==================================================================================
// �L�[�{�[�h�̃����[�X�����擾
//==================================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==================================================================================
// �L�[�{�[�h�̃��s�[�g�����擾
//==================================================================================
bool CInputKeyboard::GetRepeat(int nKey, int nPattern)
{
	m_nPatternRepeat[nKey] = nPattern;		// ���s�\�g�̊Ԋu
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------- [ ��������Q�[���p�b�h ] ---------------------------------------------------//
//==================================================================================
// �R���X�g���N�^
//==================================================================================
CInputGamepad::CInputGamepad()
{
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;	// �U���̎��
		m_nCntVibration[nCntPlayer] = 0;						// �U���̎���
		m_nMaxCntVibration[nCntPlayer] = 0;						// �U���̎���
	}
	m_nLeftStickCount = 0;					// ���g���K�[�̑I���J�E���g
	m_bLeftStickSelect[STICK_X] = false;				// ���g���K�[�̑I�𔻒�
	m_bLeftStickSelect[STICK_Y] = false;				// ���g���K�[�̑I�𔻒�
	m_bVibrationUse = false;				// �o�C�u���g�p���邩�ǂ���

	m_nCntPadrepeat = 0;									// ���s�[�g�p�J�E���g
}

//==================================================================================
// �f�X�g���N�^
//==================================================================================
CInputGamepad::~CInputGamepad()
{

}

//==================================================================================
//�Q�[���p�b�h�̏���������
//==================================================================================
HRESULT CInputGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{

	//XInput��L����
	XInputEnable(true);

	// ���X�e�B�b�N�̏���������
	m_bLeftStickSelect[STICK_X] = false;				// ���g���K�[�̑I�𔻒�
	m_bLeftStickSelect[STICK_Y] = false;				// ���g���K�[�̑I�𔻒�
	m_nLeftStickCount = 0;

	// �o�C�u�̏���������
	m_bVibrationUse = true;

	// �������N���A
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		m_nCntVibration[nCntPlayer] = 0;
		m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;
		m_nMaxCntVibration[nCntPlayer] = 0;

		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
	}

	return S_OK;
}

//==================================================================================
//�Q�[���p�b�h�̏I������
//==================================================================================
void CInputGamepad::Uninit(void)
{
	//XInput�𖳌���
	XInputEnable(false);
}

//==================================================================================
//�Q�[���p�b�h�̍X�V����
//==================================================================================
void CInputGamepad::Update(void)
{
	XINPUT_STATE  aGamepadState[mylib_const::MAX_PLAYER];
	int nCntPlayer;

	// ���̓f�o�C�X����f�[�^���擾
	for (nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		if (XInputGetState(nCntPlayer, &aGamepadState[nCntPlayer]) == ERROR_SUCCESS)
		{
			// �L�[�{�[�h�̃g���K�[����ۑ�
			m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons =
				(~m_aGamepadState[nCntPlayer].Gamepad.wButtons) & aGamepadState[nCntPlayer].Gamepad.wButtons;

			// �����[�X����ۑ�
			m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons = 
				(m_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & m_aGamepadState[nCntPlayer].Gamepad.wButtons;


			// �L�[�{�[�h�̃v���X����ۑ�
			m_aGamepadState[nCntPlayer] = aGamepadState[nCntPlayer];
			
			if ((m_nCntPadrepeat % SHOT_FPS) == 0)
			{
				// ���s�[�g����ۑ�
				m_nCntPadrepeat = (m_nCntPadrepeat + 1) % SHOT_FPS;
				m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons = aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
			else
			{
				// ���s�[�g�Ƀ����[�X�̏��ۑ�
				m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons =
					(m_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & m_aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
		}

		//�^�C�}�[�����Z
		m_nCntVibration[nCntPlayer]--;

		if (m_nCntVibration[nCntPlayer] < 0)
		{// �J�E���g���[���ȉ�

			// �������ĂȂ���Ԃɐݒ�
			m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;

			// ������������
			memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		}
		
		if (m_VibrationState[nCntPlayer] != VIBRATION_STATE_NONE)
		{// �_���[�W�̎�

			if (m_VibrationState[nCntPlayer] == VIBRATION_STATE_ITEM)
			{// �A�C�e���̎��͑����Ă���

				m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed += (USHRT_MAX / m_nMaxCntVibration[nCntPlayer]);
				m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed += (USHRT_MAX / m_nMaxCntVibration[nCntPlayer]);
			}
			else
			{
				m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed -= (USHRT_MAX / m_nMaxCntVibration[nCntPlayer]);
				m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed -= (USHRT_MAX / m_nMaxCntVibration[nCntPlayer]);
			}

			if (m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed <= 0)
			{// �X�s�[�h��0�ȉ�

				m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = 0;
			}
			else if (m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed <= 0)
			{// �X�s�[�h��0�ȉ�

				m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = 0;
			}
		}


		// �R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
		XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
	}

	
	if (GetStickMoveL(0).y == 0)
	{// �X�e�B�b�N�����Ƃɖ߂��Ă���Ƃ�

		// �J�E���g��������
		m_nLeftStickCount = 0;

		// �����OFF
		m_bLeftStickSelect[STICK_Y] = false;
	}
	if (GetStickMoveL(0).x == 0)
	{// �X�e�B�b�N�����Ƃɖ߂��Ă���Ƃ�

		// �J�E���g��������
		m_nLeftStickCount = 0;

		// �����OFF
		m_bLeftStickSelect[STICK_X] = false;
	}

}

//==================================================================================
//�o�C�u�̐ݒ菈��
//==================================================================================
void CInputGamepad::SetVibration(VIBRATION_STATE VibState, int nCntPlayer)
{
	if (m_bVibrationUse == true)
	{

		// ��Ԃ���
		m_VibrationState[nCntPlayer] = VibState;

		switch (VibState)
		{
		case VIBRATION_STATE_DMG:

			m_nCntVibration[nCntPlayer] = 15;
			m_nMaxCntVibration[nCntPlayer] = m_nCntVibration[nCntPlayer];
			m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = USHRT_MAX * (short)0.8f;
			m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = USHRT_MAX * (short)0.8f;
			break;

		case VIBRATION_STATE_ENEMYHIT:

			m_nCntVibration[nCntPlayer] = 10;
			m_nMaxCntVibration[nCntPlayer] = m_nCntVibration[nCntPlayer];
			m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = USHRT_MAX * (short)0.6f;
			m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = USHRT_MAX * (short)0.6f;
			break;

		case VIBRATION_STATE_ITEM:
			m_nCntVibration[nCntPlayer] = 100;
			m_nMaxCntVibration[nCntPlayer] = m_nCntVibration[nCntPlayer];
			m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = (USHRT_MAX * (short)0.00001f);
			m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = (USHRT_MAX * (short)0.00001f);
			break;
		}

		// �R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
		XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);

	}
}

//==================================================================================
// �Q�[���p�b�h�̃o�C�u�ݒ菈��
//==================================================================================
void CInputGamepad::SetEnableVibration(void)
{
	//�؂�ւ�
	m_bVibrationUse ^= true;
}

//==================================================================================
// �Q�[���p�b�h�̃o�C�u�ݒ菈��
//==================================================================================
bool CInputGamepad::GetEnableVibration(void)
{
	return m_bVibrationUse;
}

//==================================================================================
// �Q�[���p�b�h�̃v���X����
//==================================================================================
bool CInputGamepad::GetPress(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadState[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
// �Q�[���p�b�h�̃g���K�[����
//==================================================================================
bool CInputGamepad::GetTrigger(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
// �Q�[���p�b�h�̃����[�X����
//==================================================================================
bool CInputGamepad::GetRelease(int nKey, int nCntPlayer)
{
	return (m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
// �Q�[���p�b�h�̃��s�[�g����
//==================================================================================
bool CInputGamepad::GetRepeat(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
// �Q�[���p�b�h�̃v���X����,LT
//==================================================================================
bool CInputGamepad::GetPressLTrigger(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadState[nCntPlayer].Gamepad.bLeftTrigger & (0x01 << nKey)) ? true : false;
}

//==================================================================================
// �Q�[���p�b�h�̃v���X����,RT
//==================================================================================
bool CInputGamepad::GetPressRTrigger(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadState[nCntPlayer].Gamepad.bRightTrigger & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//  L�X�e�B�b�N�̈ړ��ʎ擾
//==================================================================================
D3DXVECTOR3 CInputGamepad::GetStickMoveL(int nCntPlayer)
{

	// L�X�e�B�b�N�̈ړ���
	D3DXVECTOR3 StickMoveL = 
		D3DXVECTOR3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX * 0.001f,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY * 0.001f,
			0.0f);

	return StickMoveL;
}

//==================================================================================
//  R�X�e�B�b�N�̈ړ��ʎ擾
//==================================================================================
D3DXVECTOR3 CInputGamepad::GetStickMoveR(int nCntPlayer)
{

	// R�X�e�B�b�N�̈ړ���
	D3DXVECTOR3 StickMoveR =
		D3DXVECTOR3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX * 0.001f,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY * 0.001f,
			0.0f);

	return StickMoveR;
}


//==================================================================================
//  L�X�e�B�b�N�̊����擾
//==================================================================================
D3DXVECTOR3 CInputGamepad::GetStickPositionRatioL(int nCntPlayer)
{
	// L�X�e�B�b�N�̊���
	D3DXVECTOR3 StickRatio = 
		D3DXVECTOR3(
		(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX / (float)SHRT_MAX,
		(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY / (float)SHRT_MAX,
		0.0f);

	return StickRatio;
}

//==================================================================================
//  R�X�e�B�b�N�̊����擾
//==================================================================================
D3DXVECTOR3 CInputGamepad::GetStickPositionRatioR(int nCntPlayer)
{
	// L�X�e�B�b�N�̊���
	D3DXVECTOR3 StickRatio =
		D3DXVECTOR3(
		(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX / (float)SHRT_MAX,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY / (float)SHRT_MAX,
			0.0f);

	return StickRatio;
}

//==================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N�̌����擾
//==================================================================================
float CInputGamepad::GetStickRotL(int nCntPlayer)
{
	// �p�x�����߂�
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY - 0.0f));

	return fRot;
}

//==================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N�̌����擾
//==================================================================================
float CInputGamepad::GetStickRotR(int nCntPlayer)
{
	// �p�x�����߂�
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY - 0.0f));

	return fRot;
}

//==================================================================================
// �Q�[���p�b�h��L�g���K�[
//==================================================================================
BYTE CInputGamepad::GetLeftTriggerPress(int nPlayer)
{
	return m_aGamepadState[nPlayer].Gamepad.bLeftTrigger;
}

//==================================================================================
// �Q�[���p�b�h��R�g���K�[
//==================================================================================
BYTE CInputGamepad::GetRightTriggerPress(int nPlayer)
{
	return m_aGamepadState[nPlayer].Gamepad.bRightTrigger;
}

//==================================================================================
// ���X�e�B�b�N�̔�����擾
//==================================================================================
bool CInputGamepad::GetStickSelect(int nXY)
{
	return m_bLeftStickSelect[nXY];
}

//==================================================================================
// ���X�e�B�b�N�̔���
//==================================================================================
void CInputGamepad::SetEnableStickSelect(bool bStick, int nXY)
{
	m_bLeftStickSelect[nXY] = bStick;
}

//==================================================================================
// �p�b�h�̃��s�[�gFPS
//==================================================================================
int CInputGamepad::GetnCntPad(void)
{
	return m_nCntPadrepeat;
}

#if 1

//==================================================================================
// �R���X�g���N�^
//==================================================================================
CInputMouse::CInputMouse()
{
	memset(&m_MouseState, 0, sizeof(DIMOUSESTATE2));
	memset(&m_MouseStateTrigger, 0, sizeof(XINPUT_STATE));
	memset(&m_MouseStateRerease, 0, sizeof(XINPUT_STATE));
	memset(&m_MouseStateRepeat, 0, sizeof(XINPUT_STATE));

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		aOldState[nCnt] = NULL;	// �O��̓��͏��
	}
}

//==================================================================================
// �f�X�g���N�^
//==================================================================================
CInputMouse::~CInputMouse()
{

}

//==========================================
//  �}�E�X�̏�����
//==========================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ����������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�̐ݒ�
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�(��Βl��DIPROPAXISMODE_ABS)

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// �f�o�C�X�̐ݒ�Ɏ��s
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����擾
	m_pDevice->Acquire();

	// �}�E�X�J�[�\���̏���
	//ShowCursor(false);

	return S_OK;
}

//==========================================
// �}�E�X�̏I������
//==========================================
void CInputMouse::Uninit(void)
{
	// �I������
	CInput::Uninit();
}

//==========================================
// �}�E�X�̍X�V����
//==========================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouse; // �}�E�X�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		//���͏��̕ۑ�
		m_MouseState = mouse;
	}
	else
	{
		// �}�E�X�ւ̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//==========================================
// �}�E�X�̃v���X���
//==========================================
bool CInputMouse::GetPress(BUTTON nKey)
{
	return (m_MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================
// �}�E�X�̈ړ���
//==========================================
D3DXVECTOR3 CInputMouse::GetMouseMove(void)
{
	return D3DXVECTOR3(((float)m_MouseState.lX) * MOUSE_SENS, (-(float)m_MouseState.lY) * MOUSE_SENS, (-(float)m_MouseState.lZ) * MOUSE_SENS);
}

#endif

