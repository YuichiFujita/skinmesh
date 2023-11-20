//=============================================================================
// 
//  �J�����̎����� [enemybase.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "debugproc.h"
#include "enemybase.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "3D_effect.h"
#include "debugpointnumber.h"
#include "objectX.h"
#include "enemymanager.h"
#include "manager.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const char* MARKOBJ = "data\\MODEL\\koko.x";
	const char* FILE_ENEMYBASE = "data\\BIN\\enemybase.bin";
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEnemyBase::CEnemyBase()
{
	// �l�̃N���A
	memset(&m_pMultiNumber[0], NULL, sizeof(m_pMultiNumber));	// �I�u�W�F�N�gX
	memset(&m_apObjX[0], NULL, sizeof(m_apObjX));	// �I�u�W�F�N�gX
	m_nNumAll = 0;		// ����
	m_nNumStage = 0;	// �X�e�[�W�̑���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEnemyBase::~CEnemyBase()
{

}

//==========================================================================
// ��������
//==========================================================================
CEnemyBase *CEnemyBase::Create(const char *pFileName)
{
	// �����p�̃I�u�W�F�N�g
	CEnemyBase *pModel = NULL;

	if (pModel == NULL)
	{// NULL��������

		// �������̊m��
		pModel = DEBUG_NEW CEnemyBase;

		if (pModel != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pModel->ReadText(pFileName);

			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}

			// ����������
			hr = pModel->Init();
			if (FAILED(hr))
			{// ���s���Ă�����
				delete pModel;
				return NULL;
			}
		}

		return pModel;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEnemyBase::Init(void)
{
	// �G�}�l�[�W���擾
	CEnemyManager *pEnemyManager = CGame::GetEnemyManager();

	//// ��������
	//for (int i = 0; i < m_nNumAll; i++)
	//{
	//	// �f�o�b�O�p�����̐���
	//	m_pMultiNumber[i] = CDebugPointNumber::Create(i);

	//	if (m_EnemyBaseInfo[i].nRush == 0)
	//	{// ���b�V���p����Ȃ�������
	//		pEnemyManager->SetEnemy(
	//			m_EnemyBaseInfo[i].pos,
	//			mylib_const::DEFAULT_VECTOR3,
	//			m_EnemyBaseInfo[i].nPattern);
	//	}
	//}

	return S_OK;
}

//==========================================================================
// �ʒu�쐬
//==========================================================================
void CEnemyBase::CreatePos(int nStage, int nPattern, D3DXVECTOR3 pos, int nRush)
{

	if (nStage < 0)
	{
		nStage = 0;
	}

	if (nStage >= (int)m_EnemyBaseInfo.size())
	{
		nStage = (int)m_EnemyBaseInfo.size();
		m_EnemyBaseInfo.push_back(std::vector<sInfo>());

		// ���_���ǉ�
		m_nBaseNum.push_back(0);	// ���_�̐�
		m_nNumStage++;
	}

	// �ʒu����
	m_EnemyBaseInfo[nStage].push_back(sInfo());
	m_EnemyBaseInfo[nStage][m_nBaseNum[nStage]].nPattern = nPattern;	// ���
	m_EnemyBaseInfo[nStage][m_nBaseNum[nStage]].pos = pos;
	m_EnemyBaseInfo[nStage][m_nBaseNum[nStage]].nRush = nRush;

	// �ڈ󐶐�
	m_apObjX[m_nNumAll] = CObjectX::Create(MARKOBJ, mylib_const::DEFAULT_VECTOR3, mylib_const::DEFAULT_VECTOR3, false);	// �I�u�W�F�N�gX
	m_apObjX[m_nNumAll]->SetType(CObject::TYPE_BALLAST);
	m_apObjX[m_nNumAll]->SetPosition(m_EnemyBaseInfo[nStage][m_nBaseNum[nStage]].pos);

	// �������Z
	m_nNumAll++;
	m_nBaseNum[nStage]++;

}

//==========================================================================
// �ʒu�폜
//==========================================================================
void CEnemyBase::DeletePos(int nStage, int nIdx)
{
	// �\�[�g����
	CalSort(&m_EnemyBaseInfo[nStage][0], nIdx, m_nNumAll);

	// �ʒu����
	m_EnemyBaseInfo[nStage].pop_back();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �I������
//==========================================================================
void CEnemyBase::Uninit(void)
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CEnemyBase::Update(void)
{
#if _DEBUG

	int nCntObj = 0;
	for (int nCntStage = 0; nCntStage < m_nNumStage; nCntStage++)
	{
		for (int i = 0; i < m_nBaseNum[nCntStage]; i++)
		{
			if (m_apObjX[nCntObj] == NULL)
			{
				m_apObjX[nCntObj] = CObjectX::Create(MARKOBJ, mylib_const::DEFAULT_VECTOR3, mylib_const::DEFAULT_VECTOR3, false);	// �I�u�W�F�N�gX
				m_apObjX[nCntObj]->SetType(CObject::TYPE_BALLAST);
				//m_apObjX[i]->SetPositionD3DXVECTOR3(pos.x, m_apObjX[i]->GetPosition().y, pos.z);
			}

			m_apObjX[nCntObj]->SetPosition(m_EnemyBaseInfo[nCntStage][i].pos);

			if (m_pMultiNumber[nCntObj] != NULL)
			{
				m_pMultiNumber[nCntObj]->SetPosition(m_EnemyBaseInfo[nCntStage][i].pos);
			}

			nCntObj++;
		}
	}
#endif

	// �X�e�[�W�̑����ݒ�
	CGame::GetGameManager()->SetNumStage(m_nNumStage);

}

//==========================================================================
// �e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT CEnemyBase::ReadText(const char *pFileName)
{

	char aComment[MAX_COMMENT] = {};	// �R�����g�p

	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(pFileName, "r");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return E_FAIL;
	}

	// ���Z�b�g
	m_nNumAll = 0;
	m_nNumStage = 0;	// �X�e�[�W�̑���

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);


		if (strcmp(aComment, "STAGESET") == 0)
		{// STAGESET�ŃX�e�[�W���̓ǂݍ��݊J�n

			// �Ō���ɐ���
			m_EnemyBaseInfo.push_back(std::vector<sInfo>());

			// ���_���ǉ�
			m_nBaseNum.push_back(0);	// ���_�̐�

			while (strcmp(aComment, "END_STAGESET") != 0)
			{// END_STAGESET������܂ŌJ��Ԃ�

				// ������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				if (strcmp(aComment, "BASESET") == 0)
				{// BASESET�œG���_�̓ǂݍ��݊J�n

					// �Ō���ɐ���
					m_EnemyBaseInfo[m_nNumStage].push_back(sInfo());

					int nCntBase = m_nBaseNum[m_nNumStage];

					while (strcmp(aComment, "END_BASESET") != 0)
					{// END_BASESET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	// �m�F����

						if (strcmp(aComment, "PATTERN") == 0)
						{// PATTERN��������G�̎�ޓǂݍ���

							fscanf(pFile, "%s", &aComment[0]);	// =�̕�
							fscanf(pFile, "%d", &m_EnemyBaseInfo[m_nNumStage][nCntBase].nPattern);	// �L�����t�@�C���ԍ�
						}

						if (strcmp(aComment, "POS") == 0)
						{// POS��������ʒu�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_EnemyBaseInfo[m_nNumStage][nCntBase].pos.x);	// X���W
							fscanf(pFile, "%f", &m_EnemyBaseInfo[m_nNumStage][nCntBase].pos.y);	// Y���W
							fscanf(pFile, "%f", &m_EnemyBaseInfo[m_nNumStage][nCntBase].pos.z);	// Z���W
						}

						if (strcmp(aComment, "ROT") == 0)
						{// ROT��������ʒu�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_EnemyBaseInfo[m_nNumStage][nCntBase].rot.x);	// X
							fscanf(pFile, "%f", &m_EnemyBaseInfo[m_nNumStage][nCntBase].rot.y);	// Y
							fscanf(pFile, "%f", &m_EnemyBaseInfo[m_nNumStage][nCntBase].rot.z);	// Z
						}

						if (strcmp(aComment, "RUSH") == 0)
						{// RUSH�������烉�b�V���p���ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);	// =�̕�
							fscanf(pFile, "%d", &m_EnemyBaseInfo[m_nNumStage][nCntBase].nRush);	// ���b�V���p
						}

					}// END_BASESET�̂�����

					// �G�̋��_�����Z
					m_nNumAll++;
					m_nBaseNum[m_nNumStage]++;
				}


			}// END_STAGESET�̂�����

			// �X�e�[�W�̑������Z
			m_nNumStage++;
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	return S_OK;
}

//==========================================================================
// �O���t�@�C�������o������
//==========================================================================
void CEnemyBase::Save(void)
{

	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\enemybase.txt", "w");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return;
	}

	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# �G���_�̔z�u\n"
		"#==============================================================================");

	for (int nCntStage = 0; nCntStage < m_nNumStage; nCntStage++)
	{
		// �o��
		fprintf(pFile,
			"\n"
			"#--------------------------------[Stage%d]-----------------------------\n"
			"STAGESET\n", nCntStage);

		for (int i = 0; i < m_nBaseNum[nCntStage]; i++)
		{
			// �o��
			fprintf(pFile,
				"\tBASESET\n"
				"\t\tPATTERN = %d\n"
				"\t\tPOS = %.2f %.2f %.2f\n"
				"\t\tRUSH = %d\n"
				"\tEND_BASESET\n\n",
				m_EnemyBaseInfo[nCntStage][i].nPattern,
				m_EnemyBaseInfo[nCntStage][i].pos.x, m_EnemyBaseInfo[nCntStage][i].pos.y, m_EnemyBaseInfo[nCntStage][i].pos.z,
				m_EnemyBaseInfo[nCntStage][i].nRush);
		}

		// �o��
		fprintf(pFile,
			"END_STAGESET\n");
	}

	fprintf(pFile, "\nEND_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

	//�t�@�C�������
	fclose(pFile);
}

//==========================================================================
// �����擾
//==========================================================================
int CEnemyBase::GetSpawnPointNum(void)
{
	return m_EnemyBaseInfo.size() - 1;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CEnemyBase::GetSpawnPoint(int nStage, int nIdx)
{
	if (nIdx < 0)
	{
		nIdx = 0;
	}

	if (nIdx >= (int)m_EnemyBaseInfo[nStage].size() ||
		nStage >= (int)m_EnemyBaseInfo.size())
	{// �v�f���𒴂��Ă�����

		return mylib_const::DEFAULT_VECTOR3;
	}

	return m_EnemyBaseInfo[nStage][nIdx].pos;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CEnemyBase::SetSpawnPoint(int nStage, int nIdx, D3DXVECTOR3 pos)
{
	if (nIdx < 0)
	{
		return;
	}

	// ���n��
	m_EnemyBaseInfo[nStage][nIdx].pos = pos;
}

//==========================================================================
// �ύX�̏��擾
//==========================================================================
CEnemyBase::sInfo CEnemyBase::GetEnemyBaseInfo(int nStage, int nIdx)
{
	sInfo InitInfo;
	memset(&InitInfo, NULL, sizeof(InitInfo));

	if (m_EnemyBaseInfo.size() <= 0 || (int)m_EnemyBaseInfo[nStage].size() <= nIdx || (int)m_EnemyBaseInfo.size() <= nStage)
	{// �T�C�Y����
		return InitInfo;
	}

	return m_EnemyBaseInfo[nStage][nIdx];
}
