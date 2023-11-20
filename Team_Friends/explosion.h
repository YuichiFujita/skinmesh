//=============================================================================
// 
//  �����w�b�_�[ [explosion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����N���X��`
class CExplosion : public CObjectBillboard
{
public:

	CExplosion(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CExplosion();

	static HRESULT Load(void);
	static void Unload(void);
	static CExplosion *Create();
	static CExplosion *Create(const D3DXVECTOR3 pos);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	static int GetNumAll(void);

private:
	int m_nCntAnim;							// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;						// �A�j���[�V�����p�^�[��No.
	static int m_nNumAll;					// �����̑���
	int m_nTexIdx;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif