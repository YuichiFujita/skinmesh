//=============================================================================
// 
//  ���̃����O�G�t�F�N�g�w�b�_�[ [effect_thunderring.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EFFECT_THUNDERRING_H_
#define _EFFECT_THUNDERRING_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���̃����O�G�t�F�N�g�N���X��`
class CThunderRing : public CObjectBillboard
{
public:

	CThunderRing(int nPriority = 8);
	~CThunderRing();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	void SetDesableAutoDeath(void) { m_bAutoDeath = false; }	// �����폜�̔���폜
	bool IsFinish(void) { return m_bFinish; }					// �I���̔���
	static int GetNumAll(void);
	static CThunderRing *Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size);	// ��������
	static void LoadTexture(void);	// �e�N�X�`���ǂݍ���
private:

	// �����o�ϐ�
	int m_nCntAnim;			// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;		// �A�j���[�V�����p�^�[��No.
	bool m_bAutoDeath;		// �����폜�̃t���O
	bool m_bFinish;			// �A�j���[�V�������I���������
	static int m_nNumAll;	// �����̑���
	static int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif