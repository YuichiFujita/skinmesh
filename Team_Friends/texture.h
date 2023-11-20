//=============================================================================
// 
// �e�N�X�`���w�b�_�[ [texture.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "constans.h"

class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
//�e�N�X�`���N���X��`
class CTexture
{
public:

	CTexture();
	~CTexture();

private:
	struct STexture
	{
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���̃|�C���^
		D3DXIMAGE_INFO imageInfo;		// �e�N�X�`���f�ޏ��
		char acFilename[mylib_const::MAX_STRING];	// �t�@�C����
		int nFileNameLen;				// �t�@�C�����̕�����
	};

public:

	static HRESULT LoadAll(void);

	HRESULT Load(void);
	void Unload(void);
	int Regist(const char *pFileName);
	LPDIRECT3DTEXTURE9 GetAdress(int nIdx);
	D3DXVECTOR2 GetImageSize(int nIdx);		// �e�N�X�`���f�ނ̃T�C�Y�擾

	static int GetNumAll(void);		// �ǂݍ��񂾑���
	STexture GetTextureInfo(const char *pFileName);	// �e�N�X�`�����擾
	STexture GetTextureInfo(int nIdxTex);	// �e�N�X�`�����擾


private:
	HRESULT LoadTex(const char *pFileName);

	static int m_nNumAll;		// �ǂݍ��񂾑���
	STexture m_pTexInfo[mylib_const::MAX_OBJ];	// �e�N�X�`���̏��
};



#endif