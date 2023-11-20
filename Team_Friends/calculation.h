//=============================================================================
// 
// �֗��֐� [calculation.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MAX_COMMENT	(512)	// �R�����g�̍ő吔
#define MOVE_LOG	(64)	// ���S�̍ő吔

// UI�g�k�̃��[�h
typedef enum
{
	MOVELOG_TYPE_ADD = 0,	// �g��
	MOVELOG_TYPE_REMOVE,	// �k��
	MOVELOG_TYPE_SAME,		// ������
	MOVELOG_TYPE_MAX
}MOVELOG_TYPE;

// ���S�g�k�̍\����
typedef struct
{
	int nCnt;		// ���S�g�k�̃J�E���g
	int nPattern;	// ���S�g�k�̃p�^�[��
	float Length;	// ���S�g�k�̒���
	bool bUse;		// �g�p���Ă��邩
}MOVELOG;

//==========================================================================
// �v���g�^�C�v�錾
//==========================================================================
void InitCalculastion(void);
void RotNormalize(float &fRot);												// �p�x�̐��K��(-3.14 || 3.14��������␳)
void RotNormalize(D3DXVECTOR3 &rot);										// �p�x�̐��K��(-3.14 || 3.14��������␳)
template<class T> void ValueNormalize(T &Value, T MaxValue, T MinValue);	// �l�̐��K������
D3DXVECTOR3 WorldMtxChangeToRotation(D3DXMATRIX worldmtx);					// ���[���h�}�g���b�N�X��rot�ɕϊ�
void InertiaCorrection(float &fMain, float fTarget, float fCntCorrection);	// �����␳
float Lerp(float start, float end, float time);								// ���`�␳
float EasingLinear(float start, float end, float time);						// ���`�␳(����)
float EasingEaseIn(float start, float end, float time);						// ���`�␳(���X�ɉ���)
float EasingEaseOut(float start, float end, float time);					// ���`�␳(���X�Ɍ���)
float EasingEaseInOut(float start, float end, float time);					// ���`�␳(�������Č���)
D3DXCOLOR HSVtoRGB(float H, float S, float V);										// HSV�ϊ�
int Random(int nMinNum, int nMaxNum);												// �����_������
int GetDigit(int nNum);																// �����擾
float GetRatio(D3DXVECTOR3 Mypos, D3DXVECTOR3 Targetpos);							// �����擾
float GetPosLength(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);								// �����擾
D3DXVECTOR3 GetPointOnEdge2D(D3DXVECTOR3 point1, D3DXVECTOR3 point2, float fRate);	// �ӏ�̈ʒu�擾(2D)
D3DXVECTOR3 GetParabola(D3DXVECTOR3 start, D3DXVECTOR3 end, float fMaxHeight, float time);				// �������ړ�
bool CircleRange2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);						// 1��2�̓����蔻��(�~)(2D)
bool CircleRange3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);						// 1��2�̓����蔻��(�~)(3D)
bool SphereRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);						// 1��2�̓����蔻��(��)
bool CylinderRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2);	// 1��2�̓����蔻��(��)
void CheckVtx(float RotY, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, LPD3DXMESH pMesh, BYTE *pVtxBuff);	// �S���_�`�F�b�N
void CollisionCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);		// �Ԃ�������␳
bool bHitCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);			// �Ԃ��������`�F�b�N
void CollisionLimitLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 &pPos, D3DXVECTOR3 posOld);													// �����̓����蔻��
bool CollisionTriangle(D3DXVECTOR3 posCenter, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);				// �O�p�̓����蔻��
bool CollisionSquare(D3DXVECTOR3 posLeftUP, D3DXVECTOR3 posRightUP, D3DXVECTOR3 posLeftDW, D3DXVECTOR3 posRightDW, D3DXVECTOR3 MainPos);			// �l�p�̓����蔻��
bool CollisionSquare2D(D3DXVECTOR3 posLeftUP, D3DXVECTOR3 posRightUP, D3DXVECTOR3 posLeftDW, D3DXVECTOR3 posRightDW, D3DXVECTOR3 MainPos);			// �l�p�̓����蔻��(2D)
bool CollisionSquare(D3DXVECTOR3 &posSquare, D3DXVECTOR2 SquareSize, float fSquareRot, D3DXVECTOR3 MainPos);										// �l�p�̓����蔻��
bool CollisionSquareSquare2D(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &TargetPos, D3DXVECTOR2 MainSize, D3DXVECTOR2 TargetSize, float fMainRot, float fTargetRot);				// �l�p�Ǝl�p�̓����蔻��
bool CollisionCircleSquare2D(D3DXVECTOR3 &posCircle, D3DXVECTOR3 &posSquare, D3DXVECTOR3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize);	// �~�Ƌ�`�̓����蔻��(2D)
bool CollisionCircleSquare3D(D3DXVECTOR3 &posCircle, D3DXVECTOR3 &posSquare, D3DXVECTOR3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize);	// �~�Ƌ�`�̓����蔻��(3D)
bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 TargetPos, float fTargetRadius);	// ��̓����蔻��
bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);							// �����̉E�ɂ��邩�ǂ���
bool CollisionLine2D(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);							// �����̉E�ɂ��邩�ǂ���
float GetVtxHeight(D3DXVECTOR3 pos, D3DXVECTOR3 NowPos, D3DXVECTOR3 posRight, D3DXVECTOR3 posLeft, bool &bLand);				// ���������߂�
int SetMoveLog(void);																											// ���S�g�k���鎞�̃C���f�b�N�X�擾
float MoveLog(int nIdxMoveLog, float pos, int nCntElapsed);																		// pVtx[?].pos�̒����ɂȂ���
void ResetMoveLog(int nIdxMoveLog);																								// �������g�k�������Ƃ�
D3DXVECTOR3 CatmullRomSplineInterp(D3DXVECTOR3 PointMili1, D3DXVECTOR3 Point0, D3DXVECTOR3 Point1, D3DXVECTOR3 Point2, float fTime);
template<class T> void CalSort(T *pValue, int nStart, int nMax);	// �\�[�g
template<class T> void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt);	// �񎟋Ȑ��␳


