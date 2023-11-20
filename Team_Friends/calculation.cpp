//==================================================================================
// 
// �֗��֐�[calculation.cpp]
// Author:���n�Ή�
// 
//==================================================================================
#include "calculation.h"
#include "constans.h"
#include "2D_effect.h"

// �O���[�o���ϐ��錾
MOVELOG g_aMoveLog[MOVE_LOG];		// ���S�g�k�̍\����

//==================================================================================
// ����������
//==================================================================================
void InitCalculastion(void)
{
	// ���S�g�k
	for (int nCntLog = 0; nCntLog < MOVE_LOG; nCntLog++)
	{
		g_aMoveLog[nCntLog].nCnt = 0;
		g_aMoveLog[nCntLog].nPattern = 0;
		g_aMoveLog[nCntLog].Length = 0.0f;
		g_aMoveLog[nCntLog].bUse = false;
	}
}

//==================================================================================
// ���S�̐ݒ�
//==================================================================================
int SetMoveLog(void)
{
	int nIdxMoveLog = -1;

	for (int nCntLog = 0; nCntLog < MOVE_LOG; nCntLog++)
	{
		if (g_aMoveLog[nCntLog].bUse == false)
		{// �g�p����Ă��Ȃ�������

		 // �C���f�b�N�X�ێ����ď�������
			g_aMoveLog[nCntLog].bUse = true;
			nIdxMoveLog = nCntLog;
			break;
		}
	}

	return nIdxMoveLog;
}

//==================================================================================
// ���S�̑��΍��W�Z�o
//==================================================================================
float MoveLog(int nIdxMoveLog, float pos, int nCntElapsed)
{

	// �J�E���g���Z
	g_aMoveLog[nIdxMoveLog].nCnt = (g_aMoveLog[nIdxMoveLog].nCnt + 1) % nCntElapsed;

	if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_ADD)
	{// �g��

		if ((g_aMoveLog[nIdxMoveLog].nCnt % nCntElapsed) == 0)
		{
			g_aMoveLog[nIdxMoveLog].nPattern = (g_aMoveLog[nIdxMoveLog].nPattern + 1) % MOVELOG_TYPE_MAX;	// �p�^�[��No.���X�V
		}

		g_aMoveLog[nIdxMoveLog].Length = pos + g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;
	}
	else if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_REMOVE)
	{// �k��

		if ((g_aMoveLog[nIdxMoveLog].nCnt % nCntElapsed) == 0)
		{
			g_aMoveLog[nIdxMoveLog].nPattern = (g_aMoveLog[nIdxMoveLog].nPattern + 1) % MOVELOG_TYPE_MAX;	// �p�^�[��No.���X�V
		}

		g_aMoveLog[nIdxMoveLog].Length = pos - g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;
	}
	else if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_SAME)
	{// �߂�

		// �g�傷��
		g_aMoveLog[nIdxMoveLog].Length = pos + g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;

		if (g_aMoveLog[nIdxMoveLog].Length >= pos)
		{// �K��l�ȏ�

		 // �K��l�ɖ߂�
			g_aMoveLog[nIdxMoveLog].Length = pos;
		}
	}

	return g_aMoveLog[nIdxMoveLog].Length;
}

//==================================================================================
// ���S�̃��Z�b�g
//==================================================================================
void ResetMoveLog(int nIdxMoveLog)
{
	g_aMoveLog[nIdxMoveLog].nCnt = 0;
	g_aMoveLog[nIdxMoveLog].Length = 0.0f;
	g_aMoveLog[nIdxMoveLog].nPattern = MOVELOG_TYPE_ADD;
}

//==================================================================================
// �p�x�̐��K������
//==================================================================================
void RotNormalize(float &fRot)
{
	while (1)
	{
		// 1�����z������␳����
		if (fRot < -D3DX_PI)
		{
			fRot += D3DX_PI * 2.0f;
		}
		else if (fRot > D3DX_PI)
		{
			fRot -= D3DX_PI * 2.0f;
		}

		if (fRot >= -D3DX_PI && fRot <= D3DX_PI)
		{// �S�Đ��K���ς݂�OK
			break;
		}
	}
}

//==================================================================================
// �p�x�̐��K������
//==================================================================================
void RotNormalize(D3DXVECTOR3 &rot)
{
	// ���ꂼ��̐��K��
	RotNormalize(rot.x);
	RotNormalize(rot.y);
	RotNormalize(rot.z);
}

//==================================================================================
// ���[���h�}�g���b�N�X��rot�ɕϊ�
//==================================================================================
D3DXVECTOR3 WorldMtxChangeToRotation(D3DXMATRIX worldmtx)
{
	D3DXVECTOR3 rot = mylib_const::DEFAULT_VECTOR3;

	rot.x = atan2f(worldmtx._32, worldmtx._33);
	rot.y = atan2f(-worldmtx._31, sqrtf(worldmtx._32 * worldmtx._32 + worldmtx._33 * worldmtx._33));
	rot.z = atan2f(worldmtx._21, worldmtx._11);

	return rot;
}

//==================================================================================
// �����␳
//==================================================================================
void InertiaCorrection(float &fMain, float fTarget, float fCntCorrection)
{
	fMain += (fTarget - fMain) * fCntCorrection;
}

//==================================================================================
// ���`�␳
//==================================================================================
float Lerp(float start, float end, float time)
{
	// �͈͊O�␳
	ValueNormalize(time, 1.0f, 0.0f);
	float fValue = start + (end - start) * time;

	return fValue;
}

//==================================================================================
// ���`�␳(����)
//==================================================================================
float EasingLinear(float start, float end, float time)
{
	// �͈͊O�␳
	ValueNormalize(time, 1.0f, 0.0f);
	float fValue = start + (end - start) * time;

	return fValue;
}

//==================================================================================
// ���`�␳(���X�ɉ���)
//==================================================================================
float EasingEaseIn(float start, float end, float time)
{
	// �͈͊O�␳
	ValueNormalize(time, 1.0f, 0.0f);

	float fValue = start + (end - start) * time * time;

	return fValue;
}

//==================================================================================
// ���`�␳(���X�Ɍ���)
//==================================================================================
float EasingEaseOut(float start, float end, float time)
{
	// �͈͊O�␳
	ValueNormalize(time, 1.0f, 0.0f);

	float fSquaredTime = time * time;
	float fValue = start + (end - start) * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime));

	return fValue;
}

//==================================================================================
// ���`�␳(�������Č���)
//==================================================================================
float EasingEaseInOut(float start, float end, float time)
{
	// �͈͊O�␳
	ValueNormalize(time, 1.0f, 0.0f);
	float fValue = 0.0f;

	if (time < 0.5) {
		time = 2.0f * time;
		fValue = start + (end - start) * 0.5f * time * time;
	}
	else {
		time = 2.0f * time - 1.0f;
		float fSquaredTime = time * time;
		fValue = start + (end - start) * (0.5f * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime)) + 0.5f);
	}

	return fValue;
}

//==================================================================================
// HSV�ϊ�
//==================================================================================
D3DXCOLOR HSVtoRGB(float H, float S, float V)
{
	if (H > 360.0f)
	{
		H -= 360.0f;
	}
	else if (H < 0.0f)
	{
		H += 360.0f;
	}
	float C = V * S;
	float X = C * (1 - abs(fmod(H / 60.0f, 2) - 1));
	float m = V - C;

	// RGB�ʂ̐��l
	float R = 0.0f, G = 0.0f, B = 0.0f;
	if (H >= 0 && H < 60) {
		R = C;
		G = X;
		B = 0;
	}
	else if (H >= 60 && H < 120) {
		R = X;
		G = C;
		B = 0;
	}
	else if (H >= 120 && H < 180) {
		R = 0;
		G = C;
		B = X;
	}
	else if (H >= 180 && H < 240) {
		R = 0;
		G = X;
		B = C;
	}
	else if (H >= 240 && H < 300) {
		R = X;
		G = 0;
		B = C;
	}
	else {
		R = C;
		G = 0;
		B = X;
	}

	return D3DXCOLOR(R + m, G + m, B + m, 1.0f);
}

//==================================================================================
// �����_��
//==================================================================================
int Random(int nMinNum, int nMaxNum)
{
	// �����_����
	int nRand = 0;

	// �͈�
	int nRange = nMaxNum - nMinNum;

	if (nMinNum == 0)
	{// �ŏ��l��0�̎�

	 // �͈͂�1���Z
		nRange += 1;
	}

	// �����_������
	nRand = rand() % nRange + nMinNum;

	return nRand;
}

//==================================================================================
// �����擾
//==================================================================================
int GetDigit(int nNum)
{
	if (nNum <= 0)
	{
		nNum = 0;
	}

	// �����𕶎���ɕϊ�
	std::string number;
	sprintf((char*)number.c_str(), "%d", nNum);

	// ������̒����i�����j���擾
	int nDigit = strlen(number.c_str());

	if (nDigit <= 0)
	{
		nDigit = 1;
	}

	return nDigit;
}

//==================================================================================
// �����擾
//==================================================================================
float GetRatio(D3DXVECTOR3 Mypos, D3DXVECTOR3 Targetpos)
{
	return sqrtf((Mypos.x - Targetpos.x) * (Mypos.x - Targetpos.x)
			+ (Mypos.z - Targetpos.z) * (Mypos.z - Targetpos.z));
}

//==================================================================================
// �����擾
//==================================================================================
float GetPosLength(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z));
}

//==================================================================================
// �ӏ�̈ʒu�擾(2D)
//==================================================================================
D3DXVECTOR3 GetPointOnEdge2D(D3DXVECTOR3 point1, D3DXVECTOR3 point2, float fRate)
{
	D3DXVECTOR3 OnPoint = D3DXVECTOR3(
		point1.x + fRate * (point2.x - point1.x),
		point1.y + fRate * (point2.y - point1.y),
		0.0f);
	return OnPoint;
}

//==================================================================================
// �������ړ�
//==================================================================================
D3DXVECTOR3 GetParabola(D3DXVECTOR3 start, D3DXVECTOR3 end, float fMaxHeight, float time)
{
	// �ʒu
	D3DXVECTOR3 pos = mylib_const::DEFAULT_VECTOR3;

	// XZ�ړ�
	pos.x = start.x + (end.x - start.x) * time;
	pos.z = start.z + (end.z - start.z) * time;

	// ����
	pos.y = start.y + (end.y - start.y) * time + sinf(D3DX_PI * time) * fMaxHeight;
	//pos.y = start.y + (end.y - start.y) * time + (0.5f * fMaxHeight * time * time);

	return pos;
}

//==================================================================================
// �~�̓����蔻��(2D)
//==================================================================================
bool CircleRange2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength = 
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y);

	if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
	{// �~�̒��ɓ�������

		// ������������Ԃ�
		return true;
	}

	return false;
}

//==================================================================================
// �~�̓����蔻��(3D)
//==================================================================================
bool CircleRange3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength = 
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
	{// �~�̒��ɓ�������

		// ������������Ԃ�
		return true;
	}

	return false;
}

//==================================================================================
// ���̓����蔻��
//==================================================================================
bool SphereRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength = 
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
	{// �~�̒��ɓ�������

		// ������������Ԃ�
		return true;
	}

	return false;
}

//==================================================================================
// 1��2�̓����蔻��(��)
//==================================================================================
bool CylinderRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2)
{
	float fLength = 
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 * fRadius2) + (fRadius1 * fRadius2))
	{// �~�̒��ɓ�������

		if (pos1.y + fHeight1 >= pos2.y &&
			pos1.y <= pos2.y + fHeight2)
		{// �������Ԃ����Ă�����

			// ������������Ԃ�
			return true;
		}
	}

	return false;
}

//==================================================================================
// �S���_�`�F�b�N
//==================================================================================
void CheckVtx(float RotY, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, LPD3DXMESH pMesh, BYTE *pVtxBuff)
{
	//���Z�b�g
	*pVtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	*pVtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	//���f���̒��_�����擾
	int nNumVtx = pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//�S�Ă̒��_���`�F�b�N����
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//����̃f�[�^
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		//X
		if (pVtxMin->x > vtx.x)
		{//���̍ŏ��l��������̒l����������������

			pVtxMin->x = vtx.x;
		}
		if (pVtxMax->x < vtx.x)
		{//���̍ő�l��������̒l���傫��������

			pVtxMax->x = vtx.x;
		}

		//Y
		if (pVtxMin->y > vtx.y)
		{//���̍ŏ��l��������̒l����������������

			pVtxMin->y = vtx.y;
		}
		if (pVtxMax->y < vtx.y)
		{//���̍ő�l��������̒l���傫��������

			pVtxMax->y = vtx.y;
		}

		//Z
		if (pVtxMin->z > vtx.z)
		{//���̍ŏ��l��������̒l����������������

			pVtxMin->z = vtx.z;
		}
		if (pVtxMax->z < vtx.z)
		{//���̍ő�l��������̒l���傫��������

			pVtxMax->z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//�T�C�Y���|�C���^�ړ�
	}

	//���_�o�b�t�@���A�����b�N
	pMesh->UnlockVertexBuffer();

	D3DXVECTOR3 VtxMax = *pVtxMax, VtxMin = *pVtxMin;

	if (RotY <= D3DX_PI * 0.5f + 0.05f && RotY >= D3DX_PI * 0.5f - 0.05f)
	{//1.57f�̂Ƃ�

		pVtxMax->x = VtxMax.z;
		pVtxMin->x = VtxMin.z;

		pVtxMax->z = -VtxMin.x;
		pVtxMin->z = -VtxMax.x;
	}
	else if (RotY <= -D3DX_PI * 0.5f + 0.05f && RotY >= -D3DX_PI * 0.5f - 0.05f)
	{//-1.57f�̂Ƃ�
		
		////////////////////////////////

		pVtxMax->x = -VtxMin.z;
		pVtxMin->x = -VtxMax.z;

		pVtxMax->z = VtxMax.x;
		pVtxMin->z = VtxMin.x;
	}
	else if (RotY <= D3DX_PI + 0.05f && RotY >= D3DX_PI - 0.05f)
	{//D3DX_PI�̂Ƃ�

	 ////////////////////////////////

		pVtxMax->x = -VtxMin.x;
		pVtxMin->x = -VtxMax.x;

		pVtxMax->z = -VtxMin.z;
		pVtxMin->z = -VtxMax.z;
	}
	else if (RotY <= -D3DX_PI + 0.05f && RotY >= -D3DX_PI - 0.05f)
	{//-D3DX_PI�̂Ƃ�

		pVtxMax->x = -VtxMin.x;
		pVtxMin->x = -VtxMax.x;

		pVtxMax->z = -VtxMin.z;
		pVtxMin->z = -VtxMax.z;
	}
}

