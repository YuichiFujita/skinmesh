//=============================================================================
// 
//  �w�i�w�b�_�[ [bg.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BG_H_
#define _BG_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D;

//==========================================================================
// �}�N����`
//==========================================================================
#define NUM_BG	(3)	// �w�i�̐�

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CBG : public CObject
{
private:

	// �\���̒�`
	struct SBgInfo
	{
		float fMoveU;				// U�X�N���[���ړ���
		float fMoveV;				// V�X�N���[���ړ���
		float fTexU;				// U�X�N���[���p
		float fTexV;				// V�X�N���[���p
	};

	struct SBg
	{
		CObject2D *pObject2D;		// �w�i�̃I�u�W�F�N�g
		SBgInfo Info;				// ���
	};

public:

	CBG(int nPriority = 0);
	~CBG();

	static HRESULT Load(void);
	static void Unload(void);
	static CBG *Create(void);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);
	void SetVtx(int nCntBg);

	void SetMoveTex(const D3DXVECTOR3 move);

private:
	static const char *m_apTextureFile[];			// �e�N�X�`���̃t�@�C��
	SBg m_aBg[NUM_BG];								// �w�i�̏��
	static const SBgInfo m_aBgInfo[];				// �w�i�̏��̏��
	int m_nTexIdx[NUM_BG];				// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif