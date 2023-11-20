//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �T�E���h�N���X
class CSound
{
public:

	// �񋓌^��`
	typedef enum
	{
		LABEL_BGM_TITLE = 0,	// �^�C�g��
		LABEL_BGM_GAME,		// �Q�[��
		LABEL_BGM_BOSSGAME,		// �{�X
		LABEL_BGM_RESULT,	// ���U���g
		LABEL_SE_EXPLOSION,	// ����
		LABEL_SE_KYUPIIN,	// �L���s�[��
		LABEL_SE_WALK,		// ���s
		LABEL_SE_JUMP,		// �W�����v
		LABEL_SE_FIELD,		// �n��
		LABEL_SE_SWING,		// �X�C���O
		LABEL_SE_IMPACT01,	// �Ռ�
		LABEL_SE_DMG01,		// �_���[�W
		LABEL_SE_COMBOIN,		// �_���[�W
		LABEL_SE_TUTORIALWINDOW,		// �_���[�W
		LABEL_SE_SPAWN,		// �_���[�W
		LABEL_SE_SPAWNEN,		// �_���[�W
		LABEL_SE_PLAYERDMG,		// �_���[�W
		LABEL_SE_PLAYERDMG_BIG,		// �_���[�W
		LABEL_SE_ENEMYDMG_SMALL,		// �_���[�W
		LABEL_SE_ENEMYDMG_NORMAL,		// �_���[�W
		LABEL_SE_ENEMYDMG_BIG,		// �_���[�W
		LABEL_SE_COMBOKILL,		// �_���[�W
		LABEL_SE_CONFUSION,		// �_���[�W
		LABEL_SE_NUMBER,		// �_���[�W
		LABEL_SE_NUMBERMOVE,		// �_���[�W
		LABEL_SE_CURSOR,		// �J�[�\���ړ�
		LABEL_SE_DONE,		// �J�[�\���ړ�
		LABEL_SE_STEPCLEAR,		// �J�[�\���ړ�
		LABEL_SE_SLASHHIT,		// �a���q�b�g
		LABEL_SE_BOUNCE,		// ���˕Ԃ�
		LABEL_SE_BOUNCEHIT,		// ���˕Ԃ��q�b�g
		LABEL_SE_ENEMYEXPLOSION,		// �G����
		LABEL_SE_TITLESLASH,		// �^�C�g���a��
		LABEL_BGM_HOBARING,		// �z�o�����O
		LABEL_SE_ASSULT,		// �ːi
		LABEL_SE_ASSULTEND,		// �ːi�I��
		LABEL_SE_STUN,			// �X�^��
		LABEL_SE_STUNHEAL,		// �X�^����
		LABEL_SE_BOSSBULLET,	// �{�X�̒e�U��
		LABEL_SE_BOSSBULLET_CHARGE,	// �{�X�̒e�`���[�W
		LABEL_SE_BOSSTALK,		// �{�X�g�[�N
		LABEL_SE_RANKINGSCORE,		// �����L���O�X�R�A
		LABEL_MAX,
	} LABEL;

	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(LABEL label);
	void StopSound(void);
	void StopSound(LABEL label);
	void VolumeChange(float fVolume);
	int GetVolume(void);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	DSBUFFERDESC SetVolume;								// ���ʒ���
	float m_fVolume;									// ����
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];			// �T�E���h�̏��
};


#endif
