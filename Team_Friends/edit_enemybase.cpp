//=============================================================================
// 
//  �G�̋��_�G�f�B�^�[���� [edit_enemybase.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_enemybase.h"
#include "enemybase.h"
#include "objectX.h"
#include "XLoad.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "input.h"
#include "elevation.h"
#include "game.h"
#include "enemymanager.h"
#include "enemy.h"
#include "player.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MOVE		(10.0f)
#define MOVE_SLOW	(1.0f)
#define MARKOBJ	"data\\MODEL\\koko.x"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEditEnemyBase::CEditEnemyBase()
{
	// �[���N���A
	m_pos = mylib_const::DEFAULT_VECTOR3;	// �ʒu
	m_rot = mylib_const::DEFAULT_VECTOR3;			// �ʒu
	m_nEnemyType = 0;			// �Ǐ]�̎��
	m_nIdxMapPoint = 0;			// �}�b�v�|�C���g�̃C���f�b�N�X�ԍ�
	m_fPointRatio = 0.0f;		// �ړ�����
	m_fMoveValue = 0.0f;		// �ړ���
	memset(&m_pEnemy[0], NULL, sizeof(m_pEnemy));	// �G�ւ̃|�C���^
	m_apObjX = NULL;			// �I�u�W�F�N�gX
	m_nStage = 0;				// �X�e�[�W
	m_nRush = 0;				// ���b�V���p���ǂ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEditEnemyBase::~CEditEnemyBase()
{

}

//==========================================================================
// ��������
//==========================================================================
CEditEnemyBase *CEditEnemyBase::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CEditEnemyBase *pObjBase = NULL;

	if (pObjBase == NULL)
	{// NULL��������

		// �������̊m��
		pObjBase = DEBUG_NEW CEditEnemyBase;

		if (pObjBase != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjBase->Init();
			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}
		}

		return pObjBase;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEditEnemyBase::Init(void)
{
	m_pos = CManager::GetInstance()->GetCamera()->GetPositionR();
	m_apObjX = CObjectX::Create(MARKOBJ, mylib_const::DEFAULT_VECTOR3, mylib_const::DEFAULT_VECTOR3, false);	// �I�u�W�F�N�gX
	m_apObjX->SetType(CObject::TYPE_OBJECT3D);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEditEnemyBase::Uninit(void)
{
	
}

//==========================================================================
// �I������
//==========================================================================
void CEditEnemyBase::Release(void)
{
	if (m_apObjX != NULL)
	{
		m_apObjX->Uninit();
		m_apObjX = NULL;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CEditEnemyBase::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// ����
	Control(m_pos);
	ChangeType();

	// �ڈ�
	m_apObjX->SetPosition(m_pos);

	for (int nCntEnemy = 0; nCntEnemy < mylib_const::MAX_PATTEN_ENEMY; nCntEnemy++)
	{
		if (m_pEnemy[nCntEnemy] != NULL)
		{// NULL����Ȃ�������
			m_pEnemy[nCntEnemy]->SetPosition(m_pos);
		}
	}

	// �͂ݏ���
	if (pInputKeyboard->GetPress(DIK_SPACE) == true)
	{// SPACE�Œ͂�
		Grab();
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{// ENTER�Ŕz�u

		// �G�̋��_�̎擾
		CEnemyBase *pEnemyBase = CGame::GetEnemyBase();
		if (pEnemyBase == NULL)
		{
			return;
		}
		// ����
		pEnemyBase->CreatePos(m_nStage, m_nEnemyType, m_pos, m_nRush);
	}

	if (pInputKeyboard->GetTrigger(DIK_DELETE) == true)
	{// delete�ō폜
		Delete();
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{// �Z�[�u

		// �G�̋��_�̎擾
		CEnemyBase *pEnemyBase = CGame::GetEnemyBase();
		if (pEnemyBase == NULL)
		{
			return;
		}
		pEnemyBase->Save();
	}

	// �f�o�b�O���
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[ �G�̋��_�G�f�B�^�[��� ]------------------\n"
		"<�z�u>         [ENTER]\n"
		"<�t�@�C���ۑ�> [F9] �ydata/BIN/enemybase.bin�z\n"
		"<�ړ�>         �����F[��,��,��,��]�@�ᑬ�F[W,A,S,D]\n"
		"<�㏸,���~>    [I,K]\n"
		"<��ޕύX>     [1, 2][%d]\n"
		"<�X�e�[�W�ύX> [3, 4][%d]\n"
		"<�e�̎g�p��> [5][%d]\n"
		"<�݈͂ړ�>     [SPACE]\n"
		"<�폜>         [DELETE]\n"
		"<�ʒu>         [X�F%f Y�F%f Z�F%f]\n"
		"\n", m_nEnemyType, m_nStage, m_nRush, m_pos.x, m_pos.y, m_pos.z);
}

//==========================================================================
// ���쏈��
//==========================================================================
void CEditEnemyBase::Control(D3DXVECTOR3 &pos)
{
	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̌����擾
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

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

	if (pInputKeyboard->GetPress(DIK_A) == true)
	{// ���L�[�������ꂽ,���ړ�

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// A+W,����ړ�

			pos.x += sinf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// A+S,�����ړ�

			pos.x += sinf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
		}
		else
		{// A,���ړ�

			pos.x += sinf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{// D�L�[�������ꂽ,�E�ړ�

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// D+W,�E��ړ�

			pos.x += sinf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// D+S,�E���ړ�

			pos.x += sinf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
		}
		else
		{// D,�E�ړ�

			pos.x += sinf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{// W�������ꂽ�A���ړ�

		pos.x += sinf(Camerarot.y) * MOVE_SLOW;
		pos.z += cosf(Camerarot.y) * MOVE_SLOW;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{// S�������ꂽ�A��O�ړ�

		pos.x += sinf(D3DX_PI + Camerarot.y) * MOVE_SLOW;
		pos.z += cosf(D3DX_PI + Camerarot.y) * MOVE_SLOW;
	}

	if (pInputKeyboard->GetPress(DIK_I) == true)
	{// I�L�[�������ꂽ,�㏸

		pos.y += MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_K) == true)
	{// K�L�[�������ꂽ,���~

		pos.y -= MOVE;
	}

}


//==========================================================================
// ��ޕύX
//==========================================================================
void CEditEnemyBase::ChangeType(void)
{
	int nNumAll = CGame::GetEnemyManager()->GetPatternNum();

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �z�u����I�u�W�F�N�g�ύX
	bool bChange = false;
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{// 1�������ꂽ

		// �F�̎�ލX�V
		m_nEnemyType = (m_nEnemyType + (nNumAll - 1)) % nNumAll;
		bChange = true;
	}
	else if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{// 2�������ꂽ

		// �F�̎�ލX�V
		m_nEnemyType = (m_nEnemyType + 1) % nNumAll;
		bChange = true;
	}

	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{// 3�������ꂽ

		// �F�̎�ލX�V
		m_nStage--;
	}
	else if (pInputKeyboard->GetTrigger(DIK_4) == true)
	{// 4�������ꂽ

		// �F�̎�ލX�V
		m_nStage++;
	}

	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{// 5�������ꂽ

		// �e�̎g�p�󋵐؂�ւ�
		m_nRush = m_nRush ? 0 : 1;
	}

	if (bChange)
	{
		for (int nCntEnemy = 0; nCntEnemy < mylib_const::MAX_PATTEN_ENEMY; nCntEnemy++)
		{
			if (m_pEnemy[nCntEnemy] != NULL)
			{// NULL�ɂ���
				m_pEnemy[nCntEnemy]->Kill();
				m_pEnemy[nCntEnemy]->Uninit();
				m_pEnemy[nCntEnemy] = NULL;
			}
		}

		CEnemyManager *pEnemyManager = CGame::GetEnemyManager();

		// �G�z�u
		CEnemy **ppEnemy = pEnemyManager->SetEnemy(m_pos, m_rot, m_nEnemyType);

		for (int nCntEnemy = 0; nCntEnemy < mylib_const::MAX_PATTEN_ENEMY; nCntEnemy++)
		{
			if (ppEnemy[nCntEnemy] != NULL)
			{
				m_pEnemy[nCntEnemy] = ppEnemy[nCntEnemy];
			}
		}
	}

}

//==========================================================================
// ���f���͂�
//==========================================================================
void CEditEnemyBase::Grab(void)
{

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �G�̋��_�̎擾
	CEnemyBase *pEnemyBase = CGame::GetEnemyBase();
	if (pEnemyBase == NULL)
	{
		return;
	}

	bool bAll = false;
	if (pInputKeyboard->GetPress(DIK_RCONTROL) == true)
	{// �E�R���g���[���őS�I��
		bAll = true;
	}

	for (int i = 0; i < pEnemyBase->GetNumBase(m_nStage); i++)
	{
		D3DXVECTOR3 TargetPoint = pEnemyBase->GetSpawnPoint(m_nStage, i);
		if (bAll == true || SphereRange(m_pos, TargetPoint, 50.0f, 50.0f))
		{// ���ɓ������Ă���

			// ����
			Control(TargetPoint);

			// �ʒu�ݒ�
			pEnemyBase->SetSpawnPoint(m_nStage, i, TargetPoint);
		}
	}
}

//==========================================================================
// ���f���͂�
//==========================================================================
void CEditEnemyBase::Delete(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �G�̋��_�̎擾
	CEnemyBase *pEnemyBase = CGame::GetEnemyBase();
	if (pEnemyBase == NULL)
	{
		return;
	}

	for (int i = 0; i < pEnemyBase->GetNumAll(); i++)
	{
		D3DXVECTOR3 TargetPoint = pEnemyBase->GetSpawnPoint(m_nStage, i);
		if (SphereRange(m_pos, TargetPoint, 50.0f, 50.0f))
		{// ���ɓ������Ă���
			pEnemyBase->DeletePos(m_nStage, i);
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CEditEnemyBase::Draw(void)
{

}
