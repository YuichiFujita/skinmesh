//=============================================================================
// 
//  �X�e�[�^�X�E�B���h�E�w�b�_�[ [statuswindow.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STATUSWINDOW_H_
#define _STATUSWINDOW_H_	// ��d�C���N���[�h�h�~

#include "object.h"
#include "gamemanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectCircleGauge2D;
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �X�e�[�^�X�E�B���h�E�N���X��`
class CStatusWindow : public CObject
{
public:

	CStatusWindow(int nPriority = 7);
	~CStatusWindow();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CObjectCircleGauge2D *GetGauge(CGameManager::eStatus status);	// �Q�[�W�擾
	static CStatusWindow *Create(D3DXVECTOR3 pos, bool bJoin);
private:
	
	bool m_bJoin;	// �Q�����Ă邩
	CObject2D *m_apOnOffSign[2];							// �I���I�t�T�C��
	CObject2D *m_apWindowShape[CGameManager::STATUS_MAX];	// �E�B���h�E�̌^
	CObjectCircleGauge2D *m_pCircleGauge2D[CGameManager::STATUS_MAX];	// �~�Q�[�W�̃|�C���^
	CMultiNumber *m_pStatusNumber[CGameManager::STATUS_MAX];			// �X�e�[�^�X�̐���
};



#endif