//=============================================================================
//
//	3D���f���N���X (model_skin.h)
//	Author : �������l
//	�T�v : 3D���f���������s��
//
//=============================================================================
//*****************************************************************************
//	��d�C���N���[�h�h�~
//*****************************************************************************
#ifndef _MODEL_SKIN_H_
#define _MODEL_SKIN_H_

//*****************************************************************************
//	�C���N���[�h
//*****************************************************************************
#include "object.h"
#include "main.h"

//*****************************************************************************
//	�}�N����`
//*****************************************************************************
#define SKIN_ANIME_SPEED	(60.0f / 4800.0f)	// ���[�V�����X�s�[�h

#define SAFE_RELEASE(p)			if (p) { (p)->Release(); (p) = nullptr; }	// Release�֐��̂���ꍇ�̔j���}�N��
#define SAFE_DELETE(p)			if (p) { delete (p); (p) = nullptr; }		// �z����g�p���Ȃ�delete���g�p����j���}�N��
#define SAFE_DELETE_ARRAY(p)	if (p) { delete[] (p); (p) = nullptr; }		// �z����g�p����delete[]���g�p����j���}�N��

//*****************************************************************************
//	�\���̒�`
//*****************************************************************************
// �h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME : public D3DXFRAME
{
	// �����o�ϐ�
	D3DXMATRIX CombinedTransformationMatrix;
	D3DXMATRIX OffsetMat;	// �I�t�Z�b�g�s��(�C���f�b�N�X�t�`��p)
	DWORD OffsetID;			// �s��e�[�u���̃C���f�b�N�X�ԍ�(�C���f�b�N�X�t�p)
};

// �h�����b�V���R���e�i�[�\����
// (�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	// �R���X�g���N�^
	MYMESHCONTAINER()
	{
		// �����o�ϐ����N���A
		ppBoneMatrix = NULL;		// �S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
		pBoneOffsetMatrices = NULL;	// �t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
	}

	// �����o�ϐ�
	LPDIRECT3DTEXTURE9* ppTextures;		// �e�N�X�`���ւ̃|�C���^
	DWORD dwWeight;						// �d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD dwBoneNum;					// �{�[���̐�
	LPD3DXBUFFER pBoneBuffer;			// �{�[���E�e�[�u��
	D3DXMATRIX** ppBoneMatrix;			// �S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
	D3DXMATRIX* pBoneOffsetMatrices;	// �t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
	LPD3DXMESH pOriMesh;				// �I���W�i�����b�V���p
	DWORD NumPaletteEntries;			// �p���b�g�T�C�Y

	// Work�p
	std::vector<D3DXMATRIX> m_WorkBoneMatArray;

	// �e������t���[���ւ̎Q�Ɣz��B�`�掞�ɂ��̃t���[���̍s����g���B
	std::vector<MYFRAME*> BoneFrameArray;
};

//*****************************************************************************
//	�N���X��`
//*****************************************************************************
// X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
// ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	// �R���X�g���N�^
	MY_HIERARCHY() {}

	STDMETHOD (CreateFrame) (THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD (CreateMeshContainer) (THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*, CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD (DestroyFrame) (THIS_ LPD3DXFRAME);
	STDMETHOD (DestroyMeshContainer) (THIS_ LPD3DXMESHCONTAINER);

private:
};

//=============================================================================
//	3D���f���N���X
//	Author : �������l
//	�T�v : 3D���f���������s���N���X
//=============================================================================
// �X�L�����b�V���N���X
class CSkinMesh
{
public:
	// ���b�V���N���X�̏�����
	// VOID InitBase(SMESH_DATA_FILE* _pSMeshData);
	// ���b�V���̌��݂�Matrix�f�[�^�擾
	D3DXMATRIX GetMatrix();
	CSkinMesh();
	~CSkinMesh() {
		Release();
	}

	// �X�L�����b�V����������
	HRESULT Init(LPDIRECT3DDEVICE9 lpD3DDevice, LPSTR pMeshPass);
	HRESULT AllocateBoneMatrix(LPD3DXFRAME pFrameRoot, LPD3DXMESHCONTAINER pMeshContainerBase);
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME pFrameRoot, LPD3DXFRAME pFrameBase);
	VOID RenderMeshContainer(LPDIRECT3DDEVICE9 lpD3DDevice, MYMESHCONTAINER*, MYFRAME*);
	VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	VOID DrawFrame(PDIRECT3DDEVICE9 lpD3DDevice, LPD3DXFRAME);

	// �t���[�����
	VOID FreeAnim(LPD3DXFRAME pFrame);

	// ��������
	CSkinMesh *Create();

	// �������
	VOID Release();

	// �X�V����
	VOID Update();

	// �`�揈��
	VOID Draw();

	// �I�u�W�F�N�g�̃A�j���[�V�����ύX( ���b�V���I�u�W�F�N�g�̑���p�ԍ�, �ύX����A�j���[�V�����ԍ� )
	VOID ChangeAnim(DWORD NewAnimNum);

	// ���݂̃A�j���[�V�����ԍ��擾
	DWORD GetAnimTrack() { return m_CurrentTrack; }

	// ���݂̃A�j���[�V�����^�C��(�A�j���[�V�����J�n����̎���)���擾
	DWORD GetAnimTime() { return m_AnimeTime; }

	// �A�j���[�V�������x���擾
	FLOAT GetAnimSpeed() { return m_AnimSpeed; }

	// �A�j���[�V�������x��ݒ�
	VOID SetAnimSpeed(FLOAT _AnimSpeed) { m_AnimSpeed = _AnimSpeed; }

private:

	// �Ώۂ̃{�[��������
	MYFRAME* SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame);

public:

	// �{�[���̃}�g���b�N�X�擾 ( �{�[���̖��O )
	D3DXMATRIX GetBoneMatrix(LPSTR _BoneName);

	// �{�[���̃}�g���b�N�X�|�C���^�擾 ( �{�[���̖��O )
	D3DXMATRIX* GetpBoneMatrix(LPSTR _BoneName);

private:

	// �ǉ�
	// ���ׂẴt���[���|�C���^�i�[�����֐�
	VOID CreateFrameArray(LPD3DXFRAME _pFrame);

	// �t���[���Q�Ɨp�z��
	std::vector<MYFRAME*> m_FrameArray;			// �S�t���[���Q�Ɣz��
	std::vector<MYFRAME*> m_IntoMeshFrameArray;	// ���b�V���R���e�i����̃t���[���Q�Ɣz��

	// �{�[�����
	LPD3DXFRAME m_pFrameRoot;

	// �A�j���[�V�����R���g���[��
	LPD3DXANIMATIONCONTROLLER m_pAnimController;

	// �q�G�����L�[�N���X�ϐ�
	MY_HIERARCHY m_cHierarchy;

	// �A�j���[�V�����f�[�^�i�[�p�ϐ�(�����͉ςɕύX�����ق�������)
	LPD3DXANIMATIONSET m_pAnimSet[20];

	// ���݂̃A�j���[�V�������J�n����Ă���̎���(1/60�b)
	DWORD m_AnimeTime;

	// �A�j���[�V�����X�s�[�h
	FLOAT m_AnimSpeed;

	// ���݂̃A�j���[�V�����g���b�N
	DWORD m_CurrentTrack;

	// ���݂̃A�j���[�V�����f�[�^�g���b�N
	D3DXTRACK_DESC m_CurrentTrackDesc;

	// �i�s����
	D3DXMATRIX m_World;

	// ���b�V���̃}�e���A���֌W
	// �}�e���A���ύX�t���O
	BOOL m_MaterialFlg;

	// �}�e���A���f�[�^
	D3DMATERIAL9 m_Material;

	// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxWorld;
};

#endif	// _MODEL_SKIN_H_
