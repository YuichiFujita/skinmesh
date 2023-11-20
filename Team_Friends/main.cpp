//==========================================================================
// 
// C++DirectX[main.cpp]
// Author:���n�Ή�
// 
//==========================================================================
#include <stdio.h>
#include "main.h"
#include "manager.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define CLASS_NAME		"WindowsClass"	// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"EcoRumble"	// �E�C���h�E�̖��O(�L���v�V����)

//==========================================================================
// �v���g�^�C�v�錾
//==========================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//==========================================================================
// �O���[�o���ϐ��錾
//==========================================================================
int g_nCountFPS;

//==========================================================================
// ���C���֐�
//==========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmbLine, int nCmbShow)
{
	// ���������[�N���o��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASS�̃������T�C�Y
		CS_CLASSDC,							// �E�C���h�E�̃X�^�C��
		WindowProc,							// �E�C���h�E�v���V�[�W��
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							// �C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),		// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			// �N���C�A���g�̈�̔w�i�F
		NULL,								// ���j���[�o�[
		CLASS_NAME,							// �E�C���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION)		// �t�@�C���̃A�C�R��
	};

	HWND hWnd;	// �E�C���h�E�n���h��(���ʎq)
	MSG msg;	// ���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// ��ʃT�C�Y�̍\����

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�̐���
	hWnd = CreateWindowEx(
		0,									// �g���E�C���h�E�X�^�C��
		CLASS_NAME,							// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,						// �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,				// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,						// �E�C���h�E�̍���X���W
		CW_USEDEFAULT,						// �E�C���h�E�̍���Y���W
		(rect.right - rect.left),			// �E�C���h�E�̕�
		(rect.bottom - rect.top),			// �E�C���h�E�̍���
		NULL,								// �e�E�C���h�E�̃n���h��
		NULL,								// ���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,							// �C���X�^���X�n���h��
		NULL								// �E�C���h�E�쐬�f�[�^
	);

	DWORD dwCurrentTime;					// ���ݎ���
	DWORD dwExecLastTime;					// �Ō�ɏ�����������
	DWORD dwFrameCount;						// �t���[���J�E���g
	DWORD dwFPSLastTime;					// �Ō��FPS���v����������


	// �}�l�[�W���̐���
	CManager *pManager = NULL;

	// �}�l�[�W���̐���
	if (pManager == NULL)
	{// NULL��������

		// �������m��
		pManager = CManager::Create();
	}

	if (pManager != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
		{// ���������������s�����ꍇ

			return -1;
		}
	}

	g_nCountFPS = 0;

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;				// ����������
	dwExecLastTime = timeGetTime();	// ���ݎ������擾(�ۑ�)

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmbShow);		// �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				// �N���C�A���g�̈���X�V

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ���b�Z�[�W���[�v
	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Window�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);		// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{// DirectX�̏���

			dwCurrentTime = timeGetTime();	// ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60����1�b�o��

				// �����J�n�̎���[���ݎ���]��ۑ�
				dwExecLastTime = dwCurrentTime;

				if (pManager != NULL)
				{// �������̊m�ۂ��o���Ă�����

					// �X�V����
					pManager->Update();

					// �`�揈��
					pManager->Draw();
				}

				// �t���[���J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	if (pManager != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		pManager->Uninit();

		// �������̊J��
		delete pManager;
		pManager = NULL;
	}
	

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
		
	return (int)msg.wParam;
}

//==========================================================================
// �E�C���h�E�v���V�[�W��
//==========================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	// �Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:				// �E�C���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				// ESC�L�[����

			// ���b�Z�[�W�{�b�N�X��\��
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				// �E�C���h�E��j������(VK_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:				// �o�c��
		// ���b�Z�[�W�{�b�N�X��\��
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			// �E�C���h�E��j������(VK_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;			// 0��Ԃ��ďI����h��
		}
		break;

	case WM_LBUTTONDOWN:		// �}�E�X���N���b�N
		// �ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd);
		break;

	case WM_TIMER:				// �^�C�}�[���s�̃��b�Z�[�W
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ����̏�����Ԃ�
}

//==========================================================================
// FPS�擾
//==========================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
