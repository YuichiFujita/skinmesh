//=============================================================================
// 
//  �I�u�W�F�N�g3D���� [objectChara.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectChara.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CObjectChara::Load CObjectChara::m_aLoadData[mylib_const::MAX_MODEL] = {};	// �ǂݍ��݃f�[�^
int CObjectChara::m_nNumLoad = 0;	// �ǂݍ��񂾐�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectChara::CObjectChara(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	D3DXMatrixIdentity(&m_mtxWorld);			// ���[���h�}�g���b�N�X
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ŏ��̈ʒu
	m_fVelocity = 0.0f;			// �ړ����x
	m_fRadius = 0.0f;			// ���a
	m_fRotDest = 0.0f;			// �ڕW�̌���
	m_nLife = 0;				// �̗�
	m_nLifeOrigin = 0;			// ���̗̑�
	m_nMotionStartIdx = 0;		// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
	m_nAddScore = 0;			// �X�R�A���Z��
	m_nNumModel = 0;			// ���f���̐�
	m_nIdxFile = 0;				// �t�@�C���̃C���f�b�N�X�ԍ�
	m_bWaveHit = false;			// �g�ɓ������Ă��邩�̔���

	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;	// ���f��(�p�[�c)�̃|�C���^
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectChara::~CObjectChara()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectChara *CObjectChara::Create(const std::string pTextFile)
{
	// �����p�̃I�u�W�F�N�g
	CObjectChara *pObjChara = NULL;

	if (pObjChara == NULL)
	{// NULL��������

		// �������̊m��
		pObjChara = DEBUG_NEW CObjectChara;

		if (pObjChara != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjChara->SetCharacter(pTextFile);
			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pObjChara;
	}

	return NULL;
}

//==========================================================================
// �L�����쐬
//==========================================================================
HRESULT CObjectChara::SetCharacter(const std::string pTextFile)
{
	// ����������
	if (FAILED(CObjectChara::Init()))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �ǂݍ��݊m�F
	for (int nCntData = 0; nCntData < m_nNumLoad; nCntData++)
	{
		if (m_aLoadData[nCntData].sTextFile == pTextFile)
		{// �t�@�C��������v���Ă���

			for (int nCntParts = 0; nCntParts < m_aLoadData[nCntData].nNumModel; nCntParts++)
			{// �p�[�c���J��Ԃ�

				// ���f�����ݒ�
				m_nNumModel = m_aLoadData[nCntData].nNumModel;

				// �ŏ��̈ʒu�ݒ�
				m_posOrigin = m_aLoadData[nCntData].posOrigin;

				// �ړ����x
				m_fVelocity = m_aLoadData[nCntData].fVelocity;

				// ���a
				m_fRadius = m_aLoadData[nCntData].fRadius;

				// �̗�
				m_nLife = m_aLoadData[nCntData].nLife;
				m_nLifeOrigin = m_nLife;	// ���̗̑�

				// �X�R�A���Z��
				m_nAddScore = m_aLoadData[nCntData].nAddScore;

				// �t�@�C���̃C���f�b�N�X�ԍ�
				m_nIdxFile = nCntData;

				// ���f���̐���
				m_apModel[nCntParts] = CModel::Create(
					m_aLoadData[nCntData].LoadData[m_aLoadData[nCntData].LoadData[nCntParts].nType].pModelFile.c_str(),
					m_aLoadData[nCntData].LoadData[nCntParts].pos,
					m_aLoadData[nCntData].LoadData[nCntParts].rot);

				if (m_apModel[nCntParts] == NULL)
				{// NULL��������

					// ���f���̏I������
					m_apModel[nCntParts]->Uninit();
					delete m_apModel[nCntParts];
					m_apModel[nCntParts] = NULL;
				}

				// �e���f���̐ݒ�
				if (m_aLoadData[nCntData].LoadData[nCntParts].nParent >= 0)
				{
					// �e�̃��f���I�u�W�F�N�g�ݒ�
					m_apModel[nCntParts]->SetParent(m_apModel[m_aLoadData[nCntData].LoadData[nCntParts].nParent]);
				}
				else
				{// �������e�̎�
					m_apModel[nCntParts]->SetParent(NULL);
				}

				// �����z�u�̔���
				if (m_aLoadData[nCntData].LoadData[nCntParts].nStart != 1)
				{
					ChangeObject(nCntParts, -1);
				}
			}

			return S_OK;
		}
	}

	if (FAILED(CObjectChara::ReadText(pTextFile)))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectChara::Init(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < mylib_const::MAX_MODEL; nCntParts++)
		{
			//m_aLoadData[nCntModel].LoadData[nCntParts].nStart = 1;	// �ǂݍ��݂̃f�[�^
		}
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectChara::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// ���f���̏I������
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectChara::Update(void)
{

}

//==========================================================================
// ���f���ύX
//==========================================================================
void CObjectChara::ChangeObject(int nDeleteParts, int nNewParts)
{
	int nIdx = GetIdxFile();
	Load LoadData = GetLoadData(nIdx);

	// �I�u�W�F�N�g�L�����N�^�[�̏��擾
	CObjectChara *pObjChar = GetObjectChara();

	int nNumAll = pObjChar->GetNumModel();

	if (m_apModel[nDeleteParts] != NULL)
	{// NULL����Ȃ�������

		// ���f���̏I������
		m_apModel[nDeleteParts]->Uninit();
		delete m_apModel[nDeleteParts];
		m_apModel[nDeleteParts] = NULL;
	}

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0 && m_apModel[nNewParts] == NULL)
	{
		if (m_apModel[nNewParts] == NULL)
		{
			// ���f���쐬
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == NULL)
		{// NULL��������

			// ���f���̏I������
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = NULL;
		}
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == NULL)
		{// NULL��������
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL);
		}
	}
}

