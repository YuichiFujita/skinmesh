//=============================================================================
// 
// �X�e�[�^�X�E�B���h�E�̉��n���� [statuswindow_base.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STATUSWINDOW_BASE_H_
#define _STATUSWINDOW_BASE_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �X�e�[�^�X�E�B���h�E�̉��n�N���X��`
class CStatusWindow_Base : public CObject2D
{
public:

	CStatusWindow_Base(int nPriority = 6);
	~CStatusWindow_Base();

	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStatusWindow_Base *Create(const D3DXVECTOR3 pos);

private:

};

#endif