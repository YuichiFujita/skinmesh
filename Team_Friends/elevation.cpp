//=============================================================================
// 
//  �N������ [elevation.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "elevation.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"
#include "input.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "player.h"
#include "model.h"
#include "ballast.h"
#include "enemy.h"
#include "impactwave.h"
#include "camera.h"
#include "enemymanager.h"
#include "sound.h"
#include "tutorial.h"
#include "tutorialstep.h"
#include "ranking.h"
#include "targetpoint.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE		"data\\TEXTURE\\glass.jpg"	// �e�N�X�`��
#define MAX_MOVE	(8.0f)		// �ړ���
#define MAX_RANGE	(200.0f)	// �͈�
#define MOVE_LEN	(1.0f)		// ���ړ���
#define MIN_HEIGHT	(-400.0f)	// �Œ�̍���
#define MOVE		(5.0f)
#define IMPACTSHAKE_DIRECT	(30.0f)
#define IMPACTSHAKE_IMPACT	(20.0f)
#define SHAKETIME_DIRECT	(15 + 5)
#define SHAKETIME_IMPACT	(10 + 5)
#define SPAWN_WAITTIME		(20)
#if 1
#define CHANGE	(0)
#endif

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
bool CElevation::m_bLoadInfo = false;		// ���ǂݍ��ݔ���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CElevation::CElevation(int nPriority) : CObject3DMesh(nPriority)
{
	m_fBrushStrength = 0.0f;					// �u���V����
	m_fBrushRange = 0.0f;						// �͈�
	m_bEdit = false;							// �G�f�B�b�gON/OFF
	m_nTexIdx = 0;								// �e�N�X�`���̃C���f�b�N�X�ԍ�
	memset(&m_VtxPos, 0, sizeof(m_VtxPos));		// ���_���W

	m_aInfo.TextureFileName = {};				// �e�N�X�`���t�@�C����
	m_aInfo.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_aInfo.fWidthLength = 0.0f;				// ������
	m_aInfo.fHeightLength = 0.0f;				// �c����
	m_aInfo.nWidthBlock = 0;					// ��������
	m_aInfo.nHeightBlock = 0;					// �c������
	m_aInfo.pVtxPos = NULL;						// ���_���W
	m_pTargetP = NULL;							// �ڕW�̒n�_
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CElevation::~CElevation()
{

}

//==========================================================================
// ��������
//==========================================================================
CElevation *CElevation::Create(const char *pText)
{
	// �����p�̃I�u�W�F�N�g
	CElevation *pObjMeshField = NULL;

	if (pObjMeshField == NULL)
	{// NULL��������

		// �������̊m��
		pObjMeshField = DEBUG_NEW CElevation;

		if (pObjMeshField != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObjMeshField->Init(pText);
		}

		return pObjMeshField;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CElevation::Init(void)
{
	return E_FAIL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CElevation::Init(const char *pText)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// ��ސݒ�
	SetType(TYPE_ELEVATION);

	// ���[�h����
	hr = Load(pText);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �e��ϐ�������
	SetPosition(m_aInfo.pos);				// �ʒu
	SetWidthBlock(m_aInfo.nWidthBlock);		// ������
	SetHeightBlock(m_aInfo.nHeightBlock);	// �c����
	SetWidthLen(m_aInfo.fWidthLength);		// �c����
	SetHeightLen(m_aInfo.fHeightLength);	// ������
	m_fBrushStrength = MAX_MOVE;			// �u���V����
	m_fBrushRange = MAX_RANGE;				// �͈�

	//for (int nCntHeight = 0; nCntHeight < m_aInfo.nHeightBlock + 1; nCntHeight++)
	//{// �c�̕������J��Ԃ�

	//	for (int nCntWidth = 0; nCntWidth < m_aInfo.nWidthBlock + 1; nCntWidth++)
	//	{// ���̕������J��Ԃ�
	//		m_VtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y = m_aInfo.pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y;
	//	}
	//}

	// �I�u�W�F�N�g3D���b�V���̏���������
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CElevation::Uninit(void)
{
	// ���_���W�̔j��
	if (m_aInfo.pVtxPos != NULL)
	{
		delete[] m_aInfo.pVtxPos;
		m_aInfo.pVtxPos = NULL;
	}

	// �ڕW�̒n�_����
	if (m_pTargetP != NULL)
	{
		m_pTargetP->Uninit();
		m_pTargetP = NULL;
	}

	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �������
//==========================================================================
void CElevation::Release(void)
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CElevation::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{// �؂�ւ�
		m_bEdit = m_bEdit ? false : true;

		// �ڕW�̒n�_����
		if (m_pTargetP != NULL)
		{
			m_pTargetP->Uninit();
			m_pTargetP = NULL;
		}
		// �ڕW�̒n�_����
		else if (m_pTargetP == NULL)
		{
			m_pTargetP = CTargetPoint::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 400.0f, 800.0f);
		}
	}

	// ���_����
	if (m_bEdit == true)
	{// �G�f�B�b�g���Ȃ�
		UPVtxField(m_pTargetP->GetPosition());
	}
	else
	{
		// ��ԍX�V����
		UpdateState();
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CElevation::UpdateState(void)
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CElevation::Draw(void)
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (CManager::GetInstance()->IsWireframe() == true)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ���C���[�t���[�����[�h
	}

	// �`�揈��
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ���߂郂�[�h
}

//==========================================================================
// �����擾
//==========================================================================
float CElevation::GetHeight(D3DXVECTOR3& pos, bool &bLand)
{
	// �x�N�g���Ɩ@��
	D3DXVECTOR3 vec1, vec2, nor;
	float fHeight = 0.0f;
	D3DXVECTOR3 *pVtxPos = GetVtxPos();

	// �t�B�[���h�̈ʒu
	D3DXVECTOR3 posfield = GetPosition();

	// �����擾
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	// �ő�̒���
	float fMaxWidthLen = fWidthLen * GetWidthBlock();
	float fMaxHeightLen = -fWidthLen * GetHeightBlock();

	// ���肷�钸�_�̌v�Z
	int nCntWidth = 0;
	int nCntHeight = 0;
	int nWidthPoint = ((pos.x - posfield.x) - fMaxWidthLen * 0.5f) / fWidthLen + GetWidthBlock();
	int nHeightPoint = GetHeightBlock() - ((pos.z - posfield.z) - fMaxHeightLen * 0.5f) / fHeightLen;

	for (int nCntH = 0; nCntH < 2; nCntH++)
	{
		for (int nCntW = 0; nCntW < 2; nCntW++)
		{
			// �����_
			nCntWidth = nWidthPoint + (nCntW - 1);
			nCntHeight = nHeightPoint + (nCntH - 1);

			if (nCntWidth < 0 || m_aInfo.nWidthBlock < nCntWidth)
			{// �͈͊O�Ő擪
				continue;
			}

			if (nCntHeight < 0 || m_aInfo.nHeightBlock < nCntHeight)
			{// �͈͊O�Ő擪
				continue;
			}

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (m_aInfo.nWidthBlock + 1));
			int nVerTexW = (m_aInfo.nWidthBlock + 1) + 1;

			int nLeft = nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1));
			int nRight = nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1)) + nVerTexW;

			if (nLeft >= GetNumVertex() || nRight >= GetNumVertex())
			{
				continue;
			}

			fHeight = GetVtxHeight(pos, pVtxPos[nNowPoint] + posfield, pVtxPos[nLeft] + posfield, pVtxPos[nRight] + posfield, bLand);
			if (bLand)
			{
				break;
			}
			//if (CollisionTriangle(pVtxPos[nNowPoint] + posfield, pVtxPos[nLeft] + posfield, pVtxPos[nRight] + posfield, pos, pos) == true)
			//{// �O�p�ɓ����Ă�����

			//	// �x�N�g�����v�Z
			//	vec1 = pVtxPos[nRight] - pVtxPos[nNowPoint];
			//	vec2 = pVtxPos[nLeft] - pVtxPos[nNowPoint];

			//	// �O�ς����߂�
			//	D3DXVec3Cross(&nor, &vec1, &vec2);

			//	// �O�ς̐��K�������Ė@���ɂ���
			//	D3DXVec3Normalize(&nor, &nor);

			//	if (nor.y != 0.0f)
			//	{// �@����0.0f����Ȃ�������

			//		// ���������߂�
			//		fHeight = ((pos.x - pVtxPos[nNowPoint].x - posfield.x) * nor.x + (pos.z - pVtxPos[nNowPoint].z - posfield.z) * nor.z + (pVtxPos[nNowPoint].y * -nor.y)) / -nor.y;
			//		fHeight += GetPosition().y;
			//		bLand = true;
			//	}
			//	break;
			//}

			fHeight = GetVtxHeight(pos, pVtxPos[nNowPoint + GetWidthBlock()] + posfield, pVtxPos[nRight] + posfield, pVtxPos[nLeft] + posfield, bLand);
			if (bLand)
			{
				break;
			}
			//if (CollisionTriangle(pVtxPos[nNowPoint + GetWidthBlock()] + posfield, pVtxPos[nRight] + posfield, pVtxPos[nLeft] + posfield, pos, pos) == true)
			//{// �O�p�ɓ����Ă�����

			//	// �x�N�g�����v�Z
			//	vec1 = pVtxPos[nLeft] - pVtxPos[nNowPoint + m_aInfo.nWidthBlock];
			//	vec2 = pVtxPos[nRight] - pVtxPos[nNowPoint + m_aInfo.nWidthBlock];

			//	// �O�ς����߂�
			//	D3DXVec3Cross(&nor, &vec1, &vec2);

			//	// �O�ς̐��K�������Ė@���ɂ���
			//	D3DXVec3Normalize(&nor, &nor);

			//	if (nor.y != 0.0f)
			//	{// �@����0.0f����Ȃ�������

			//		// ���������߂�
			//		fHeight = ((pos.x - pVtxPos[nNowPoint + GetWidthBlock()].x - posfield.x) * nor.x + (pos.z - pVtxPos[nNowPoint + GetWidthBlock()].z - posfield.z) * nor.z + (pVtxPos[nNowPoint + GetWidthBlock()].y * -nor.y)) / -nor.y;
			//		fHeight += GetPosition().y;
			//		bLand = true;
			//		break;
			//	}
			//}

		}
	}

	if (bLand == true)
	{// ���n���Ă�����

		return fHeight;
	}

	// �������擾
	return pos.y;
}

//==========================================================================
// �n�ʂɓ���������
//==========================================================================
bool CElevation::IsHit(D3DXVECTOR3& pos)
{
	bool bHit = false;
	bool bLand = false;

	// �x�N�g���Ɩ@��
	D3DXVECTOR3 vec1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f), vec2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f), nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = pos.y;
	D3DXVECTOR3 *pVtxPos = GetVtxPos();
	D3DXVECTOR3 posfield = GetPosition();

	// �����擾
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	// �ő�̒���
	float fMaxWidthLen = fWidthLen * GetWidthBlock();
	float fMaxHeightLen = -fWidthLen * GetHeightBlock();

	// ���肷�钸�_�̌v�Z
	int nCntWidth = 0;
	int nCntHeight = 0;
	int nWidthPoint = ((pos.x - posfield.x) - fMaxWidthLen * 0.5f) / fWidthLen + GetWidthBlock();
	int nHeightPoint = GetHeightBlock() - ((pos.z - posfield.z) - fMaxHeightLen * 0.5f) / fHeightLen;

	for (int nCntH = 0; nCntH < 2; nCntH++)
	{
		for (int nCntW = 0; nCntW < 2; nCntW++)
		{
			// �����_
			nCntWidth = nWidthPoint + (nCntW - 1);
			nCntHeight = nHeightPoint + (nCntH - 1);

			if (nCntWidth < 0 || m_aInfo.nWidthBlock < nCntWidth)
			{// �͈͊O�Ő擪
				continue;
			}

			if (nCntHeight < 0 || m_aInfo.nHeightBlock < nCntHeight)
			{// �͈͊O�Ő擪
				continue;
			}

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (m_aInfo.nWidthBlock + 1));
			int nVerTexW = (m_aInfo.nWidthBlock + 1) + 1;

			int nLeft = nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1));
			int nRight = nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1)) + nVerTexW;

			if (nLeft >= GetNumVertex() || nRight >= GetNumVertex())
			{
				continue;
			}

			fHeight = GetVtxHeight(pos, pVtxPos[nNowPoint] + posfield, pVtxPos[nLeft] + posfield, pVtxPos[nRight] + posfield, bLand);
			if (bLand)
			{
				if (fHeight > pos.y)
				{// �O��̈ʒu��������������(�������Ă�����)
					bHit = true;
					pos.y = fHeight;
				}
				return bHit;
			}

			//if (CollisionTriangle(pVtxPos[nNowPoint], pVtxPos[nLeft], pVtxPos[nRight], pos, pos) == true)
			//{// �O�p�ɓ����Ă�����

			//	// �x�N�g�����v�Z
			//	vec1 = pVtxPos[nRight] - pVtxPos[nNowPoint];
			//	vec2 = pVtxPos[nLeft] - pVtxPos[nNowPoint];

			//	// �O�ς����߂�
			//	D3DXVec3Cross(&nor, &vec1, &vec2);

			//	// �O�ς̐��K�������Ė@���ɂ���
			//	D3DXVec3Normalize(&nor, &nor);

			//	if (nor.y != 0.0f)
			//	{// �@����0.0f����Ȃ�������

			//		// ���������߂�
			//		fHeight = ((pos.x - pVtxPos[nNowPoint].x) * nor.x + (pos.z - pVtxPos[nNowPoint].z) * nor.z + (pVtxPos[nNowPoint].y * -nor.y)) / -nor.y;
			//		fHeight += GetPosition().y;
			//		break;
			//	}
			//}

			bLand = false;
			fHeight = GetVtxHeight(pos, pVtxPos[nNowPoint + GetWidthBlock()] + posfield, pVtxPos[nRight] + posfield, pVtxPos[nLeft] + posfield, bLand);
			if (bLand)
			{
				if (fHeight > pos.y)
				{// �O��̈ʒu��������������(�������Ă�����)
					bHit = true;
					pos.y = fHeight;
				}
				return bHit;
			}

			//if (CollisionTriangle(pVtxPos[nNowPoint + m_aInfo.nWidthBlock], pVtxPos[nRight], pVtxPos[nLeft], pos, pos) == true)
			//{// �O�p�ɓ����Ă�����

			//	// �x�N�g�����v�Z
			//	vec1 = pVtxPos[nLeft] - pVtxPos[nNowPoint + m_aInfo.nWidthBlock];
			//	vec2 = pVtxPos[nRight] - pVtxPos[nNowPoint + m_aInfo.nWidthBlock];

			//	// �O�ς����߂�
			//	D3DXVec3Cross(&nor, &vec1, &vec2);

			//	// �O�ς̐��K�������Ė@���ɂ���
			//	D3DXVec3Normalize(&nor, &nor);

			//	if (nor.y != 0.0f)
			//	{// �@����0.0f����Ȃ�������

			//		// ���������߂�
			//		fHeight = ((pos.x - pVtxPos[nNowPoint + m_aInfo.nWidthBlock].x) * nor.x + (pos.z - pVtxPos[nNowPoint + m_aInfo.nWidthBlock].z) * nor.z + (pVtxPos[nNowPoint + m_aInfo.nWidthBlock].y * -nor.y)) / -nor.y;
			//		fHeight += GetPosition().y;
			//		break;
			//	}
			//}
		}
	}

	if (fHeight > pos.y)
	{// �O��̈ʒu��������������(�������Ă�����)
		bHit = true;
		pos.y = fHeight;
	}

	// �����Ԃ�
	return bHit;
}

//==========================================================================
// ���_�グ����
//==========================================================================
void CElevation::UPVtxField(D3DXVECTOR3 pos)
{

	float m_fWidthLen = GetWidthLen();
	float m_fHeightLen = GetHeightLen();
	D3DXVECTOR3 *pVtxPos = GetVtxPos();

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[ �N���G�f�B�b�g��� ]------------------\n"
		"�u���V�̃T�C�Y�F[1, 2] �y%f�z\n"
		"�u���V�̋����F  [3, 4] �y%f�z\n"
		"���_�グ�����F  [5, 6]\n"
		"���g�k�F        [7, 8] �y%f, %f�z\n"
		"�܂����F        [ �X ]\n"
		"���_���Z�b�g�F  [delete]\n"
		"�N�����ۑ��F  [F9]\n\n", m_fBrushRange, m_fBrushStrength, m_fWidthLen, m_fHeightLen);

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{// �Z�[�u
		Save();
	}

	// �u���V�̃T�C�Y
	if (pInputKeyboard->GetPress(DIK_1))
	{// 1�ŉ�����
		m_fBrushRange -= 1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_2))
	{// 2�ŏグ��
		m_fBrushRange += 1.0f;
	}

	// �u���V�̋���
	if (pInputKeyboard->GetPress(DIK_3))
	{// 3�ŉ�����
		m_fBrushStrength -= 0.01f;
	}

	if (pInputKeyboard->GetPress(DIK_4))
	{// 4�ŏグ��
		m_fBrushStrength += 0.01f;
	}

	// �t�B�[���h�̕�
	if (pInputKeyboard->GetPress(DIK_7))
	{// 7�ŏグ��
		m_fWidthLen -= MOVE_LEN;
		m_fHeightLen -= MOVE_LEN;
	}

	if (pInputKeyboard->GetPress(DIK_8))
	{// 8�ŉ�����
		m_fWidthLen += MOVE_LEN;
		m_fHeightLen += MOVE_LEN;
	}

	// �l�̐��K��
	ValueNormalize(m_fWidthLen, 99999999.0f, 10.0f);
	ValueNormalize(m_fHeightLen, 99999999.0f, 10.0f);

	// �l�̐��K��
	ValueNormalize(m_fBrushStrength, 99999999.0f, 0.1f);
	ValueNormalize(m_fBrushRange, 99999999.0f, 1.0f);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_aInfo.nHeightBlock + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_aInfo.nWidthBlock + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			if (pInputKeyboard->GetTrigger(DIK_DELETE))
			{// delete�Ń��Z�b�g
				m_aInfo.pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y = 0.0f;
			}

			float fNowLength =
				sqrtf((pos.x - pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].x) * (pos.x - pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].x)
					+ (pos.z - pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].z) * (pos.z - pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].z));

			float Wariai = fNowLength / m_fBrushRange;

			if (Wariai >= 1.0f)
			{// ������1.0f�ȉ�����
				continue;
			}

			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (pInputKeyboard->GetPress(DIK_5))
			{// 5�ŉ�����
				move.y = -m_fBrushStrength - (-m_fBrushStrength * Wariai);
			}

			if (pInputKeyboard->GetPress(DIK_6))
			{// 6�ŏグ��
				move.y = m_fBrushStrength - (m_fBrushStrength * Wariai);
			}

			if (pInputKeyboard->GetTrigger(DIK_9))
			{// 9�ł܂���
				m_aInfo.pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y = pos.y;
				move.y = 0.0f;
			}

			m_aInfo.pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y += move.y;
			// �e���_���W�m�F
			//pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y += move.y;
		}
	}

	// ���_�ݒ�
	SetVtxPos(pVtxPos);
	SetWidthLen(m_fWidthLen);
	SetHeightLen(m_fHeightLen);

	if (m_pTargetP != NULL)
	{
		// �����ύX
		m_pTargetP->SetWidthLen(m_fBrushRange);

		// �J�����̏��擾
		CCamera *pCamera = CManager::GetInstance()->GetCamera();

		// �J�����̌����擾
		D3DXVECTOR3 Camerarot = pCamera->GetRotation();

		if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		{// ���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_UP) == true)
			{// A+W,����ړ�

				pos.x += sinf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
				pos.z += cosf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
			}
			else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
			{// A+S,�����ړ�

				pos.x += sinf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
				pos.z += cosf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
			}
			else
			{// A,���ړ�

				pos.x += sinf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
				pos.z += cosf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		{// D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_UP) == true)
			{// D+W,�E��ړ�

				pos.x += sinf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
				pos.z += cosf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
			}
			else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
			{// D+S,�E���ړ�

				pos.x += sinf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
				pos.z += cosf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
			}
			else
			{// D,�E�ړ�

				pos.x += sinf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
				pos.z += cosf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_UP) == true)
		{// W�������ꂽ�A���ړ�

			pos.x += sinf(Camerarot.y) * MOVE;
			pos.z += cosf(Camerarot.y) * MOVE;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{// S�������ꂽ�A��O�ړ�

			pos.x += sinf(D3DX_PI + Camerarot.y) * MOVE;
			pos.z += cosf(D3DX_PI + Camerarot.y) * MOVE;
		}

		// �ʒu�ݒ�
		m_pTargetP->SetPosition(pos);
	}

}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CElevation::SetVtx(void)
{

	D3DXVECTOR3 *pVtxPos = GetVtxPos();
	D3DXVECTOR3 *pVtxNor = GetVtxNor();
	D3DXVECTOR3 vec1, vec2, nor;
	D3DXVECTOR3 VtxRight, VtxLeft, VtxNow;
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	for (int nCntHeight = 0; nCntHeight < m_aInfo.nHeightBlock + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_aInfo.nWidthBlock + 1; nCntWidth++)
		{// ���̕������J��Ԃ�

			m_VtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].x = pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].x;
			m_VtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].z = pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].z;

			// ���v�l��������
			pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))] = m_VtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))];
			pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y += m_aInfo.pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y;

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (m_aInfo.nWidthBlock + 1));
			int nVerTexW = (m_aInfo.nWidthBlock + 1) + 1;

			int nLeft = nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1));
			int nRight = nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1)) + nVerTexW;

			if (nNowPoint >= (m_aInfo.nWidthBlock + 1) * (m_aInfo.nHeightBlock + 1))
			{
				continue;
			}

			if (nRight >= (m_aInfo.nWidthBlock + 1) * (m_aInfo.nHeightBlock + 1))
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxRight = D3DXVECTOR3(
					(fWidthLen * nCntWidth) - ((fWidthLen * m_aInfo.nWidthBlock) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * m_aInfo.nHeightBlock) * 0.5f)));
			}
			else
			{
				VtxRight = pVtxPos[nRight];
			}

			if (nLeft >= (m_aInfo.nWidthBlock + 1) * (m_aInfo.nHeightBlock + 1))
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxLeft = D3DXVECTOR3(
					(fWidthLen * nCntWidth) - ((fWidthLen * m_aInfo.nWidthBlock) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * m_aInfo.nHeightBlock) * 0.5f)));
			}
			else
			{
				VtxLeft = pVtxPos[nLeft];
			}

			if (nNowPoint >= (m_aInfo.nWidthBlock + 1) * (m_aInfo.nHeightBlock + 1))
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxNow = D3DXVECTOR3(
					(fWidthLen * nCntWidth) - ((fWidthLen * m_aInfo.nWidthBlock) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * m_aInfo.nHeightBlock) * 0.5f)));
			}
			else
			{
				VtxNow = pVtxPos[nNowPoint];
			}

			// �x�N�g�����v�Z
			vec1 = VtxRight - VtxNow;
			vec2 = VtxLeft - VtxNow;

			// �O�ς����߂�
			D3DXVec3Cross(&nor, &vec1, &vec2);

			// �O�ς̐��K�������Ė@���ɂ���
			D3DXVec3Normalize(&nor, &nor);

			// �@��
			pVtxNor[nNowPoint] = nor;
		}
	}

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ���[�h����
//==========================================================================
HRESULT CElevation::Load(const char *pText)
{
	char aComment[mylib_const::MAX_STRING] = {};	//�R�����g�p

	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	//�t�@�C�����J��
	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_GAME:
		pFile = fopen(pText, "r");
		break;

	default:
		pFile = fopen("data\\TEXT\\elevation\\field_outgame.txt", "r");
		break;
	}

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		fclose(pFile);
		return E_FAIL;
	}

	int nCntStage = 0;	// �X�e�[�W�̃J�E���g

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		if (strcmp(&aComment[0], "TEXTURE_FILENAME") == 0)
		{// TEXTYPE���������ޓǂݍ���

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%s", &aComment[0]);	// �e�N�X�`���t�@�C����

			// �t�@�C�����ۑ�
			m_aInfo.TextureFileName = aComment;
		}

		// ���b�V���t�B�[���h�̐ݒ�
		if (strcmp(&aComment[0], "FIELDSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_FIELDSET"))
			{// END_FIELDSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &m_aInfo.pos.x);		// X���W
					fscanf(pFile, "%f", &m_aInfo.pos.y);		// Y���W
					fscanf(pFile, "%f", &m_aInfo.pos.z);		// Z���W
				}

				if (strcmp(&aComment[0], "BLOCK") == 0)
				{// BLOCK������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);			// =�̕�
					fscanf(pFile, "%d", &m_aInfo.nWidthBlock);	// ���̕�����
					fscanf(pFile, "%d", &m_aInfo.nHeightBlock);	// �c�̕�����
				}

				if (strcmp(&aComment[0], "SIZE") == 0)
				{// SIZE������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%f", &m_aInfo.fWidthLength);	// ���̒���
					fscanf(pFile, "%f", &m_aInfo.fHeightLength);	// �c�̒���
				}

				if (strcmp(&aComment[0], "VTXPOS") == 0)
				{// VTXPOS�������璸�_�ǂݍ���

					// ���_��
					int nVtxNum = (m_aInfo.nHeightBlock + 1) * (m_aInfo.nWidthBlock + 1);
					int nCntVtx = 0;

					// ���_���Ń������m��
					if (nCntStage == 0)
					{
						m_aInfo.pVtxPos = DEBUG_NEW D3DXVECTOR3[nVtxNum];
					}

					// ���_�ǂݍ��ݗp
					std::string VtxPos;

					while (1)
					{// END_VTXPOS������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", (char*)VtxPos.c_str());

						if (strcmp(VtxPos.c_str(), "END_VTXPOS") == 0)
						{// �r���I������Ă���

							for (int nCnt = nCntVtx; nCnt < nVtxNum; nCnt++)
							{
								// 0�N���A
								m_aInfo.pVtxPos[nCnt].y = 0.0f;	// ����
							}
							break;
						}

						// float�ϊ�
						if (nCntStage == 0)
						{
							m_aInfo.pVtxPos[nCntVtx].y = 0.0f;	// ����
						}
						m_aInfo.pVtxPos[nCntVtx].y = std::strtof(VtxPos.c_str(), nullptr);	// ���
						nCntVtx++;
					}
					// �X�e�[�W�����Z
					nCntStage++;
				}

			}// END_FIELDSET�̂�����
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);
	return S_OK;

}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CElevation::Save(void)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\elevation\\field_save.txt", "w");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �e�N�X�`���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// �e�N�X�`���t�@�C����
	fprintf(pFile, "TEXTURE_FILENAME = %s\t\t\n", m_aInfo.TextureFileName.c_str());
	
	// �t�B�[���h�̐ݒu
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# �t�B�[���h�̐ݒu\n"
		"#==============================================================================\n");

	float m_fWidthLen = GetWidthLen();
	float m_fHeightLen = GetHeightLen();

	fprintf(pFile,
		"FIELDSET\n"
		"\tPOS = %.2f %.2f %.2f\n"
		"\tBLOCK = %d %d\n"
		"\tSIZE = %.2f %.2f\n"
		"\tVTXPOS\n", m_aInfo.pos.x, m_aInfo.pos.y, m_aInfo.pos.z,
		m_aInfo.nWidthBlock, m_aInfo.nHeightBlock,
		m_fWidthLen, m_fHeightLen);

	// ���_�擾
	D3DXVECTOR3 *pVtxPos = GetVtxPos();

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_aInfo.nHeightBlock + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		fprintf(pFile, "\t");
		for (int nCntWidth = 0; nCntWidth < m_aInfo.nWidthBlock + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			fprintf(pFile, "%.2f ", pVtxPos[nCntWidth + (nCntHeight * (m_aInfo.nWidthBlock + 1))].y);
		}
		fprintf(pFile, "\n");
	}
	fprintf(pFile, "\nEND_VTXPOS\n");

	fprintf(pFile, "\nEND_FIELDSET\n");
	fprintf(pFile, "\nEND_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

	//�t�@�C�������
	fclose(pFile);
}

//==========================================================================
// �G�f�B�b�g�̔���
//==========================================================================
bool CElevation::IsEdit(void)
{
	return m_bEdit;
}

//==========================================================================
// �N���̏��擾
//==========================================================================
CElevation *CElevation::GetElevation(void)
{
	return this;
}
