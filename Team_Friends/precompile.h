//=============================================================================
// 
//  �v���R���t�@�C���w�b�_�[ [precompile.h]
//  Author : ���n�Ή끕���搶
// 
//=============================================================================
#ifndef _PRECOMPIRE_H_
#define _PRECOMPIRE_H_	// ��d�C���N���[�h�h�~

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <vector>
#include <algorithm>

#include <d3dx9.h>						// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION	(0x0800)	// �r���h���̌x���p�}�N��
#include <dinput.h>						// ���͏����ɕK�v
#include <xaudio2.h>					// �T�E���h�����ɕK�v
#include <dsound.h>						// �T�E���h���������ɕK�v
#include <Xinput.h>

// ���������[�N�o�͗p�}�N��
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// ���������[�N���o�ɕK�v

// crtdbg.h�̃o�O�΍��p�}�N��
#if _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else

#define DEBUG_NEW new
#endif

#endif	// _PRECOMPIRE_H_