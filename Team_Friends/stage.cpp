//=============================================================================
// 
//  �X�e�[�W���� [stage.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stage.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "objectX.h"
#include "shadow.h"
#include "input.h"
#include "injectiontable.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStage::CStage()
{
	// �[���N���A
	m_nNumAll = 0;
	memset(&m_pObjX[0], NULL, sizeof(m_pObjX));	// �I�u�W�F�N�gX�̃|�C���^
	m_pInjectionTable = NULL;			// �ˏo��̃I�u�W�F�N�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStage::~CStage()
{

}

//==========================================================================
// ��������
//==========================================================================
CStage *CStage::Create(const char *pTextFile)
{
	// �����p�̃I�u�W�F�N�g
	CStage *pObjectX = NULL;

	if (pObjectX == NULL)
	{// NULL��������

		// �������̊m��
		pObjectX = DEBUG_NEW CStage;

		if (pObjectX != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjectX->Init();
			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}

			// �O���e�L�X�g�ǂݍ��ݏ���
			hr = pObjectX->ReadText(pTextFile);
			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pObjectX;
	}

	return NULL;
}


//==========================================================================
// ����������
//==========================================================================
HRESULT CStage::Init(void)
{
	// ����
	m_nNumAll = 0;

	// �ˏo�䐶��
	m_pInjectionTable = CInjectionTable::Create(D3DXVECTOR3(0.0f, 0.0f, -2000.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStage::Uninit(void)
{
	// �I������
	for (int nCntObj = 0; nCntObj < mylib_const::MAX_STAGE; nCntObj++)
	{
		if (m_pObjX[nCntObj] != NULL)
		{// NULL����Ȃ�������
			m_pObjX[nCntObj] = NULL;
		}
	}
}

//==========================================================================
// �������
//==========================================================================
void CStage::Release(void)
{
	// �I������
	for (int nCntObj = 0; nCntObj < mylib_const::MAX_STAGE; nCntObj++)
	{
		if (m_pObjX[nCntObj] != NULL)
		{// NULL����Ȃ�������
			m_pObjX[nCntObj]->Uninit();
			m_pObjX[nCntObj] = NULL;
		}
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CStage::Update(void)
{
	//for (int nCntObj = 0; nCntObj < mylib_const::MAX_STAGE; nCntObj++)
	//{
	//	if (m_pObjX[nCntObj] == NULL)
	//	{// NULL��������
	//		continue;
	//	}

	//}

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	for (int nCntObj = 0; nCntObj < mylib_const::MAX_STAGE; nCntObj++)
	{
		if (m_pObjX[nCntObj] == NULL)
		{
			continue;
		}

		D3DXVECTOR3 pos = m_pObjX[nCntObj]->GetPosition();

		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputGamepad->GetStickMoveR(0).y > 0)
		{//���L�[�������ꂽ,���ړ�
			pos.y += 10.0f;
		}
		if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputGamepad->GetStickMoveR(0).y < 0)
		{//���L�[�������ꂽ,���ړ�
			pos.y -= 10.0f;
		}

		m_pObjX[nCntObj]->SetPosition(pos);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStage::Draw(void)
{

}

//==========================================================================
// �O���t�@�C���������ݏ���
//==========================================================================
HRESULT CStage::SaveText(void)
{
#if TOPCUR
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\stage_info.txt", "w");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return E_FAIL;
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# ���f����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", m_nNumAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// ���f���t�@�C������
	for (int nCntFileNum = 0; nCntFileNum < m_nNumAll; nCntFileNum++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &ModelFile[nCntFileNum][0], nCntFileNum);
	}


	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ���f���̔z�u\n"
		"#==============================================================================\n");

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		// �擪��ۑ�
		CObject *pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{// NULL������܂Ŗ������[�v

			// ���̃I�u�W�F�N�g���ꎞ�ۑ�
			CObject *pObjNext = pObj->GetNext();

			// ��ނ̎擾
			CObject::TYPE TargetType = pObj->GetType();

			if (TargetType == CObject::TYPE_XFILE)
			{// X�t�@�C���̃��f����������

				// X�t�@�C���̏��擾
				CObjectX *pObjX = pObj->GetObjectX();

				int nType = pObjX->GetIdxXFile();		// ���
				D3DXVECTOR3 pos = pObjX->GetPosition();	// �ʒu
				D3DXVECTOR3 rot = pObjX->GetRotation();	// ����
				int nShadow = 0;						// �e�g�����ǂ���

				if (pObjX->GetUseShadow() == true)
				{// �g���Ă���ꍇ
					nShadow = 1;
				}

				// �o��
				fprintf(pFile,
					"MODELSET\n"
					"\tTYPE = %d\n"
					"\tPOS = %.2f %.2f %.2f\n"
					"\tROT = %.2f %.2f %.2f\n"
					"\tSHADOW = %d\n"
					"END_MODELSET\n\n",
					nType, pos.x, pos.y, pos.z,
					rot.x, rot.y, rot.z, nShadow);
			}

			// ���̃I�u�W�F�N�g����
			pObj = pObjNext;
		}
	}

	fprintf(pFile, "\nEND_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

	//�t�@�C�������
	fclose(pFile);
#endif
	return S_OK;
}

//==========================================================================
// ���f���ǂݍ��ݏ���
//==========================================================================
HRESULT CStage::ReadXFile(const char *pTextFile)
{
	// ���Z�b�g
	int nNumFileAll = 0;

	char aComment[MAX_COMMENT] = {};	// �R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �t�@�C���|�C���^
	FILE *pFile = NULL;

	//�t�@�C�����J��
	pFile = fopen(pTextFile, "r");

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &nFileNum);	// ���f����
		}

		while (nNumFileAll != nFileNum)
		{// ���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			// ���f�����̐ݒ�
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �Ō���ɒǉ�
				ModelFile.push_back(&aComment[0]);

				// X�t�@�C���̃f�[�^�擾
				CXLoad *pXLoad = CScene::GetXLoad();

				// �t�@�C���ǂݍ���
				pXLoad->XLoad(&ModelFile[m_nNumAll][0]);

				// �t�@�C����
				nNumFileAll++;
			}
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
// �O���e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT CStage::ReadText(const char *pTextFile)
{

	// �t�@�C������ǂݍ���
	if (FAILED(ReadXFile(pTextFile)))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	char aComment[MAX_COMMENT] = {};	//�R�����g�p

	// �t�@�C���|�C���^
	FILE *pFile = NULL;

	//�t�@�C�����J��
	pFile = fopen(pTextFile, "r");

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f���̐ݒ�
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			int nType = 0;
			int nShadow = 0;
			D3DXVECTOR3 pos = mylib_const::DEFAULT_VECTOR3;
			D3DXVECTOR3 rot = mylib_const::DEFAULT_VECTOR3;


			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &pos.x);		// X���W
					fscanf(pFile, "%f", &pos.y);		// Y���W
					fscanf(pFile, "%f", &pos.z);		// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &rot.x);		// X�̌���
					fscanf(pFile, "%f", &rot.y);		// Y�̌���
					fscanf(pFile, "%f", &rot.z);		// Z�̌���
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOW������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &nShadow);		// �e���g�����ǂ���
				}

			}// END_MODELSET�̂�����

			if (nShadow == 1)
			{// �e���g�p����ꍇ

				// �^�C�v�̕��𐶐�
				m_pObjX[m_nNumAll] = CObjectX::Create(&ModelFile[nType][0], pos, rot, true);
			}
			else
			{
				// �^�C�v�̕��𐶐�
				m_pObjX[m_nNumAll] = CObjectX::Create(&ModelFile[nType][0], pos, rot, false);
			}

			if (m_pObjX[m_nNumAll] == NULL)
			{// ���s���Ă�����
				return E_FAIL;
			}

			// ��ސݒ�
			m_pObjX[m_nNumAll]->SetType(CObject::TYPE_STAGE);
			m_nNumAll++;	// ���f�������Z
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
// �I�u�W�F�N�g�擾
//==========================================================================
CObjectX *CStage::GetObj(int nIdx)
{
	return m_pObjX[nIdx];
}

//==========================================================================
// �ˏo��擾
//==========================================================================
CInjectionTable *CStage::GetInjectionTable(void)
{
	return m_pInjectionTable;
}

//==========================================================================
// �����擾
//==========================================================================
int CStage::GetNumAll(void)
{
	return m_nNumAll;
}