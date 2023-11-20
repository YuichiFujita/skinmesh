//=============================================================================
// 
//  �ˏo�䏈�� [injectiontable.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "injectiontable.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "scene.h"
#include "instantfade.h"
#include "camera.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TIME_CHARGE	(120)	// �`���[�W����
#define TIME_UP		(60)	// �㏸����
#define MODELFILE	"data\\MODEL\\movefllor.x"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �֐��|�C���^
//==========================================================================
CInjectionTable::STATE_FUNC CInjectionTable::m_StateFuncList[] =
{
	&CInjectionTable::StateNone,
	&CInjectionTable::StateCharge,
	&CInjectionTable::StateUP,
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CInjectionTable::CInjectionTable(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_state = STATE_NONE;	// ���
	m_nCntState = 0;		// ��ԑJ�ڃJ�E���^�[
	m_nModelIdx = 0;		// ���f���̃C���f�b�N�X�ԍ�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CInjectionTable::~CInjectionTable()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CInjectionTable *CInjectionTable::Create(const D3DXVECTOR3 pos)
{
	// �����p�̃I�u�W�F�N�g
	CInjectionTable *pTable = NULL;

	if (pTable == NULL)
	{// NULL��������

		// �������̊m��
		pTable = DEBUG_NEW CInjectionTable;

		if (pTable != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pTable->SetPosition(pos);
			pTable->SetOriginPosition(pos);

			// ����������
			pTable->Init();
		}

		return pTable;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CInjectionTable::Init(void)
{
	HRESULT hr;

	// ����������
	hr = CObjectX::Init(MODELFILE);
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_STAGE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CInjectionTable::Uninit(void)
{
	
	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CInjectionTable::Update(void)
{
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �ʏ��ԏ���
//==========================================================================
void CInjectionTable::StateNone(void)
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
		m_state = STATE_NONE;
	}
}

//==========================================================================
// �`���[�W��ԏ���
//==========================================================================
void CInjectionTable::StateCharge(void)
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState++;
	if (m_nCntState >= TIME_CHARGE)
	{
		m_nCntState = TIME_UP;
		m_state = STATE_UP;

		// �J�����擾
		CCamera *pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetEnableFollow(false);
	}
}

//==========================================================================
// �㏸���
//==========================================================================
void CInjectionTable::StateUP(void)
{
	// �J�ڏ�ԂɕύX
	CGame::GetGameManager()->SetType(CGameManager::SCENE_TRANSITION);

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ʒu�X�V
	pos.y += 10.0f;

	// �ʒu�ݒ�
	SetPosition(pos);

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
		m_state = STATE_NONE;

		// �J�ڂȂ��t�F�[�h�ǉ�
		CManager::GetInstance()->GetInstantFade()->SetFade();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CInjectionTable::Draw(void)
{
	// �`��
	CObjectX::Draw(GetColor().a);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CInjectionTable::SetState(STATE state, int nCntState)
{
	m_state = state;			// ���
	m_nCntState = nCntState;	// ��ԑJ�ڃJ�E���^�[
}

//==========================================================================
// ��Ԑݒ�(�I�[�o�[���[�h)
//==========================================================================
void CInjectionTable::SetState(STATE state)
{
	m_state = state;			// ���
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CInjectionTable::STATE CInjectionTable::GetState(void)
{
	return m_state;
}