//==================================================================================
// �L�������m�̓����蔻��(�␳)
//==================================================================================
void CollisionCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin)
{

	// ���̔���
	if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)		// �G�̍ő�l�ɓ����Ă�
	{// �G��Z���̒��ɂ���Ƃ�

		if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// �E����

			MainPos.x = (TargetPos.x + TargetVtxMax.x) - MainVtxMin.x + 0.1f;
		}
		if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ������

			MainPos.x = (TargetPos.x + TargetVtxMin.x) - MainVtxMax.x - 0.1f;
		}
	}

	// �c�̔���
	if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)		// �G�̍ő�l�ɓ����Ă�
	{// �G��X���̒��ɂ���Ƃ�

		if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// ������

			MainPos.z = (TargetPos.z + TargetVtxMax.z) - MainVtxMin.z + 0.1f;
		}
		if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ��O����

			MainPos.z = (TargetPos.z + TargetVtxMin.z) - MainVtxMax.z - 0.1f;
		}
	}
}

//==================================================================================
// �L�������m�̓����蔻��(�����������̔���Ԃ�)
//==================================================================================
bool bHitCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin)
{
	bool bHit = false;

	// ���̔���
	if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)		// �G�̍ő�l�ɓ����Ă�
	{// �G��Z���̒��ɂ���Ƃ�

		if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// �E����

			bHit = true;
		}
		if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ������

			bHit = true;
		}
	}

	// �c�̔���
	if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)		// �G�̍ő�l�ɓ����Ă�
	{// �G��X���̒��ɂ���Ƃ�

		if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// ������

			bHit = true;
		}
		if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ��O����

			bHit = true;
		}
	}

	return bHit;
}

//==================================================================================
// �����̓����蔻��
//==================================================================================
void CollisionLimitLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 &pPos, D3DXVECTOR3 posOld)
{
	// ���E���̃x�N�g��
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// �v���C���[�̋��E���̃x�N�g��
	D3DXVECTOR3 vecLinePlayer;
	vecLinePlayer.x = pPos.x - posOld.x;
	vecLinePlayer.z = pPos.z - posOld.z;

	// �v���C���[�ƕǂ̃x�N�g��
	D3DXVECTOR3 vecToPosPlayer;
	vecToPosPlayer.x = pos1.x - posOld.x;
	vecToPosPlayer.z = pos1.z - posOld.z;

	// �ʐς̍ő�l
	float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

	// ����̖ʐ�
	float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

	// ����
	float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

	if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
	{// �ʐς͈͓̔��ɂ����画��

		if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
		{// �ǂɓ���������

			//// ��_����͂ݏo����
			//D3DXVECTOR3 CollisionPointPlayer = D3DXVECTOR3(0.0f, pPos.y, 0.0f);
			//CollisionPointPlayer.x = (pPos.x + (vecLinePlayer.x * (fRatePlayer - 1.0f)));
			//CollisionPointPlayer.z = (pPos.z + (vecLinePlayer.z * (fRatePlayer - 1.0f)));

			// �@���x�N�g��(���E���x�N�g����X��Z���])
			D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&vecNor, &vecNor);

			// �v���C���[�̋t�ړ���
			D3DXVECTOR3 PlayerInverceMove;
			PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);
			PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);

			// ����(�ǂ̖@���ƃv���C���[�̋t�ړ���)
			float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

			// �ǂ���ړ���
			D3DXVECTOR3 MoveWall = vecNor * fDot;

			/*D3DXVECTOR3 ppp = (CollisionPointPlayer - pPos);
			D3DXVECTOR3 pppp = ppp + MoveWall;*/

			// �Ԃ������_�ɕ␳
			pPos += MoveWall + (vecNor * 0.1f);

		}
	}
}