//==========================================================================
// �l�̐��K������
//==========================================================================
template<class T> void ValueNormalize(T &Value, T MaxValue, T MinValue)
{
	if (Value >= MaxValue)
	{// �ő�l�𒴂�����

		// �ő�l�Œ�
		Value = MaxValue;
	}
	else if (Value <= MinValue)
	{// �ŏ��l�����������

		// �ŏ��l�Œ�
		Value = MinValue;
	}
}

//==========================================================================
// �񎟋Ȑ��␳
//==========================================================================
template<class T> void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt)
{
	// �⊮�̒l����o��
	T Comp = ((T)Max / (T)(nRange * nRange));

	if (nAlphaCnt > nRange)
	{// �͈͒�������߂�
		nAlphaCnt = -nRange;
	}
	else if (nAlphaCnt < -nRange)
	{// �͈͒�������߂�
		nAlphaCnt = nRange;
	}

	// �s�����̃J�E���g���Z
	nAlphaCnt++;

	// �Ȑ��␳
	Value = (T)((Comp * (nAlphaCnt * nAlphaCnt)) - (2 * (Comp * nAlphaCnt * RangeOrigin)) + (Comp * (RangeOrigin * RangeOrigin)));
	Value += RangeOrigin;
}

//==========================================================================
// �\�[�g
//==========================================================================
template<class T> void CalSort(T *pValue, int nStart, int nMax)
{
	for (int nCntData = nStart; nCntData < nMax - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < nMax; nCntSort++)
		{
			T nTemp = pValue[nCntData];
			pValue[nCntData] = pValue[nCntSort];
			pValue[nCntSort] = nTemp;
		}
	}
}


#endif