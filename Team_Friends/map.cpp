//=============================================================================
// 
//  �}�b�v���� [map.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "map.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "objectX.h"
#include "object3DMesh.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "shadow.h"
using namespace std;	// ���O��Ԃ��w��

namespace mapdate
{
	// �\���̒�`
	struct SMap
	{
		int nType;						// ���
		D3DXVECTOR3 pos;				// �ʒu
		D3DXVECTOR3 rot;				// ����
		int nWidth, nHeight;			// ������
		float fWidthLen, fHeightLen;	// �T�C�Y
		float fMove;					// �ړ���
		int nShadow;					// �e�g�����ǂ���
	};

	int nNumModelAll = 0;	// ���f���̑���
	int nNumTexAll = 0;		// �e�N�X�`���̑���
	int nNumObjXAll = 0;		// �I�u�W�F�N�gX�̑���
	int nNumObj3DMeshAll = 0;		// �I�u�W�F�N�g3D���b�V���̑���
	CObjectX *pObjX[mylib_const::MAX_OBJ];		// �I�u�W�F�N�gX
	CObject3DMesh *pObj3DMesh[mylib_const::MAX_OBJ];		// �I�u�W�F�N�g3D���b�V��
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
vector<std::string> ModelFile;		// ���f���t�@�C����
vector<std::string> TextureFile;	// �e�N�X�`���t�@�C����
mapdate::SMap g_Map;

//==========================================================================
// ��������
//==========================================================================
HRESULT map::Create(const char *pTextFile)
{
	// ����
	mapdate::nNumModelAll = 0;
	mapdate::nNumObjXAll = 0;
	mapdate::nNumObj3DMeshAll = 0;

	memset(&mapdate::pObjX[0], NULL, sizeof(mapdate::pObjX));
	memset(&mapdate::pObj3DMesh[0], NULL, sizeof(mapdate::pObj3DMesh));	// �I�u�W�F�N�g3D���b�V��

	// �O���e�L�X�g�ǂݍ��ݏ���
	HRESULT hr = ReadText(pTextFile);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �������
//==========================================================================
void map::Release(void)
{
	for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
	{
		if (mapdate::pObjX[nCntObj] != NULL)
		{
			mapdate::pObjX[nCntObj]->Uninit();
			mapdate::pObjX[nCntObj] = NULL;
		}

		if (mapdate::pObj3DMesh[nCntObj] != NULL)
		{
			mapdate::pObj3DMesh[nCntObj]->Uninit();
			mapdate::pObj3DMesh[nCntObj] = NULL;
		}
	}
}

//==========================================================================
// �O���t�@�C���������ݏ���
//==========================================================================
HRESULT map::SaveText(void)
{
#if TOPCUR
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\map\\info.txt", "w");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return E_FAIL;
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �e�N�X�`����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_TEXTURE = %d\n\n", mapdate::nNumTexAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �e�N�X�`���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// �e�N�X�`���t�@�C������
	for (int nCntFileNum = 0; nCntFileNum < mapdate::nNumTexAll; nCntFileNum++)
	{
		fprintf(pFile, "TEXTURE_FILENAME = %s\t\t# [%d]\n", &TextureFile[nCntFileNum][0], nCntFileNum);
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# ���f����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", mapdate::nNumModelAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// ���f���t�@�C������
	for (int nCntFileNum = 0; nCntFileNum < mapdate::nNumModelAll; nCntFileNum++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &ModelFile[nCntFileNum][0], nCntFileNum);
	}


	//**********************************
	// ��z�u
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ����\n"
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

			if (TargetType == CObject::TYPE_MESHDOME)
			{// ���b�V���h�[����������

				// X�t�@�C���̏��擾
				CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

				// �e�N�X�`���̃C���f�b�N�X�ԍ�
				int nType = pObjMesh->GetIdxTex();	// ���
				nType--;	// NULL��

				// �o��
				fprintf(pFile,
					"SKYSET\n"
					"\tTEXTYPE = %d\n"
					"\tMOVE = %f\n"
					"END_SKYSET\n\n",
					nType, g_Map.fMove);
			}

			// ���̃I�u�W�F�N�g����
			pObj = pObjNext;
		}
	}

	
	//**********************************
	// �R�z�u
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# �R���\n"
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

			if (TargetType == CObject::TYPE_MESHCYLINDER)
			{// ���b�V���V�����_�[��������

				// ���b�V���̏��擾
				CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

				// �e�N�X�`���̃C���f�b�N�X�ԍ�
				int nType = pObjMesh->GetIdxTex();	// ���
				nType--;	// NULL��

				// �o��
				fprintf(pFile,
					"MOUNTAINSET\n"
					"\tTEXTYPE = %d\n"
					"END_MOUNTAINSET\n\n", nType);
			}

