//=============================================================================
//
// �f�o�b�O�\������ [debugproc.h]
// Author Ibuki Okusada
//
//=============================================================================

#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_	// ��d�C���N���[�h�h�~

#include "main.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MAX_DEBUGSTRING	(2048)	// �ő啶����

//==========================================================================
// �N���X��`
//==========================================================================
class CDebugProc
{
public:

	CDebugProc();
	~CDebugProc();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Print(const char *fmt, ...);
	static void SetText(void);		// �ݒ菈��

private:

	char m_aStr[MAX_DEBUGSTRING];	// �f�o�b�O�\���p
	bool m_bDisp;					// �f�o�b�O�\�����邩
	static LPD3DXFONT m_pFont;		// �t�H���g�ւ̃|�C���^
};

#endif