//==========================================================================
// ���f���ݒ�
//==========================================================================
void CObjectChara::SetObject(int nNewParts)
{
	int nIdx = GetIdxFile();
	Load LoadData = GetLoadData(nIdx);

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0 && m_apModel[nNewParts] == NULL)
	{
		if (m_apModel[nNewParts] == NULL)
		{
			// ���f���쐬
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == NULL)
		{// NULL��������

			// ���f���̏I������
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = NULL;
		}
	}
	else
	{
		int n = 0;
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == NULL)
		{// NULL��������
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL);
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾
	
	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw();
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(D3DXCOLOR col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw(col);
		}
	}
}


//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(float fAlpha)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾
	
	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw(fAlpha);
		}
	}
}

//==========================================================================
// �e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT CObjectChara::ReadText(const std::string pTextFile)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(pTextFile.c_str(), "r");

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	// �t�@�C�����ۑ�
	m_aLoadData[m_nNumLoad].sTextFile = pTextFile;

	// �t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxFile = m_nNumLoad;

	char aComment[MAX_COMMENT];	// �R�����g
	int nCntSetParts = 0;
	int nCntFileName = 0;

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(aComment, "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &m_nNumModel);	// ���f����
			m_aLoadData[m_nNumLoad].nNumModel = m_nNumModel;
		}

		while (nCntFileName != m_nNumModel)
		{// ���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			// ���f�����̐ݒ�
			if (strcmp(aComment, "MODEL_FILENAME") == 0)
			{// NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �t�@�C�����ۑ�
				m_aLoadData[m_nNumLoad].LoadData[nCntFileName].pModelFile = aComment;

				nCntFileName++;	// �t�@�C�������Z
			}
		}

		// �e�p�[�c�̐ݒ�
		if (strcmp(aComment, "CHARACTERSET") == 0)
		{// �L�����N�^�[���̓ǂݍ��݂��J�n

			while (strcmp(aComment, "END_CHARACTERSET") != 0)
			{// END_CHARACTERSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	//�m�F����

				if (strcmp(aComment, "VELOCITY") == 0)
				{// VELOCITY�ňړ����x

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fVelocity);	// �ړ����x
					m_fVelocity = m_aLoadData[m_nNumLoad].fVelocity;
				}

				if (strcmp(aComment, "RADIUS") == 0)
				{// RADIUS�Ŕ��a

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fRadius);	// ���a
					m_fRadius = m_aLoadData[m_nNumLoad].fRadius;
				}

				if (strcmp(aComment, "LIFE") == 0)
				{// LIFE�ő̗�

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nLife);	// �̗�
					m_nLife = m_aLoadData[m_nNumLoad].nLife;
					m_nLifeOrigin = m_nLife;	// ���̗̑�
				}

				if (strcmp(aComment, "MOTION_STARTPARTS") == 0)
				{// MOTION_STARTPARTS�Ń��[�V�����J�n�̃C���f�b�N�X�ԍ�

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nMotionStartIdx);	// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
					m_nMotionStartIdx = m_aLoadData[m_nNumLoad].nMotionStartIdx;
				}

				if (strcmp(aComment, "SCORE") == 0)
				{// LIFE�ő̗�

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nAddScore);	// �̗�
					m_nAddScore = m_aLoadData[m_nNumLoad].nAddScore;
				}

				if (strcmp(aComment, "PARTSSET") == 0)
				{// PARTSSET�Ńp�[�c���̓ǂݍ��݊J�n

					while (strcmp(aComment, "END_PARTSSET") != 0)
					{// END_PARTSSET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	// �m�F����

						if (strcmp(aComment, "INDEX") == 0)
						{// INDEX�������烂�f���ԍ��ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);	// =�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType);		// ���f����ނ̗�
						}

						if (strcmp(aComment, "PARENT") == 0)
						{// PARENT��������e�̔ԍ��ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);	// =�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);		// �e�̔ԍ�
						}

						if (strcmp(aComment, "POS") == 0)
						{// POS��������ʒu�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.x);	// X���W
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.y);	// Y���W
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.z);	// Z���W

							if (nCntSetParts == 0)
							{
								m_posOrigin = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos;	// �ŏ��̈ʒu
								m_aLoadData[m_nNumLoad].posOrigin = m_posOrigin;	// �ŏ��̈ʒu
							}
						}

						if (strcmp(aComment, "ROT") == 0)
						{// ROT������������ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.x);	// X�̊p�x
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.y);	// Y�̊p�x
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.z);	// Z�̊p�x
						}

						if (strcmp(aComment, "START") == 0)
						{// START�������珉�����

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart);	// �ŏ����炠�邩
						}

						if (strcmp(aComment, "SWITCHTYPE") == 0)
						{// SWITCHTYPE��������؂�ւ��̎��

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nSwitchType);	// �؂�ւ��̎��
						}
						
						if (strcmp(aComment, "ID_SWITCHMODEL") == 0)
						{// ID_SWITCHMODEL��������؂�ւ��郂�f��ID

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nIDSwitchModel);	// �؂�ւ��郂�f��ID
						}

					}// END_PARTSSET�̂�����

					// ���f���̐���
					if (m_apModel[nCntSetParts] == NULL)
					{
						m_apModel[nCntSetParts] = CModel::Create(
							m_aLoadData[m_nNumLoad].LoadData[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType].pModelFile.c_str(),
							m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos,
							m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot);
					}

					if (m_apModel[nCntSetParts] == NULL)
					{// NULL��������

						// ���f���̏I������
						m_apModel[nCntSetParts]->Uninit();
						delete m_apModel[nCntSetParts];
						m_apModel[nCntSetParts] = NULL;
					}

					// �e���f���̐ݒ�
					if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent >= 0)
					{
						m_apModel[nCntSetParts]->SetParent(m_apModel[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent]);
					}
					else
					{
						m_apModel[nCntSetParts]->SetParent(NULL);
					}

					if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart != 1)
					{
						ChangeObject(nCntSetParts, -1);
					}

					nCntSetParts++;	// �p�[�c�̃J�E���g�����Z
				}
			}// END_CHARACTERSET�̂�����
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

	return S_OK;
}