			// ���̃I�u�W�F�N�g����
			pObj = pObjNext;
		}
	}


	//**********************************
	// �n�ʔz�u
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# �n�ʏ��\n"
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

			if (TargetType == CObject::TYPE_MESHFIELD ||
				TargetType == CObject::TYPE_WATERFIELD)
			{// ���b�V���t�B�[���h��������

				// ���b�V���̏��擾
				CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

				// �e�N�X�`���̃C���f�b�N�X�ԍ�
				int nType = pObjMesh->GetIdxTex();	// ���
				D3DXVECTOR3 pos = pObjMesh->GetPosition();		// �ʒu
				D3DXVECTOR3 rot = pObjMesh->GetRotation();		// ����
				int nWidth = pObjMesh->GetWidthBlock();			// ��������
				int nHeight = pObjMesh->GetHeightBlock();		// �c������
				float fWidthLen = pObjMesh->GetWidthLen();		// ������
				float fHeightLen = pObjMesh->GetHeightLen();	// �c����
				nType--;	// NULL��

				// �o��
				fprintf(pFile,
					"FIELDSET\n"
					"\tTEXTYPE = %d\n"
					"\tPOS = %.0f %.0f %.0f\n"
					"\tROT = %.0f %.0f %.0f\n"
					"\tBLOCK = %d %d\n"
					"\tSIZE = %.0f %.0f\n"
					"END_FIELDSET\n\n",
					nType, pos.x, pos.y, pos.z,
					rot.x, rot.y, rot.z,
					nWidth, nHeight,
					fWidthLen, fHeightLen);
			}

			// ���̃I�u�W�F�N�g����
			pObj = pObjNext;
		}
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
// �e�N�X�`���ǂݍ��ݏ���
//==========================================================================
HRESULT map::ReadTexture(void)
{
	char aComment[MAX_COMMENT] = {};	// �R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �t�@�C���|�C���^
	FILE *pFile = NULL;

	//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\map\\info.txt", "r");

	// ��ł���
	/*switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_TUTORIAL:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_GAME:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RESULT:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RANKING:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	default:
		return E_FAIL;
		break;
	}*/

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// �e�N�X�`�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_TEXTURE") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &mapdate::nNumTexAll);	// �e�N�X�`����
		}

		while (nCntTexture != mapdate::nNumTexAll)
		{// �e�N�X�`���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�

			if (strcmp(&aComment[0], "TEXTURE_FILENAME") == 0)
			{// TEXTURE_FILENAME��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �Ō���ɒǉ�
				TextureFile.push_back(&aComment[0]);

				//TextureFile.push_back(std::string());

				// �e�N�X�`���̊��蓖��
				CManager::GetInstance()->GetTexture()->Regist(&TextureFile[nCntTexture][0]);

				nCntTexture++;	// �e�N�X�`�������Z
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
// ���f���ǂݍ��ݏ���
//==========================================================================
HRESULT map::ReadXFile(void)
{
	// ���Z�b�g
	mapdate::nNumModelAll = 0;


	char aComment[MAX_COMMENT] = {};	// �R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �t�@�C���|�C���^
	FILE *pFile = NULL;

	//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\map\\info.txt", "r");

	// ��Ŏg��
	/*switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_TUTORIAL:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_GAME:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RESULT:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RANKING:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	default:
		return E_FAIL;
		break;
	}*/

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

		while (mapdate::nNumModelAll != nFileNum)
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
				pXLoad->XLoad(&ModelFile[mapdate::nNumModelAll][0]);

				mapdate::nNumModelAll++;	// ���f�������Z
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
HRESULT map::ReadText(const char *pTextFile)
{
	char aComment[MAX_COMMENT] = {};	//�R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �t�@�C���|�C���^
	FILE *pFile = NULL;

	//�t�@�C�����J��
	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		pFile = fopen("data\\TEXT\\map\\info_title.txt", "r");
		break;

	case CScene::MODE_TUTORIAL:
		pFile = fopen("data\\TEXT\\map\\info_title.txt", "r");
		break;

	case CScene::MODE_GAME:
		pFile = fopen(pTextFile, "r");
		break;

	case CScene::MODE_RESULT:
		pFile = fopen("data\\TEXT\\map\\info_title.txt", "r");
		break;

	case CScene::MODE_RANKING:
		pFile = fopen("data\\TEXT\\map\\info_title.txt", "r");
		//pFile = fopen("data\\TEXT\\map\\info_ranking.txt", "r");
		break;

	default:
		//pFile = fopen("data\\TEXT\\map\\info.txt", "r");
		return S_OK;
		break;
	}

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

#if 1
		// ���b�V���t�B�[���h�̐ݒ�
		if (strcmp(&aComment[0], "FIELDSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_FIELDSET"))
			{// END_FIELDSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.pos.x);		// X���W
					fscanf(pFile, "%f", &g_Map.pos.y);		// Y���W
					fscanf(pFile, "%f", &g_Map.pos.z);		// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		 // =�̕�
					fscanf(pFile, "%f", &g_Map.rot.x);		 // X�̌���
					fscanf(pFile, "%f", &g_Map.rot.y);		 // Y�̌���
					fscanf(pFile, "%f", &g_Map.rot.z);		 // Z�̌���
					g_Map.rot.x = D3DXToRadian(g_Map.rot.x); // 360�x�`������ϊ�
					g_Map.rot.y = D3DXToRadian(g_Map.rot.y); // 360�x�`������ϊ�
					g_Map.rot.z = D3DXToRadian(g_Map.rot.z); // 360�x�`������ϊ�
				}

				if (strcmp(&aComment[0], "BLOCK") == 0)
				{//BLOCK������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%d", &g_Map.nWidth);		// ���̕�����
					fscanf(pFile, "%d", &g_Map.nHeight);	// �c�̕�����
				}

				if (strcmp(&aComment[0], "SIZE") == 0)
				{//SIZE������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%f", &g_Map.fWidthLen);	// ���̒���
					fscanf(pFile, "%f", &g_Map.fHeightLen);	// �c�̒���
				}

			}// END_FIELDSET�̂�����

			 //**********************************
			 // ��������
			 //**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshField::Create(
				g_Map.pos, g_Map.rot,
				g_Map.fWidthLen, g_Map.fHeightLen,
				g_Map.nWidth, g_Map.nHeight,
				(CMeshField::TYPE)g_Map.nType, &TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == NULL)
			{// NULL��������
				return E_FAIL;
			}

			// �I�u�W�F�N�g���b�V���̑������Z
			mapdate::nNumObj3DMeshAll++;
		}

