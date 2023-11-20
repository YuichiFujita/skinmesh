//=============================================================================
// 
//  �R���p�N�g�R�A���� [compactcore.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "compactcore.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "3D_effect.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"
#include "game.h"
#include "player.h"
#include "player_union.h"
#include "impactwave.h"
#include "scene.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TIME_FADEOUT	(40)	// �t�F�[�h�A�E�g����
#define TIME_GET		(120)	// �擾����
#define MODELFILE		"data\\MODEL\\compactcore_01.x"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CCompactCore::m_nNumAll = 0;		// �A�C�e���̑���

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCompactCore::STATE_FUNC CCompactCore::m_StateFuncList[] =
{
	&CCompactCore::StateNone,
	&CCompactCore::StateGet,
	&CCompactCore::StateFadeOut,
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCompactCore::CCompactCore(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_state = STATE_NONE;		// ���
	m_nCntState = 0;			// ��ԑJ�ڃJ�E���^�[
	m_nLife = 0;				// ����
	m_nLifeMax = 0;				// �����̍ő�l
	m_nCntEmission = 0;			// �������̃J�E���^�[
	m_nNumGetPlayer = 0;		// �擾���Ă�v���C���[�̐�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCompactCore::~CCompactCore()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CCompactCore *CCompactCore::Create(const D3DXVECTOR3 pos)
{
	// �����p�̃I�u�W�F�N�g
	CCompactCore *pBullet = NULL;

	if (pBullet == NULL)
	{// NULL��������

		// �������̊m��
		pBullet = DEBUG_NEW CCompactCore;

		if (pBullet != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pBullet->SetPosition(pos);

			// ����������
			pBullet->Init();
		}

		return pBullet;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCompactCore::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_nLifeMax = 60 * 30;
	m_nLife = m_nLifeMax;	// ����

	// ����������
	hr = CObjectX::Init(MODELFILE);
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_BULLET);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCompactCore::Uninit(void)
{
	// �I������
	CObjectX::Uninit();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CCompactCore::Update(void)
{
	// �ʒu�X�V
	UpdatePos();

	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();

	// �����蔻�菈��
	CollisionPlayer();

	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// �������Z
	//m_nLife--;

	if (m_nLife <= 0 && m_state != STATE_FADEOUT)
	{// �������s������
		
		// �t�F�[�h�A�E�g���
		m_state = STATE_FADEOUT;
		m_nCntState = TIME_FADEOUT;
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �ړ�����
//==========================================================================
void CCompactCore::UpdatePos(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	if (CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) == false)
	{// �n�ʂƐڐG���Ă�����

		// �ʒu�X�V
		pos.y += -mylib_const::GRAVITY;
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �����ݒ�
	SetRotation(rot);
}

//==========================================================================
// �ʏ��ԏ���
//==========================================================================
void CCompactCore::StateNone(void)
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
// �擾��ԏ���
//==========================================================================
void CCompactCore::StateGet(void)
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;

		// �I������
		Uninit();
		return;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g����
//==========================================================================
void CCompactCore::StateFadeOut(void)
{
	// �F�擾
	D3DXCOLOR col = GetColor();

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �s�����x�X�V
	col.a = (float)m_nCntState / (float)TIME_FADEOUT;

	// �F�ݒ�
	SetColor(col);

	if (m_nCntState <= 0)
	{
		m_nCntState = 0;

		// �I������
		Uninit();
		return;
	}
}

//==========================================================================
// �v���C���[�Ƃ̔���
//==========================================================================
void CCompactCore::CollisionPlayer(void)
{
	if (m_state == STATE_GET)
	{
		return;
	}

	// �擾���Ă�v���C���[�̐����Z�b�g
	m_nNumGetPlayer = 0;

	// �擾�����v���C���[�̃C���f�b�N�X
	int nGetPlayerIdx[mylib_const::MAX_PLAYER];

	// �v���C���[���擾
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL || 
			pPlayer->GetState() == CPlayer::STATE_COMPACTUNION || pPlayer->GetState() == CPlayer::STATE_RELEASEUNION)
		{// NULL��������
			continue;
		}

		// �v���C���[�̏��擾
		D3DXVECTOR3 PlayerPosition = pPlayer->GetPosition();
		//D3DXVECTOR3 PlayerPosition = pPlayer->GetCenterPosition();
		D3DXVECTOR3 PlayerRotation = pPlayer->GetRotation();
		float fPlayerRadius = pPlayer->GetRadius();

		// ���擾
		D3DXVECTOR3 pos = GetPosition();
		float fRadius = GetVtxMax().x;

		if (SphereRange(pos, PlayerPosition, fRadius, fPlayerRadius))
		{// �������Ă�����

			// �p�[�e�B�N������
			my_particle::Create(pos, my_particle::TYPE_ENEMY_FADE);

			// �C���f�b�N�X�ۑ�
			nGetPlayerIdx[m_nNumGetPlayer] = nCntPlayer;

			// �擾�l�����Z
			m_nNumGetPlayer++;

			if (m_nNumGetPlayer >= 2)
			{
				// �擾��Ԃɂ���
				m_state = STATE_GET;
				m_nCntState = TIME_GET;

				// �v���C���[���ȈՍ��̏�Ԃɐݒ�
				int nParent = nGetPlayerIdx[0];
				int nExcept = nGetPlayerIdx[1];

				CPlayer **ppPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
				ppPlayer[nParent]->SetState(CPlayer::STATE_COMPACTUNION);
				ppPlayer[nExcept]->SetState(CPlayer::STATE_COMPACTUNION);
				

				// �e�C���f�b�N�X�̎�ގ擾
				int nParentType = CManager::GetInstance()->GetByPlayerPartsType(nParent);
				int nExceptType = CManager::GetInstance()->GetByPlayerPartsType(nExcept);

				CPlayerUnion *pPlayerUnion = NULL;

				// ��ގ擾���Ă��̎�ނɊY�����鍇�̐�𐶐�����
				if (nParentType == CPlayerUnion::PARTS_LEG ||
					nExceptType == CPlayerUnion::PARTS_LEG)
				{// �r��������

					if ((nParentType == CPlayerUnion::PARTS_LEG && nExceptType == CPlayerUnion::PARTS_BODY) ||
						(nParentType == CPlayerUnion::PARTS_BODY && nExceptType == CPlayerUnion::PARTS_LEG))
					{
						// �̂Ƌr
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_BODYtoLEG);
					}
					else if ((nParentType == CPlayerUnion::PARTS_LEG && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)) ||
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && nExceptType == CPlayerUnion::PARTS_LEG))
					{
						// �r�Ƌr
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_LEGtoARM);
					}

					int nControllIdx = nParentType;
					if (nExceptType == CPlayerUnion::PARTS_LEG)
					{
						nControllIdx = nExceptType;
					}
					pPlayerUnion->SetControllMoveIdx(nParent);
				}
				else
				{// �r�Ȃ��ł̍���

					if ((nParentType == CPlayerUnion::PARTS_BODY && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)) ||
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && nExceptType == CPlayerUnion::PARTS_BODY))
					{
						// �̂Ƙr
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_BODYtoARM);
					}
					else if (
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)) ||
						((nParentType == CPlayerUnion::PARTS_L_ARM || nParentType == CPlayerUnion::PARTS_R_ARM) && (nExceptType == CPlayerUnion::PARTS_L_ARM || nExceptType == CPlayerUnion::PARTS_R_ARM)))
					{
						// �r�Ƙr
						pPlayerUnion = CPlayerUnion::Create(CPlayerUnion::TYPE_ARMtoARM);
					}
					pPlayerUnion->SetControllMoveIdx(nParent);
				}

				// �v���C���[���̃p�[�c�C���f�b�N�X�ԍ�
				pPlayerUnion->SetPlayerByPartsIdx(0, nParent);
				pPlayerUnion->SetPlayerByPartsIdx(1, nExcept);
				pPlayerUnion->SetPosition(pos);
				return;
			}
			continue;
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCompactCore::Draw(void)
{
	// �`��
	CObjectX::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CCompactCore::SetState(STATE state, int nCntState)
{
	m_state = state;			// ���
	m_nCntState = nCntState;	// ��ԑJ�ڃJ�E���^�[
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CCompactCore::STATE CCompactCore::GetState(void)
{
	return m_state;
}

//==========================================================================
// �����擾
//==========================================================================
int CCompactCore::GetNumAll(void)
{
	return m_nNumAll;
}
