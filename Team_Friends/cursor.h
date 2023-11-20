//=============================================================================
// 
// �J�[�\������ [cursor.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CURSOR_H_
#define _CURSOR_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�[�\���N���X��`
class CCursor : public CObject2D
{
public:

	CCursor(int nPriority = 8);
	~CCursor();

	static CCursor *Create(const D3DXVECTOR3 pos, int nIdx);
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	void SetEnbaleDicide(bool bDecide) { m_bDecide = bDecide; }	// �����Ԑݒ�
	bool IsDecide(void) { return m_bDecide; }					// �����Ԏ擾
	void SetSelectIdx(int nIdx) { m_nSelectIdx = nIdx; }		// �I�����̔ԍ��ݒ�
	int GetSelectIdx(void) { return m_nSelectIdx; }				// �I�����̔ԍ��擾

private:

	int m_nMyPlayerIdx;		// �v���C���[�C���f�b�N�X�ԍ�
	int m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nSelectIdx;		// �I�����̃C���f�b�N�X�ԍ�
	bool m_bDecide;			// ���܂������ǂ���
	static const char *m_apTextureFile[mylib_const::MAX_PLAYER];	// �e�N�X�`���̃t�@�C��
};

#endif