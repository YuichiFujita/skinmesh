//=============================================================================
// 
//  �}�b�v�w�b�_�[ [map.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MAP_H_
#define _MAP_H_	// ��d�C���N���[�h�h�~

#include "main.h"

//==========================================================================
// ���O���
//==========================================================================
namespace map
{
	HRESULT Create(const char *pTextFile);	// ��������
	HRESULT ReadTexture(void);	// �e�N�X�`���ǂݍ��ݏ���
	HRESULT ReadXFile(void);	// ���ł�ǂݍ��ݏ���
	HRESULT ReadText(const char *pTextFile);	// �O���t�@�C���ǂݍ��ݏ���
	HRESULT SaveText(void);	// �O���t�@�C���������ݏ���
	int GetNumModelAll(void);	// �����擾
	void Release(void);
}

#endif