//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObjectChara::SetmtxWorld(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
D3DXMATRIX CObjectChara::GetmtxWorld(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// ���S�̈ʒu�擾
//==========================================================================
D3DXVECTOR3 CObjectChara::GetCenterPosition(void) const
{
	if (m_apModel[0] == NULL)
	{
		return mylib_const::DEFAULT_VECTOR3;
	}
	return GetPosition() + m_apModel[0]->GetPosition();
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObjectChara::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObjectChara::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// �ړ����x�ݒ�
//==========================================================================
void CObjectChara::SetVelocity(const float fVelocity)
{
	m_fVelocity = fVelocity;
}

//==========================================================================
// �ړ����x�ݒ�
//==========================================================================
float CObjectChara::GetVelocity(void) const
{
	return m_fVelocity;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
void CObjectChara::SetRotDest(const float fRotDest)
{
	m_fRotDest = fRotDest;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
float CObjectChara::GetRotDest(void) const
{
	return m_fRotDest;
}

//==========================================================================
// ���a�ݒ�
//==========================================================================
void CObjectChara::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//==========================================================================
// ���a�擾
//==========================================================================
float CObjectChara::GetRadius(void) const
{
	return m_fRadius;
}

//==========================================================================
// �̗͐ݒ�
//==========================================================================
void CObjectChara::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//==========================================================================
// �̗͎擾
//==========================================================================
int CObjectChara::GetLife(void) const
{
	return m_nLife;
}

//==========================================================================
// ���̗͎̑擾
//==========================================================================
int CObjectChara::GetLifeOrigin(void) const
{
	return m_nLifeOrigin;
}

//==========================================================================
// ���[�V�����J�n�̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObjectChara::GetMotionStartIdx(void) const
{
	return m_nMotionStartIdx;
}

//==========================================================================
// �X�R�A���Z�ʎ擾
//==========================================================================
int CObjectChara::GetAddScoreValue(void) const
{
	return m_nAddScore;
}

//==========================================================================
// �g�̓����蔻��ON
//==========================================================================
void CObjectChara::SetEnableWaveHit(void)
{
	m_bWaveHit = true;
}

//==========================================================================
// �g�̓����蔻��OFF
//==========================================================================
void CObjectChara::SetDisableWaveHit(void)
{
	m_bWaveHit = false;
}

//==========================================================================
// �g�̔����Ԏ擾
//==========================================================================
bool CObjectChara::IsWaveHit(void)
{
	return m_bWaveHit;
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
int CObjectChara::GetState(void)
{
	return STATE_NONE;
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CObjectChara::SetState(int state)
{
	
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CObjectChara::SetState(int state, int nCntState)
{

}

//==========================================================================
// ��ގ擾
//==========================================================================
int CObjectChara::GetCharaType(void)
{
	return 0;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectChara *CObjectChara::GetObjectChara(void)
{
	return this;
}

//==========================================================================
// ���f�����擾
//==========================================================================
int CObjectChara::GetNumModel(void) 
{
	return m_nNumModel;
}

//==========================================================================
// ���f�����ݒ�
//==========================================================================
void CObjectChara::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//==========================================================================
// ���f�����̎擾
//==========================================================================
CModel **CObjectChara::GetModel(void)
{
	return &m_apModel[0];
}

//==========================================================================
// �ǂݍ��ݏ��擾
//==========================================================================
CObjectChara::Load CObjectChara::GetLoadData(int nIdx)
{
	return m_aLoadData[nIdx];
}

//==========================================================================
// �t�@�C���̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObjectChara::GetIdxFile(void)
{
	return m_nIdxFile;
}