//==================================================================================
//  ��̓����蔻��
//==================================================================================
bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 TargetPos, float fTargetRadius)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bCircle = false;

	// ���̓_�Ƃ̐���
	bLine1 = CollisionLine(posLeft, posChar, TargetPos, TargetPos);

	// �E�̓_�Ƃ̐���
	bLine2 = CollisionLine(posChar, posRight, TargetPos, TargetPos);

	// ��̔��a
	float fLen = 
		sqrtf((posLeft.x - posChar.x) * (posLeft.x - posChar.x)
		+ (posLeft.z - posChar.z) * (posLeft.z - posChar.z));

	// �~�̓����蔻��
	if (CircleRange3D(posChar, TargetPos, fLen, fTargetRadius) == true)
	{// �������Ă�����
		bCircle = true;
	}

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bCircle == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �O�p�̓����ɂ��邩����
//==================================================================================
bool CollisionTriangle(D3DXVECTOR3 posCenter, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bLine3 = false;

	// �^�񒆂ƍ��̐�
	bLine1 = CollisionLine(posCenter, posLeft, MainPos, MainPosOld);

	// ���ƉE�̐�
	bLine2 = CollisionLine(posLeft, posRight, MainPos, MainPosOld);

	// �E�Ɛ^�񒆂̐�
	bLine3 = CollisionLine(posRight, posCenter, MainPos, MainPosOld);

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bLine3 == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �l�p�̓����ɂ��邩����
//==================================================================================
bool CollisionSquare(D3DXVECTOR3 posLeftUP, D3DXVECTOR3 posRightUP, D3DXVECTOR3 posLeftDW, D3DXVECTOR3 posRightDW, D3DXVECTOR3 MainPos)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

	// ����ƉE��
	bLine1 = CollisionLine(posRightUP, posLeftUP, MainPos, MainPos);

	// �E��ƉE��
	bLine2 = CollisionLine(posRightDW, posRightUP, MainPos, MainPos);

	// �E���ƍ���
	bLine3 = CollisionLine(posLeftDW, posRightDW, MainPos, MainPos);

	// �����ƍ���
	bLine4 = CollisionLine(posLeftUP, posLeftDW, MainPos, MainPos);

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bLine3 == true && bLine4 == true)
	{
		// ��������
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �l�p�̓����ɂ��邩����(2D)
//==================================================================================
bool CollisionSquare2D(D3DXVECTOR3 posLeftUP, D3DXVECTOR3 posRightUP, D3DXVECTOR3 posLeftDW, D3DXVECTOR3 posRightDW, D3DXVECTOR3 MainPos)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

	// ����ƉE��
	bLine1 = CollisionLine2D(posRightUP, posLeftUP, MainPos, MainPos);

	// �E��ƉE��
	bLine2 = CollisionLine2D(posRightDW, posRightUP, MainPos, MainPos);

	// �E���ƍ���
	bLine3 = CollisionLine2D(posLeftDW, posRightDW, MainPos, MainPos);

	// �����ƍ���
	bLine4 = CollisionLine2D(posLeftUP, posLeftDW, MainPos, MainPos);

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bLine3 == true && bLine4 == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �l�p�̓����ɂ��邩����
//==================================================================================
bool CollisionSquare(D3DXVECTOR3 &posSquare, D3DXVECTOR2 SquareSize, float fSquareRot, D3DXVECTOR3 MainPos)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

	float fLength = sqrtf(SquareSize.x * SquareSize.x + SquareSize.y * SquareSize.y);	// �Ίp���̒���
	float fAngle = atan2f(SquareSize.x, SquareSize.y);									// �Ίp���̌���

	// ���肷��l�p��4���_
	D3DXVECTOR3 LeftUp = D3DXVECTOR3(posSquare.x + sinf(fSquareRot - fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot - fAngle) * fLength);
	D3DXVECTOR3 RightUp = D3DXVECTOR3(posSquare.x + sinf(fSquareRot + fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot + fAngle) * fLength);
	D3DXVECTOR3 LeftDown = D3DXVECTOR3(posSquare.x + sinf(fSquareRot - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot - D3DX_PI + fAngle) * fLength);
	D3DXVECTOR3 RightDown = D3DXVECTOR3(posSquare.x + sinf(fSquareRot + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot + D3DX_PI - fAngle) * fLength);

	// ����ƉE��
	bLine1 = CollisionLine(RightUp, LeftUp, MainPos, MainPos);

	// �E��ƉE��
	bLine2 = CollisionLine(RightDown, RightUp, MainPos, MainPos);

	// �E���ƍ���
	bLine3 = CollisionLine(LeftDown, RightDown, MainPos, MainPos);

	// �����ƍ���
	bLine4 = CollisionLine(LeftUp, LeftDown, MainPos, MainPos);

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bLine3 == true && bLine4 == true)
	{
		// ��������
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �l�p�Ǝl�p�̓����蔻��
//==================================================================================
bool CollisionSquareSquare2D(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &TargetPos, D3DXVECTOR2 MainSize, D3DXVECTOR2 TargetSize, float fMainRot, float fTargetRot)
{
	float fTargetLength = sqrtf(
		TargetSize.x * TargetSize.x +
		TargetSize.y * TargetSize.y);							// �Ίp���̒���
	float fTargetAngle = atan2f(TargetSize.x, TargetSize.y);	// �Ίp���̌���
	
	// ���肷��l�p��4���_
	D3DXVECTOR3 LeftUp = D3DXVECTOR3(TargetPos.x + sinf(fTargetRot - fTargetAngle) * fTargetLength, TargetPos.y, TargetPos.z + cosf(fTargetRot - fTargetAngle) * fTargetLength);
	D3DXVECTOR3 RightUp = D3DXVECTOR3(TargetPos.x + sinf(fTargetRot + fTargetAngle) * fTargetLength, TargetPos.y, TargetPos.z + cosf(fTargetRot + fTargetAngle) * fTargetLength);
	D3DXVECTOR3 LeftDown = D3DXVECTOR3(TargetPos.x + sinf(fTargetRot - D3DX_PI + fTargetAngle) * fTargetLength, TargetPos.y, TargetPos.z + cosf(fTargetRot - D3DX_PI + fTargetAngle) * fTargetLength);
	D3DXVECTOR3 RightDown = D3DXVECTOR3(TargetPos.x + sinf(fTargetRot + D3DX_PI - fTargetAngle) * fTargetLength, TargetPos.y, TargetPos.z + cosf(fTargetRot + D3DX_PI - fTargetAngle) * fTargetLength);

	//***********************
	// ��`�̔���
	//***********************
	if (CollisionSquare(MainPos, MainSize, fMainRot, LeftUp) == true)
	{// ����
		return true;
	}
	if (CollisionSquare(MainPos, MainSize, fMainRot, RightUp) == true)
	{// �E��
		return true;
	}
	if (CollisionSquare(MainPos, MainSize, fMainRot, LeftDown) == true)
	{// ����
		return true;
	}
	if (CollisionSquare(MainPos, MainSize, fMainRot, RightDown) == true)
	{// �E��
		return true;
	}

	return false;
}

//==================================================================================
// �~�Ƌ�`�̓����蔻��(2D)
//==================================================================================
bool CollisionCircleSquare2D(D3DXVECTOR3 &posCircle, D3DXVECTOR3 &posSquare, D3DXVECTOR3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize)
{

	float fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
	float fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

	float fLength = 0.0f;	// �Ίp���̒���
	float fAngle = 0.0f;	// �Ίp���̌���

	D3DXVECTOR3 LeftUp, RightUp, LeftDown, RightDown;

	//****************************************************
	// X�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
	//****************************************************
	fSquareSizeX += fCircleRadius;	// ��`�̃T�C�YX
	fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

	fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
	fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

	// ���肷��l�p��4���_
	LeftUp.x = posSquare.x + sinf(rotSquare.z - D3DX_PI + fAngle) * fLength;
	LeftUp.y = posSquare.y + cosf(rotSquare.z - D3DX_PI + fAngle) * fLength;
	LeftUp.z = 0.0f;

	RightUp.x = posSquare.x + sinf(rotSquare.z + D3DX_PI - fAngle) * fLength;
	RightUp.y = posSquare.y + cosf(rotSquare.z + D3DX_PI - fAngle) * fLength;
	RightUp.z = 0.0f;

	LeftDown.x = posSquare.x + sinf(rotSquare.z - fAngle) * fLength;
	LeftDown.y = posSquare.y + cosf(rotSquare.z - fAngle) * fLength;
	LeftDown.z = 0.0f;

	RightDown.x = posSquare.x + sinf(rotSquare.z + fAngle) * fLength;
	RightDown.y = posSquare.y + cosf(rotSquare.z + fAngle) * fLength;
	RightDown.z = 0.0f;

	// ��`�̔���
	if (CollisionSquare2D(LeftUp, RightUp, LeftDown, RightDown, posCircle) == true)
	{// ��`�ɓ��������ꍇ
		return true;
	}

	//****************************************************
	// Y�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
	//****************************************************
	fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
	fSquareSizeY += fCircleRadius;	// ��`�̃T�C�YY

	fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
	fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

	// ���肷��l�p��4���_
	LeftUp.x = posSquare.x + sinf(rotSquare.z - D3DX_PI + fAngle) * fLength;
	LeftUp.y = posSquare.y + cosf(rotSquare.z - D3DX_PI + fAngle) * fLength;
	LeftUp.z = 0.0f;

	RightUp.x = posSquare.x + sinf(rotSquare.z + D3DX_PI - fAngle) * fLength;
	RightUp.y = posSquare.y + cosf(rotSquare.z + D3DX_PI - fAngle) * fLength;
	RightUp.z = 0.0f;

	LeftDown.x = posSquare.x + sinf(rotSquare.z - fAngle) * fLength;
	LeftDown.y = posSquare.y + cosf(rotSquare.z - fAngle) * fLength;
	LeftDown.z = 0.0f;

	RightDown.x = posSquare.x + sinf(rotSquare.z + fAngle) * fLength;
	RightDown.y = posSquare.y + cosf(rotSquare.z + fAngle) * fLength;
	RightDown.z = 0.0f;

	// ��`�̔���
	if (CollisionSquare2D(LeftUp, RightUp, LeftDown, RightDown, posCircle) == true)
	{// ��`�ɓ��������ꍇ
		return true;
	}


	//***********************
	// �e���_���̉~�̔���
	//***********************
	//****************************************************
	// ���̃T�C�Y
	//****************************************************
	fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
	fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

	fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
	fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

	// ���肷��l�p��4���_
	LeftUp.x = posSquare.x + sinf(rotSquare.z - D3DX_PI + fAngle) * fLength;
	LeftUp.y = posSquare.y + cosf(rotSquare.z - D3DX_PI + fAngle) * fLength;
	LeftUp.z = 0.0f;

	RightUp.x = posSquare.x + sinf(rotSquare.z + D3DX_PI - fAngle) * fLength;
	RightUp.y = posSquare.y + cosf(rotSquare.z + D3DX_PI - fAngle) * fLength;
	RightUp.z = 0.0f;

	LeftDown.x = posSquare.x + sinf(rotSquare.z - fAngle) * fLength;
	LeftDown.y = posSquare.y + cosf(rotSquare.z - fAngle) * fLength;
	LeftDown.z = 0.0f;

	RightDown.x = posSquare.x + sinf(rotSquare.z + fAngle) * fLength;
	RightDown.y = posSquare.y + cosf(rotSquare.z + fAngle) * fLength;
	RightDown.z = 0.0f;

	if (CircleRange2D(posCircle, LeftUp, fCircleRadius, 0.0f) == true)
	{// ����
		return true;
	}
	if (CircleRange2D(posCircle, RightUp, fCircleRadius, 0.0f) == true)
	{// �E��
		return true;
	}
	if (CircleRange2D(posCircle, LeftDown, fCircleRadius, 0.0f) == true)
	{// ����
		return true;
	}
	if (CircleRange2D(posCircle, RightDown, fCircleRadius, 0.0f) == true)
	{// �E��
		return true;
	}

	// �������ĂȂ������Ԃ�
	return false;
}

//==================================================================================
// �~�Ƌ�`�̓����蔻��(3D)
//==================================================================================
bool CollisionCircleSquare3D(D3DXVECTOR3 &posCircle, D3DXVECTOR3 &posSquare, D3DXVECTOR3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize)
{

	float fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
	float fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

	float fLength = 0.0f;	// �Ίp���̒���
	float fAngle = 0.0f;	// �Ίp���̌���

	D3DXVECTOR3 LeftUp, RightUp, LeftDown, RightDown;

	//****************************************************
	// X�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
	//****************************************************
	fSquareSizeX += fCircleRadius;	// ��`�̃T�C�YX
	fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

	fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
	fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

	// ���肷��l�p��4���_
	LeftUp = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - fAngle) * fLength);
	RightUp = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + fAngle) * fLength);
	LeftDown = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - D3DX_PI + fAngle) * fLength);
	RightDown = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + D3DX_PI - fAngle) * fLength);

	// ��`�̔���
	if (CollisionSquare(LeftUp, RightUp, LeftDown, RightDown, posCircle) == true)
	{// ��`�ɓ��������ꍇ
		return true;
	}

	//****************************************************
	// Y�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
	//****************************************************
	fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
	fSquareSizeY += fCircleRadius;	// ��`�̃T�C�YY

	fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
	fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

	// ���肷��l�p��4���_
	LeftUp = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - fAngle) * fLength);
	RightUp = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + fAngle) * fLength);
	LeftDown = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - D3DX_PI + fAngle) * fLength);
	RightDown = D3DXVECTOR3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + D3DX_PI - fAngle) * fLength);

	// ��`�̔���
	if (CollisionSquare(LeftUp, RightUp, LeftDown, RightDown, posCircle) == true)
	{// ��`�ɓ��������ꍇ
		return true;
	}


	//***********************
	// �e���_���̉~�̔���
	//***********************
	if (CircleRange3D(posCircle, LeftUp, fCircleRadius, fCircleRadius) == true)
	{// ����
		return true;
	}
	if (CircleRange3D(posCircle, RightUp, fCircleRadius, fCircleRadius) == true)
	{// �E��
		return true;
	}
	if (CircleRange3D(posCircle, LeftDown, fCircleRadius, fCircleRadius) == true)
	{// ����
		return true;
	}
	if (CircleRange3D(posCircle, RightDown, fCircleRadius, fCircleRadius) == true)
	{// �E��
		return true;
	}

	// �������ĂȂ������Ԃ�
	return false;
}

