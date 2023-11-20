//=============================================================================
// 
//  �a���q�b�g�G�t�F�N�g�w�b�_�[ [effect_slashhit.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EFFECT_SLASHHIT_H_
#define _EFFECT_SLASHHIT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �a���q�b�g�G�t�F�N�g�N���X��`
class CEffectSlashHit : public CObjectBillboard
{
public:

	CEffectSlashHit(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CEffectSlashHit();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	static int GetNumAll(void);
	static CEffectSlashHit *Create(const D3DXVECTOR3 pos);	// ��������
	static void LoadTexture(void);	// �e�N�X�`���ǂݍ���
private:

	// �����o�ϐ�
	int m_nCntAnim;			// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;		// �A�j���[�V�����p�^�[��No.
	static int m_nNumAll;	// �����̑���
	static int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif