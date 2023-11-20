//==========================================================================
//
// �T�E���h���� [sound.cpp]
// Author : ���n�Ή�
//
//==========================================================================
#include "sound.h"

//==========================================================================
//�}�N����`
//==========================================================================
#define START_VOLUME	(1.0f)		//���ʕ␳�̏����l
#define MAX_VOLUME		(1.0f)		//���ʕ␳�̍ő�l
#define MIN_VOLUME		(0.0f)		//���ʕ␳�̍ŏ��l

//==========================================================================
// �v���g�^�C�v�錾
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CSound::SOUNDINFO CSound::m_aSoundInfo[LABEL_MAX] = 
{
	{ "data/BGM/title.wav", -1 },			// �^�C�g��
	{ "data/BGM/gameBGM.wav", -1 },			// �Q�[����
	{ "data/BGM/bossBGM.wav", -1 },			// �Q�[����
	{ "data/BGM/result.wav", -1 },			// �^�C�g��
	{ "data/SE/explosion000moto.wav", 0 },		// ����
	{ "data/SE/game_clear.wav", 0 },			// �L���s�[��
	{ "data/SE/walk_glass.wav", 0 },			// ���s
	{ "data/SE/jump.wav", 0 },			// �W�����v
	{ "data/SE/field_01.wav", 0 },			// �n��
	{ "data/SE/swing_01.wav", 0 },			// �X�C���O
	{ "data/SE/impact_01.wav", 0 },			// �Ռ�
	{ "data/SE/kill_01.wav", 0 },			// �_���[�W
	{ "data/SE/comboin_01.wav", 0 },			// �_���[�W
	{ "data/SE/tutorialwindow.wav", 0 },			// �_���[�W
	{ "data/SE/spawn.wav", 0 },			// �_���[�W
	{ "data/SE/spawnemey.wav", 0 },			// �_���[�W
	{ "data/SE/playerdmg_01.wav", 0 },			// �_���[�W
	{ "data/SE/playerdmg_big.wav", 0 },			// �_���[�W
	{ "data/SE/enemydmg_01.wav", 0 },			// �_���[�W
	{ "data/SE/enemydmg_02.wav", 0 },			// �_���[�W
	{ "data/SE/enemydmg_03.wav", 0 },			// �_���[�W
	{ "data/SE/combokill.wav", 0 },			// �_���[�W
	{ "data/SE/confusion.wav", -1 },			// �_���[�W
	{ "data/SE/number.wav", 0 },			// �_���[�W
	{ "data/SE/numbermove.wav", 0 },			// �_���[�W
	{ "data/SE/cursor.wav", 0 },			// �_���[�W
	{ "data/SE/done.wav", 0 },			// �_���[�W
	{ "data/SE/tutorial_clear.wav", 0 },			// �_���[�W
	{ "data/SE/slashHit.wav", 0 },			// �a���q�b�g
	{ "data/SE/bounce.wav", 0 },			// ���˕Ԃ�
	{ "data/SE/bounceHit.wav", 0 },		// ���˕Ԃ��q�b�g
	{ "data/SE/enemy_explosion.wav", 0 },		// �G����
	{ "data/SE/titleslash.wav", 0 },		// �^�C�g���a��
	{ "data/SE/hobaring.wav", -1 },		// �z�o�����O
	{ "data/SE/assult.wav", 0 },		// �ːi
	{ "data/SE/assultend.wav", 0 },		// �ːi�I��
	{ "data/SE/stun.wav", -1 },			// �X�^��
	{ "data/SE/stunheal.wav", 0 },		// �X�^����
	{ "data/SE/boss_bullet.wav", 0 },	// �{�X�̒e�U��
	{ "data/SE/boss_bulletcharge.wav", 0 },	// �{�X�̒e�`���[�W
	{ "data/SE/boss_talk.wav", 0 },		// �{�X�g�[�N
	{ "data/SE/ranking_00.wav", 0 },		// �{�X�g�[�N

};	// �T�E���h�̏��

//==========================================================================
// �}�N����`
//==========================================================================
CSound::CSound()
{
	
}

//==========================================================================
// �}�N����`
//==========================================================================
CSound::~CSound()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = m_aSoundInfo[nCntSound].nCntLoop;
		SetVolume.dwFlags = DSBCAPS_CTRLVOLUME;		//���ʒ����̃t���O

		m_fVolume = START_VOLUME;

		//���ʂ��Z�b�g����
		m_pMasteringVoice->SetVolume(START_VOLUME);

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSound::Uninit(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if(m_pXAudio2 != NULL)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//==========================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//==========================================================================
HRESULT CSound::PlaySound(LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_aSoundInfo[label].nCntLoop;
	//SetVolume.dwFlags = DSBCAPS_CTRLVOLUME;		//���ʒ����̃t���O

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//==========================================================================
// �Z�O�����g��~(���x���w��)
//==========================================================================
void CSound::StopSound(LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);

	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//==========================================================================
// �Z�O�����g��~(�S��)
//==========================================================================
void CSound::StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//==========================================================================
// �`�����N�̃`�F�b�N
//==========================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//==========================================================================
// �`�����N�f�[�^�̓ǂݍ���
//==========================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//==========================================================================
// ���ʒ���(0.0�`1.0�܂�)
//==========================================================================
void CSound::VolumeChange(float fVolume)
{

	m_fVolume += fVolume;

	if (m_fVolume >= MAX_VOLUME)
	{
		m_fVolume = MAX_VOLUME;
	}

	if (m_fVolume <= MIN_VOLUME)
	{
		m_fVolume = MIN_VOLUME;
	}

	//���ʂ��Z�b�g����
	m_pMasteringVoice->SetVolume(m_fVolume);
}

//==========================================================================
// ���ʎ擾(3���\��)
//==========================================================================
int CSound::GetVolume(void)
{
	return (int)((m_fVolume + 0.009) * 100);
}
