//=============================================================================
// 
//  ���f������ [motion.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "model.h"
#include "objectChara.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
std::vector<std::string> CMotion::m_sTextFile = {};		// �e�L�X�g�t�@�C����
std::vector<CMotion::Info> CMotion::m_aLoadData[MAX_MOTION] = {};	// ���[�V�����̏��
std::vector<CMotion::AttackInfo> CMotion::m_aLoadAttackData[MAX_MOTION][MAX_PARTS] = {};	// �U���̓ǂݍ��ݏ��
int CMotion::m_nNumLoad = 0;								// �ǂݍ��񂾐�
std::vector<int> CMotion::m_nNumLoadData = {};				// ���[�V�������̃f�[�^��

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMotion::CMotion()
{
	// �l�̃N���A
	m_nNumAll = 0;			// ���[�V�����̑���
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_nOldType = 0;			// �O��̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
	m_fCntAllFrame = 0.0f;	// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
	m_fSlowFactor = 0.0f;	// �x���W��
	m_bFinish = false;		// �I���������ǂ���
	m_pObjChara = NULL;		// �I�u�W�F�N�g�̃|�C���^
	m_ppModel = NULL;		// ���f���̃|�C���^
	m_nNumModel = 0;		// ���f���̑���
	m_nNumMotion = 0;		// ���[�V�����̑���

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		m_aInfo[nCntMotion].nNumKey = 0;			// �L�[�̐�
		m_aInfo[nCntMotion].nLoop = 0;				// ���[�v����
		m_aInfo[nCntMotion].nMove = 0;				// �ړ�����
		m_aInfo[nCntMotion].nNumAttackInfo = 0;		// �U�����̐�

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aInfo[nCntMotion].aKey[nCntKey].nFrame = 0;

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				m_aInfo[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
				m_aInfo[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
				m_aInfo[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
				m_aInfo[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̈ʒu
				m_aInfo[nCntMotion].aKey[nCntKey].aParts[nCntParts].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̌��_
			}
		}

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			m_aInfo[nCntMotion].AttackInfo[nCntParts] = NULL;	// �U�����
		}
	}

	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		// �ߋ��̏��
		aPartsOld[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		aPartsOld[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
		aPartsOld[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		aPartsOld[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̈ʒu
		aPartsOld[nCntParts].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̌��_
	}

	m_nNumAll = 0;			// ���[�V�����̑���
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_nOldType = 0;			// �O��̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fSlowFactor = 0.0f;	// �x���W��
	m_bFinish = false;		// �I���������ǂ���
	m_pObjChara = NULL;		// �I�u�W�F�N�g�̃|�C���^
	m_ppModel = NULL;		// ���f���̃|�C���^
	m_nNumModel = 0;		// ���f���̑���
	m_nNumMotion = 0;		// ���[�V�����̑���

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMotion::~CMotion()
{

}


//==========================================================================
// ��������
//==========================================================================
CMotion *CMotion::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CMotion *pMotion = NULL;

	if (pMotion == NULL)
	{// NULL��������

		// �������̊m��
		pMotion = DEBUG_NEW CMotion;

		if (pMotion != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pMotion->Init();
		}

		return pMotion;
	}

	return NULL;
}


//==========================================================================
// ��������
//==========================================================================
CMotion *CMotion::Create(const std::string pTextFile)
{
	// �����p�̃I�u�W�F�N�g
	CMotion *pMotion = NULL;

	if (pMotion == NULL)
	{// NULL��������

		// �������̊m��
		pMotion = DEBUG_NEW CMotion;

		if (pMotion != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pMotion->Init();
			pMotion->ReadText(pTextFile);
		}

		return pMotion;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMotion::Init(void)
{
	m_nNumAll = 0;			// ���[�V�����̑���
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
	m_fCntAllFrame = 0.0f;	// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
	m_bFinish = true;		// �I���������ǂ���
	m_ppModel = NULL;		// ���f���̃|�C���^
	m_nNumModel = 0;		// ���f���̑���
	m_fSlowFactor = 1.0f;	// �x���W��

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMotion::Uninit(void)
{
	// �U�����̔j��
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		for (int nCntAttack = 0; nCntAttack < MAX_PARTS; nCntAttack++)
		{
			if (m_aInfo[nCntMotion].AttackInfo[nCntAttack] != NULL)
			{
				delete m_aInfo[nCntMotion].AttackInfo[nCntAttack];
				m_aInfo[nCntMotion].AttackInfo[nCntAttack] = NULL;
			}
		}
	}
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfo(Info info)
{
	// ����n��
	//m_aInfo[m_nNumAll] = info;\

	// �R�s�[
	memcpy(m_aInfo[m_nNumAll].aKey, info.aKey, sizeof(info.aKey));

	m_aInfo[m_nNumAll].nLoop = info.nLoop;
	m_aInfo[m_nNumAll].nMove = info.nMove;
	m_aInfo[m_nNumAll].nNumKey = info.nNumKey;

	for (int nCntKey = 0; nCntKey < m_aInfo[m_nNumAll].nNumKey; nCntKey++)
	{
		int nBeforeCnt = nCntKey - 1;
		if (nBeforeCnt <= 0)
		{
			nBeforeCnt = 0;
		}

		float PosX = m_aInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.x;
		float PosZ = m_aInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.z;

		// ���������߂�
		m_aInfo[m_nNumAll].aKey[nCntKey].fRotMove = atan2f(
			(PosX - m_aInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.x),
			(PosZ - m_aInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.z));
	}

	// ���[�V�����̑������Z
	m_nNumAll++;
}

//==========================================================================
// �U�����̓o�^
//==========================================================================
void CMotion::SetAttackInfo(AttackInfo info)
{
	//// �������m��
	m_aInfo[m_nNumAll].AttackInfo[m_aInfo[m_nNumAll].nNumAttackInfo] = DEBUG_NEW AttackInfo;

	if (m_aInfo[m_nNumAll].AttackInfo[m_aInfo[m_nNumAll].nNumAttackInfo] != NULL)
	{// NULL����Ȃ����

		// ���n��
		*m_aInfo[m_nNumAll].AttackInfo[m_aInfo[m_nNumAll].nNumAttackInfo] = info;

		// �U�����̑������Z
		m_aInfo[m_nNumAll].nNumAttackInfo++;
	}
}

//==========================================================================
// ���[�V���������郂�f���̓o�^
//==========================================================================
void CMotion::SetModel(CModel **pModel, int nNumModel, CObjectChara *pObjChara)
{
	// ���f���̃|�C���^��n��
	m_ppModel = pModel;

	// ���f���̑���
	m_nNumModel = nNumModel;

	// �I�u�W�F�N�g�̃|�C���^��n��
	m_pObjChara = pObjChara;
}

//==========================================================================
// �|�[�Y�̃��Z�b�g
//==========================================================================
void CMotion::ResetPose(int nType)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		if (m_ppModel[nCntParts] == NULL)
		{// NULL��������
			continue;
		}

		// �����ݒ�
		m_ppModel[nCntParts]->SetRotation(m_aInfo[nType].aKey[0].aParts[nCntParts].rot);
		aPartsOld[nCntParts].rot = m_aInfo[nType].aKey[0].aParts[nCntParts].rot;
	}
	aPartsOld[0].pos = m_pObjChara->GetOriginPosition() + m_aInfo[nType].aKey[0].aParts[0].pos;
}

//==========================================================================
// �X�V����
//==========================================================================
void CMotion::Update(float fBuff)
{
	if (m_bFinish == true && m_aInfo[m_nType].nLoop == LOOP_OFF)
	{// �I�����Ă� && ���[�vOFF��������
		return;
	}

	// �Đ��t���[��
	int nFrame;

	// �Đ��t���[���ۑ�
	nFrame = m_aInfo[m_nType].aKey[m_nPatternKey].nFrame;

	if (m_nType == 0 && m_nOldType != 0 && m_nPatternKey == 0)
	{// �j���[�g������0�̎�
		nFrame = 10;
	}

	// ���̃L�[
	int nNextKey = (m_nPatternKey + 1) % m_aInfo[m_nType].nNumKey;

	if (nNextKey == 0 && m_aInfo[m_nType].nLoop == LOOP_OFF)
	{// ���[�v���Ȃ��Ƃ�

		// �Ō�ŌŒ�
		nNextKey = m_aInfo[m_nType].nNumKey - 1;
	}

	int nStartIdx = m_pObjChara->GetMotionStartIdx();
	for (int nCntParts = nStartIdx; nCntParts < m_nNumModel + nStartIdx + 1; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		int nCntModel = nCntParts;
		if (nStartIdx != 0)
		{
			nCntModel = nCntParts - nStartIdx;
		}

		if (nCntModel >= m_nNumModel)
		{
			break;
		}

		if (m_ppModel[nCntModel] == NULL)
		{// NULL��������
			continue;
		}

		// ���ƍ��̌����̍����擾
		float rotDiffX = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.x -
			aPartsOld[nCntParts].rot.x;

		float rotDiffY = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.y -
			aPartsOld[nCntParts].rot.y;

		float rotDiffZ = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.z -
			aPartsOld[nCntParts].rot.z;

		// �p�x�̐��K��
		RotNormalize(rotDiffX);
		RotNormalize(rotDiffY);
		RotNormalize(rotDiffZ);

		// �p�[�c�̌�����ݒ�
		D3DXVECTOR3 rot = mylib_const::DEFAULT_VECTOR3;

		// �p�[�c�̌�����ݒ�
		rot.x =
			aPartsOld[nCntParts].rot.x +
			rotDiffX *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// �p�[�c�̌�����ݒ�
		rot.y =
			aPartsOld[nCntParts].rot.y +
			rotDiffY *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// �p�[�c�̌�����ݒ�
		rot.z =
			aPartsOld[nCntParts].rot.z +
			rotDiffZ *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// �p�x�̐��K��
		RotNormalize(rot.x);
		RotNormalize(rot.y);
		RotNormalize(rot.z);

		// �����ݒ�
		m_ppModel[nCntModel]->SetRotation(rot);

		// �p�[�c�̈ʒu��ݒ�
		if (nCntParts == 0)
		{
			// �{�̂̈ʒu�擾
			D3DXVECTOR3 pos = m_pObjChara->GetPosition();

			// �{�̂̌����擾
			D3DXVECTOR3 rot = m_pObjChara->GetRotation();

			// ���̈ʒu�擾
			D3DXVECTOR3 posOrigin = m_pObjChara->GetOriginPosition();

			// �p�[�c�̈ʒu�擾
			D3DXVECTOR3 posParts = m_ppModel[nCntModel]->GetPosition();
			D3DXVECTOR3 posPartsOld = m_ppModel[nCntModel]->GetPosition();

			// �ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x -
				aPartsOld[nCntParts].pos.x;

			float posDiffY = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.y -
				aPartsOld[nCntParts].pos.y;

			float posDiffZ = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z -
				aPartsOld[nCntParts].pos.z;

			// �e��Y��␳
			posParts.y =
				aPartsOld[nCntParts].pos.y +
				posDiffY *
				(
				m_fCntFrame /
				(float)nFrame);

			/*posParts.x =
				aPartsOld[nCntParts].pos.x +
				posDiffX *
				(
				m_fCntFrame /
				(float)nFrame);*/

			/*posParts.z =
				aPartsOld[nCntParts].pos.z +
				posDiffZ *
				(
				m_fCntFrame /
				(float)nFrame);*/

			// �ʒu�ݒ�
			m_ppModel[nCntModel]->SetPosition(posParts + posOrigin);

			// ���������������߂�
			float fMoveDiff =
				sqrtf((m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x - aPartsOld[nCntParts].pos.x) * (m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x - aPartsOld[nCntParts].pos.x)
					+ (m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z - aPartsOld[nCntParts].pos.z) * (m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z - aPartsOld[nCntParts].pos.z));
			fMoveDiff /= (float)nFrame;

			// �����̌������ꎞ���
			float fRot = m_aInfo[m_nType].aKey[nNextKey].fRotMove;

			// �����̌��������ֈړ�
			pos.x += sinf(D3DX_PI + fRot + rot.y) * fMoveDiff;
			pos.z += cosf(D3DX_PI + fRot + rot.y) * fMoveDiff;

			m_pObjChara->SetPosition(pos);
		}
	}

	// �t���[���̃J�E���g�����Z
	m_fCntFrame += 1.0f * fBuff;
	m_fCntAllFrame += 1.0f * fBuff;

	if (m_aInfo[m_nType].nLoop == LOOP_ON)
	{// ���[�v���[�V�����͂��ł��I����Ă�

		// �I������ON
		m_bFinish = true;
	}

	if (m_fCntFrame >= (float)nFrame)
	{// �t���[���̃J�E���g���t���[�����ɒB������

		// �t���[���̃J�E���g���[���ɖ߂�
		m_fCntFrame = 0;

		m_nOldType = m_nType;	// �O��̃��[�V�����̎��

		// �p�^�[��NO.�X�V
		m_nPatternKey = (m_nPatternKey + 1) % m_aInfo[m_nType].nNumKey;


		int nStartIdx = m_pObjChara->GetMotionStartIdx();
		for (int nCntParts = nStartIdx; nCntParts < m_nNumModel + nStartIdx + 1; nCntParts++)
		{// �S�p�[�c���J��Ԃ�
			aPartsOld[nCntParts].rot = m_aInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].rot;
			aPartsOld[nCntParts].pos = m_aInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].pos;
		}

		if (m_nPatternKey == 0)
		{// �L�[���[���̎�

			// ���t���[�������Z�b�g
			m_fCntAllFrame = 0.0f;

			//���[�V�����̐ݒ�
			if (m_aInfo[m_nType].nLoop == LOOP_OFF)
			{// ���[�v���[�V��������Ȃ���

				// �I������ON
				m_bFinish = true;
			}
		}
	}
}

//==========================================================================
// ���[�V�����̐ݒ菈��
//==========================================================================
void CMotion::Set(int nType, bool bBlend)
{
	if (nType == 15)
	{
		int n = 0;
	}

	if (nType > m_nNumAll)
	{// �����𒴂��Ă�����
		return;
	}

	if (m_nType != nType)
	{// ���݂̎�ނƓ�������Ȃ�������

		m_nOldType = m_nType;	// �O��̃��[�V�����̎��
		m_nType = nType;		// ��ސݒ�
		m_nPatternKey = 0;		// ���ڂ̃L�[��
		m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
		m_fCntAllFrame = 0.0f;	// �S�ẴJ�E���g
		m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
		m_bFinish = false;		// �I���������ǂ���

		for (int nCntKey = 0; nCntKey < m_aInfo[m_nPatternKey].nNumKey; nCntKey++)
		{
			m_fMaxAllFrame += m_aInfo[m_nPatternKey].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
		}

		int nStartIdx = m_pObjChara->GetMotionStartIdx();
		for (int nCntParts = nStartIdx; nCntParts < m_nNumModel + nStartIdx + 1; nCntParts++)
		{// �S�p�[�c���J��Ԃ�

			int nCntModel = nCntParts;
			if (nStartIdx != 0)
			{
				nCntModel = nCntParts - nStartIdx;
			}

			if (nCntModel >= m_nNumModel)
			{
				break;
			}

			if (m_ppModel[nCntModel] == NULL)
			{// NULL��������
				continue;
			}

			// �ߋ��̈ʒu�E������ۑ�
			if (bBlend == true)
			{
				aPartsOld[nCntParts].rot = m_ppModel[nCntModel]->GetRotation();
				aPartsOld[nCntParts].pos = m_ppModel[nCntModel]->GetPosition() - m_pObjChara->GetOriginPosition();
			}
			else
			{
				aPartsOld[nCntParts].rot = m_aInfo[m_nType].aKey[0].aParts[nCntParts].rot;
				aPartsOld[nCntParts].pos = m_aInfo[m_nType].aKey[0].aParts[nCntParts].pos + m_pObjChara->GetOriginPosition();
			}


		}
	}
	else
	{
		int n = 0;
	}
}

//==========================================================================
// ���݂̃��[�V�����^�C�v�擾
//==========================================================================
int CMotion::GetType(void)
{
	return m_nType;
}

//==========================================================================
// �O��̃��[�V�����^�C�v�擾
//==========================================================================
int CMotion::GetOldType(void)
{
	return m_nOldType;
}

//==========================================================================
// �I�����Ă��邩�̔���
//==========================================================================
bool CMotion::IsFinish(void)
{
	return m_bFinish;
}

//==========================================================================
// �I�����Ă��邩�̔���؂�ւ�
//==========================================================================
void CMotion::ToggleFinish(bool bFinish)
{
	m_bFinish = bFinish;
}

//==========================================================================
// �x���W���̐ݒ�
//==========================================================================
void CMotion::SetSlowFactor(float fFactor)
{
	m_fSlowFactor = fFactor;
}

//==========================================================================
// �ړ��̔���擾
//==========================================================================
int CMotion::IsGetMove(int nType)
{
	return m_aInfo[nType].nMove;
}

//==========================================================================
// ���[�V�������̎擾
//==========================================================================
CMotion::Info CMotion::GetInfo(int nType)
{
	return m_aInfo[nType];
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfoSave(int nType, int nKey, int nParts, D3DXVECTOR3 rot)
{
	m_aInfo[nType].aKey[nKey].aParts[nParts].rot = rot;
}

//==========================================================================
// �L�[�����ύX
//==========================================================================
void CMotion::ChangeKeyNum(int nType, int nNum)
{
	m_aInfo[nType].nNumKey = nNum;
}

//==========================================================================
// �L�[�t���[���ύX
//==========================================================================
void CMotion::ChangeKeyFrame(int nType, int nKey, int nNum)
{
	m_aInfo[nType].aKey[nKey].nFrame = nNum;
}

//==========================================================================
// �U�����ύX
//==========================================================================
void CMotion::ChangeAttackInfo(int nType, int nIdx, AttackInfo info)
{
	*m_aInfo[nType].AttackInfo[nIdx] = info;
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
void CMotion::AddNumAttackInfo(int nType)
{
	if (m_aInfo[nType].nNumAttackInfo >= MAX_PARTS)
	{
		return;
	}

	// �������m��
	m_aInfo[nType].AttackInfo[m_aInfo[nType].nNumAttackInfo] = DEBUG_NEW AttackInfo;

	if (m_aInfo[nType].AttackInfo[m_aInfo[nType].nNumAttackInfo] != NULL)
	{// NULL����Ȃ����

		// ������
		memset(m_aInfo[nType].AttackInfo[m_aInfo[nType].nNumAttackInfo], 0, sizeof(AttackInfo));

		// �U�����̑������Z
		m_aInfo[nType].nNumAttackInfo++;
	}
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
void CMotion::SubNumAttackInfo(int nType)
{
	if (m_aInfo[nType].nNumAttackInfo <= 1)
	{
		return;
	}
	int nIdx = m_aInfo[nType].nNumAttackInfo - 1;

	// ���������
	//if (m_aInfo[nType].AttackInfo[nIdx] != NULL)
	{
		delete m_aInfo[nType].AttackInfo[nIdx];
		m_aInfo[nType].AttackInfo[nIdx] = NULL;

		// �U�����̑������Z
		m_aInfo[nType].nNumAttackInfo--;
	}
}

//==========================================================================
// �U���̈ʒu�擾
//==========================================================================
D3DXVECTOR3 CMotion::GetAttackPosition(CModel **ppModel, AttackInfo attackInfo)
{
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	if (ppModel[attackInfo.nCollisionNum] == NULL)
	{// NULL��������
		return mylib_const::DEFAULT_VECTOR3;
	}

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	D3DXMATRIX mtxWepon = ppModel[attackInfo.nCollisionNum]->GetWorldMtx();

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, attackInfo.Offset.x, attackInfo.Offset.y, attackInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	return D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);
}

//==========================================================================
// �t���[���J�E���g�ݒ�
//==========================================================================
void CMotion::SetFrameCount(float fCnt)
{
	m_fCntFrame = fCnt;
}

//==========================================================================
// �J�E���g�擾
//==========================================================================
float CMotion::GetAllCount(void)
{
	return m_fCntAllFrame;
}

//==========================================================================
// �S�ẴJ�E���g�擾
//==========================================================================
int CMotion::GetMaxAllCount(int nType)
{
	int nAllFrame = 0;
	for (int nCntKey = 0; nCntKey < m_aInfo[nType].nNumKey; nCntKey++)
	{
		nAllFrame += m_aInfo[nType].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
	}
	return nAllFrame;
}

//==========================================================================
// �O���t�@�C���ǂݍ���
//==========================================================================
void CMotion::ReadText(const std::string pTextFile)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	char aComment[MAX_COMMENT];	// �R�����g�p
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0;
	Info InitInfo = {};
	AttackInfo InitAttackInfo = {};

	// �ǂݍ��݊m�F
	for (int nCntData = 0; nCntData < m_nNumLoad; nCntData++)
	{
		if (m_sTextFile[nCntData] != pTextFile)
		{// �t�@�C��������v���ĂȂ�
			continue;
		}

		for (int nCntInfo = 0; nCntInfo < m_nNumLoadData[nCntData]; nCntInfo++)
		{
			for (int nCntATK = 0; nCntATK < m_aLoadData[nCntData][nCntInfo].nNumAttackInfo; nCntATK++)
			{// �U����񕪌J��Ԃ�

				// �U�����o�^
				SetAttackInfo(m_aLoadAttackData[nCntData][nCntInfo][nCntATK]);
			}

			// ���o�^
			SetInfo(m_aLoadData[nCntData][nCntInfo]);
		}
		return;
	}

	// �t�@�C�����ۑ�
	m_sTextFile.push_back(pTextFile);

	// �t�@�C�����J��
	pFile = fopen(pTextFile.c_str(), "r");

	if (pFile == NULL)
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �ǂݍ��݂̃f�[�^�����Z
	m_nNumLoadData.push_back(0);

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		//==================================================
		// �e���[�V�����̐ݒ�
		//==================================================
		if (strcmp(aComment, "MOTIONSET") == 0)
		{// ���[�V�������̓ǂݍ��݂��J�n

			// ������
			m_aLoadData[m_nNumLoad].push_back(InitInfo);

			while (strcmp(aComment, "END_MOTIONSET") != 0)
			{// END_MOTIONSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(aComment, "LOOP") == 0)
				{// LOOP�������烋�[�vON/OFF�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nCntMotion].nLoop);	// ���[�v0��1
				}

				if (strcmp(aComment, "NUM_KEY") == 0)
				{// NUM_KEY��������L�[���ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nCntMotion].nNumKey);	// �L�[��
				}

				if (strcmp(aComment, "MOVE") == 0)
				{// MOVE��������ړ�ON/OFF�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nCntMotion].nMove);	// �ړ�0��1��
				}

				if (strcmp(aComment, "ATTACKINFO") == 0)
				{// ATTACKINFO��������

					// ������
					m_aLoadAttackData[m_nNumLoad][nCntMotion].push_back(InitAttackInfo);

					int nNowAttackNum = m_aLoadData[m_nNumLoad][nCntMotion].nNumAttackInfo;

					while (strcmp(aComment, "END_ATTACKINFO") != 0)
					{// END_ATTACKINFO������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	// �m�F����

						if (strcmp(aComment, "COLLISIONPARTS") == 0)
						{// COLLISIONPARTS��������p�[�c�ԍ��ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].nCollisionNum);	// ��������p�[�c�ԍ�
						}

						if (strcmp(aComment, "ATTACKRANGE") == 0)
						{// ATTACKRANGE��������U���T�C�Y�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].fRangeSize);	// ����̃T�C�Y
						}

						if (strcmp(aComment, "OFFSET") == 0)
						{// OFFSET��������I�t�Z�b�g�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].Offset.x);	// X���W
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].Offset.y);	// Y���W
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].Offset.z);	// Z���W
						}

						if (strcmp(aComment, "ATTACKCOUNT") == 0)
						{// ATTACKCOUNT��������U���J�E���g�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].nMinCnt);	// ����̍ŏ��J�E���g
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].nMaxCnt);	// ����̍ő�J�E���g
						}

						if (strcmp(aComment, "INPACTCOUNT") == 0)
						{// INPACTCOUNT��������Ռ��J�E���g�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].nInpactCnt);	// �Ռ��̃J�E���g
						}

						if (strcmp(aComment, "DAMAGE") == 0)
						{// DAMAGE��������U���͓ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum].nDamage);	// �U����
						}
					}

					// �U�����̓o�^
					SetAttackInfo(m_aLoadAttackData[m_nNumLoad][nCntMotion][nNowAttackNum]);

					// �U�����̐����Z
					m_aLoadData[m_nNumLoad][nCntMotion].nNumAttackInfo++;
				}

				//==================================================
				//�e�L�[�̐ݒ�
				//==================================================
				if (strcmp(aComment, "KEYSET") == 0)
				{// KEYSET�ŃL�[���̓ǂݍ��݊J�n

					while (strcmp(aComment, "END_KEYSET") != 0)
					{// END_KEYSET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	// �m�F����

						if (strcmp(aComment, "FRAME") == 0)
						{// FRAME��������Đ��t���[�����ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nCntMotion].aKey[nCntKey].nFrame);	// �Đ��t���[��
						}

						//==================================================
						// �p�[�c�̐ݒ�
						//==================================================
						if (strcmp(aComment, "PARTS") == 0)
						{// PARTS�Ńp�[�c���̓ǂݍ��݊J�n

							while (strcmp(aComment, "END_PARTS") != 0)
							{// END_PARTS������܂ŌJ��Ԃ�

								fscanf(pFile, "%s", &aComment[0]);	// �m�F����

								if (strcmp(aComment, "POS") == 0)
								{// POS��������ʒu�ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		// =�̕�
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	// X���W
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	// Y���W
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	// Z���W
								}

								if (strcmp(aComment, "ROT") == 0)
								{// ROT��������p�x�ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		// =�̕�
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	// X�p�x
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	// Y�p�x
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	// Z�p�x
								}
							}// END_PARTS�̂�����

							nCntParts++;	//�p�[�c�̃J�E���g���Z
						}
					}// END_KEYSET�̂�����

					nCntParts = 0;	// �p�[�c�̃J�E���g���Z�b�g
					nCntKey++;	// �L�[�̃J�E���g���Z
				}

			}// END_MOTIONSET�̂�����

			// ���[�V�������̓o�^
			SetInfo(m_aLoadData[m_nNumLoad][nCntMotion]);

			if (m_nNumMotion == 0)
			{
				m_fMaxAllFrame = 0;
				for (int nCntKey = 0; nCntKey < m_aInfo[m_nPatternKey].nNumKey; nCntKey++)
				{
					m_fMaxAllFrame += m_aInfo[m_nPatternKey].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
				}
			}

			nCntKey = 0;	// �L�[�̃J�E���g���Z�b�g
			nCntMotion++;	// ���[�V�����̃J�E���g���Z
			m_nNumMotion++;	// ���[�V�����̃J�E���g���Z

			// ���[�V�������̃f�[�^��
			m_nNumLoadData[m_nNumLoad]++;
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	// �ǂݍ��񂾐����Z
	m_nNumLoad++;
}