//==================================================================================
// ���������߂�
//==================================================================================
float GetVtxHeight(D3DXVECTOR3 pos, D3DXVECTOR3 NowPos, D3DXVECTOR3 posRight, D3DXVECTOR3 posLeft, bool &bLand)
{
	// ����
	float fHeight = 0.0f;

	// ���p�����ƈʒu�̃x�N�g��
	D3DXVECTOR3 calvec = pos - NowPos;

	// �x�N�g���Ɩ@��
	D3DXVECTOR3 
		vec1 = posRight - NowPos,
		vec2 = posLeft - posRight,
		nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CollisionTriangle(NowPos, posRight, posLeft, pos, pos) == true)
	{// �O�p�ɓ����Ă�����

		// �O�ς����߂�
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// �O�ς̐��K�������Ė@���ɂ���
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{// �@����0.0f����Ȃ�������

			// ���������߂�
			fHeight = -((calvec.x * nor.x) + (-NowPos.y * nor.y) + (calvec.z * nor.z)) / nor.y;
			bLand = true;
		}
		else
		{
			// ���������߂�
			fHeight = -((calvec.x * nor.x) + (-NowPos.y * nor.y) + (calvec.z * nor.z)) / nor.y;
			bLand = true;
		}
	}

	return fHeight;
}

