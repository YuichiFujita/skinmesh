//=============================================================================
// 
//  �e�N�X�`������ [texture.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

// �ǂݍ��ރe�N�X�`���̃w�b�_�[
#include "map.h"
#include "3D_effect.h"
#include "effect_slashhit.h"
#include "effect_thunderring.h"
#include "effect_enemyspawn.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CTexture::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTexture::CTexture()
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		m_pTexInfo[nCntData].nFileNameLen = 0;	// �t�@�C�����̕�����
		m_pTexInfo[nCntData].pTexture = NULL;		// �e�N�X�`���̃|�C���^
		memset(&m_pTexInfo[nCntData].imageInfo, 0, sizeof(D3DXIMAGE_INFO));	// �e�N�X�`���f�ޏ��

		for (int nCntFile = 0; nCntFile < mylib_const::MAX_STRING; nCntFile++)
		{
			m_pTexInfo[nCntData].acFilename[nCntFile] = NULL;	// �t�@�C����
		}
	}
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTexture::~CTexture()
{

}

//==========================================================================
// �e�N�X�`���̓ǂݍ���
//==========================================================================
HRESULT CTexture::LoadAll(void)
{
	// �}�b�v�p�̓ǂݍ���
	if (FAILED(map::ReadTexture()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// 3D�G�t�F�N�g
	CEffect3D::LoadTexture();

	// �a���q�b�g
	CEffectSlashHit::LoadTexture();

	// ���̃����O
	CThunderRing::LoadTexture();

	// �G�X�|�[��
	CEffectEnemySpawn::LoadTexture();

	return S_OK;
}

//==========================================================================
// ����e�N�X�`���̓ǂݍ���
//==========================================================================
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//==========================================================================
// �S�Ẵe�N�X�`���̔j��
//==========================================================================
void CTexture::Unload(void)
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexInfo[nCntData].pTexture != NULL)
		{
			m_pTexInfo[nCntData].pTexture->Release();
			m_pTexInfo[nCntData].pTexture = NULL;
		}
	}
}

//==========================================================================
// �e�N�X�`���̊��蓖�ď���
//==========================================================================
int CTexture::Regist(const char *pFileName)
{
	// �ő吔�擾
	int nNumAll = GetNumAll() + 1;
	int nIdx = -1;	// ����̔ԍ��ۑ�
	int nNowLen = 0;	// ����̃t�@�C��������

	if (pFileName != NULL)
	{
		nNowLen = strlen(pFileName);
	}

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (pFileName != NULL)
		{// NULL����Ȃ�������

			if (m_pTexInfo[nCntData].nFileNameLen == nNowLen)
			{// �t�@�C�����̒�����������������

				// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
				if (strcmp(m_pTexInfo[nCntData].acFilename, pFileName) == 0)
				{// �t�@�C��������v���Ă���

					// �ԍ����蓖��
					nIdx = nCntData;

					return nIdx;
				}
			}
		}
	}

	if (pFileName != NULL)
	{// NULL����Ȃ�������

		// �e�N�X�`���ǂݍ���
		HRESULT hr = LoadTex(pFileName);

		if (FAILED(hr))
		{// ���s���Ă�����
			return 0;
		}

		// �ԍ����蓖��
		nIdx = nNumAll - 1;
		return nIdx;
	}

	return 0;
}

//==========================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//==========================================================================
HRESULT CTexture::LoadTex(const char *pFileName)
{
	HRESULT hr;
	int nIdx = m_nNumAll;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile(pDevive,
		pFileName,
		&m_pTexInfo[nIdx].pTexture);

	if (hr == D3DXERR_INVALIDDATA)
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �e�N�X�`���f�ޏ��
	D3DXGetImageInfoFromFile(pFileName, &m_pTexInfo[nIdx].imageInfo);

	// �t�@�C�����ƒ����ۑ�
	strcpy(m_pTexInfo[nIdx].acFilename, pFileName);
	m_pTexInfo[nIdx].nFileNameLen = strlen(&m_pTexInfo[nIdx].acFilename[0]);

	// �������Z
	m_nNumAll++;

	return S_OK;
}

//==========================================================================
// �e�N�X�`���̃A�h���X�擾
//==========================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAdress(int nIdx)
{
	return m_pTexInfo[nIdx].pTexture;
}

//==========================================================================
// �e�N�X�`���̑����擾
//==========================================================================
int CTexture::GetNumAll(void)
{
	return m_nNumAll;
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(const char *pFileName)
{
	// �ő吔�擾
	int nNumAll = GetNumAll() + 1;

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (pFileName != NULL)
		{// NULL����Ȃ�������

			if (m_pTexInfo[nCntData].nFileNameLen == strlen(pFileName))
			{// �t�@�C�����̒�����������������

				// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
				if (strcmp(m_pTexInfo[nCntData].acFilename, pFileName) == 0)
				{// �t�@�C��������v���Ă���

					return m_pTexInfo[nCntData];
				}
			}
		}
	}

	return m_pTexInfo[nNumAll + 1];
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(int nIdxTex)
{
	return m_pTexInfo[nIdxTex];
}

//==========================================================================
// �e�N�X�`���f�ނ̃T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CTexture::GetImageSize(int nIdx)
{
	return D3DXVECTOR2((float)m_pTexInfo[nIdx].imageInfo.Width, (float)m_pTexInfo[nIdx].imageInfo.Height);
}
