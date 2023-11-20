//=============================================================================
// 
//  �X�R�A�w�b�_�[ [score.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "resultmanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CScore
{
public:

	CScore(int nPriority = 6);
	~CScore();

	static CScore *Create(void);
	static CScore *Create(D3DXVECTOR3 pos);
	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) const;

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Add(int nValue);

private:
	
	void SetValue(void);

	int m_nNum;								// �l
	CMultiNumber *m_apNumber;			// �����̃I�u�W�F�N�g
	int m_nTexIdx;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif