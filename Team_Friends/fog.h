//==========================================
//
//  �t�H�O���Ǘ����閼�O���(fog.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _FOG_H_
#define _FOG_H_

//==========================================
//  ���O��Ԃ̒�`
//==========================================
namespace Fog
{
	//�g�p�ɕK�{�̊֐�
	void Set(bool bUse); //�t�H�O�̐ݒ�
	void Draw(); //�`��

	//����ݒ肷�邽�߂̊֐�
	void SetCol(D3DXCOLOR col);
	void SetStart(float fStart);
	void SetEnd(float fEnd);

	//�����擾���邽�߂̊֐�
	D3DXCOLOR GetCol(void);
	float GetStart(void);
	float GetEnd(void);
}

#endif