//==================================================================================
//  ���̓����蔻��  
//==================================================================================
bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// �����������̔���
	bool bHit = false;

	// ���E���̃x�N�g��
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// �e�ƕǂ̃x�N�g��
	D3DXVECTOR3 vecToPos;
	vecToPos.x = MainPos.x - pos0.x;
	vecToPos.z = MainPos.z - pos0.z;

	if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) <= 0)
	{// ���𒴂���

		// ��������
		bHit = true;
	}

	return bHit;
}

//==================================================================================
//  ���̓����蔻�� (2D) 
//==================================================================================
bool CollisionLine2D(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// �����������̔���
	bool bHit = false;

	// ���E���̃x�N�g��
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.y = pos0.y - pos1.y;

	// �e�ƕǂ̃x�N�g��
	D3DXVECTOR3 vecToPos;
	vecToPos.x = MainPos.x - pos0.x;
	vecToPos.y = pos0.y - MainPos.y;

	if ((vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y) <= 0)
	{// ���𒴂���

		// ��������
		bHit = true;
	}

	return bHit;
}

//==================================================================================
//  �X�v���C�g�Ȑ��␳
//==================================================================================
D3DXVECTOR3 CatmullRomSplineInterp(D3DXVECTOR3 PointMili1, D3DXVECTOR3 Point0, D3DXVECTOR3 Point1, D3DXVECTOR3 Point2, float fTime)
{
	// 4�_�ŕ␳
	D3DXVECTOR3 a4 = Point0;
	D3DXVECTOR3 a3 = (Point1 - PointMili1) / 2.0f;
	D3DXVECTOR3 a1 = (Point2 - Point0) / 2.0f - 2.0f * Point1 + a3 + 2.0f * a4;
	D3DXVECTOR3 a2 = 3.0f * Point1 - (Point2 - Point0) / 2.0f - 2.0f * a3 - 3.0f * a4;

	return (a1 * fTime * fTime * fTime + a2 * fTime * fTime + a3 * fTime + a4);
}