#endif
		// ���b�V���V�����_�[�̐ݒ�
		if (strcmp(&aComment[0], "MOUNTAINSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_MOUNTAINSET"))
			{// END_MOUNTAINSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);	// ���f����ނ̗�
				}

			}// END_MOUNTAINSET�̂�����

			 //**********************************
			 // ��������
			 //**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshCylinder::Create(&TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == NULL)
			{// NULL��������
				return E_FAIL;
			}

			// �I�u�W�F�N�g���b�V���̑������Z
			mapdate::nNumObj3DMeshAll++;
		}

		// ���b�V���h�[���̐ݒ�
		if (strcmp(&aComment[0], "SKYSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_SKYSET"))
			{// END_MOUNTAINSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "MOVE") == 0)
				{//MOVE��������ړ��ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.fMove);	// �ړ���
				}

			}// END_SKYSET�̂�����

			//**********************************
			// ��������
			//**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshDome::Create(g_Map.fMove, &TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == NULL)
			{// NULL��������
				return E_FAIL;
			}

			// �I�u�W�F�N�g���b�V���̑������Z
			mapdate::nNumObj3DMeshAll++;
		}

		// ���f���̐ݒ�
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);	// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.pos.x);	// X���W
					fscanf(pFile, "%f", &g_Map.pos.y);	// Y���W
					fscanf(pFile, "%f", &g_Map.pos.z);	// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.rot.x);	// X�̌���
					fscanf(pFile, "%f", &g_Map.rot.y);	// Y�̌���
					fscanf(pFile, "%f", &g_Map.rot.z);	// Z�̌���
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOW������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%d", &g_Map.nShadow);	// �e���g�����ǂ���
				}

			}// END_MODELSET�̂�����

			if (g_Map.nShadow == 1)
			{// �e���g�p����ꍇ

				// �^�C�v�̕��𐶐�
				mapdate::pObjX[mapdate::nNumObjXAll] = CObjectX::Create(&ModelFile[g_Map.nType][0], g_Map.pos, g_Map.rot, true);
			}
			else
			{
				// �^�C�v�̕��𐶐�
				mapdate::pObjX[mapdate::nNumObjXAll] = CObjectX::Create(&ModelFile[g_Map.nType][0], g_Map.pos, g_Map.rot, false);
			}

			if (mapdate::pObjX[mapdate::nNumObjXAll] == NULL)
			{// ���s���Ă�����
				return E_FAIL;
			}

			// ��ސݒ�
			mapdate::pObjX[mapdate::nNumObjXAll]->SetType(CObject::TYPE_XFILE);
			mapdate::nNumObjXAll++;
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
// �����擾
//==========================================================================
int map::GetNumModelAll(void)
{
	return mapdate::nNumModelAll;
}