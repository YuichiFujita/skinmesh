//=============================================================================
// 
//  �A�C�e������ [item.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "item.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "explosion.h"
#include "3D_effect.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"
#include "game.h"
#include "player.h"
#include "impactwave.h"
#include "scene.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TIME_FADEOUT	(40)	// �t�F�[�h�A�E�g����

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CItem::m_apModelFile[CGameManager::STATUS_MAX] =	// �e�N�X�`���̃t�@�C��
{
	"data\\MODEL\\item_power.x",	// �Η�
	"data\\MODEL\\item_speed.x",	// �쓮��
	"data\\MODEL\\item_life.x",	// �ϋv��
};
int CItem::m_nNumAll = 0;		// �A�C�e���̑���

//==========================================================================
// �֐��|�C���^
//==========================================================================
CItem::STATE_FUNC CItem::m_StateFuncList[] =
{
	&CItem::StateNone,
	&CItem::StateDamage,
	&CItem::StateFadeOut,
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_type = CGameManager::STATUS_POWER;	// ���
	m_state = STATE_NONE;			// ���
	m_nCntState = 0;			// ��ԑJ�ڃJ�E���^�[
	m_nLife = 0;
	m_nIdxBulletManager = 0;			// �A�C�e���}�l�[�W���̃C���f�b�N�X�ԍ�
	m_nCntEmission = 0;	// �������̃J�E���^�[

	// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	m_nModelIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CItem::~CItem()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CItem *CItem::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// �����p�̃I�u�W�F�N�g
	CItem *pBullet = NULL;

	if (pBullet == NULL)
	{// NULL��������

		// �������̊m��
		pBullet = DEBUG_NEW CItem;

		if (pBullet != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pBullet->SetPosition(pos);

			// �ʒu���蓖��
			pBullet->SetRotation(rot);

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
HRESULT CItem::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_nLifeMax = 60 * 30;
	m_nLife = m_nLifeMax;	// ����

	// ���
	m_type = (CGameManager::eStatus)Random(0, (int)CGameManager::STATUS_MAX - 1);

	// ����������
	hr = CObjectX::Init(m_apModelFile[m_type]);
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_BULLET);

	// �����_���ړ���
	SetMove(D3DXVECTOR3(Random(-50, 50) * 0.1f, 10.0f, Random(-50, 50) * 0.1f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CItem::Uninit(void)
{
	//// �폜
	//if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME && CGame::GetBulletManager() != NULL)
	//{// �A�C�e���}�l�[�W���̍폜
	//	CGame::GetBulletManager()->Delete(m_nIdxBulletManager);
	//}

	// �I������
	CObjectX::Uninit();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CItem::Update(void)
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
	m_nLife--;

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
void CItem::UpdatePos(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ʒu�X�V
	pos += move;

	// �d��
	move.y -= mylib_const::GRAVITY;

	if (CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos))
	{// �n�ʂƐڐG���Ă�����
		move *= 0.8f;
		move.y *= 0.6f;
		move.y *= -1;
	}

	// �����␳
	move += (mylib_const::DEFAULT_VECTOR3 - move) * 0.01f;

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);
}

//==========================================================================
// �ʏ��ԏ���
//==========================================================================
void CItem::StateNone(void)
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
// �_���[�W��ԏ���
//==========================================================================
void CItem::StateDamage(void)
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
// �t�F�[�h�A�E�g����
//==========================================================================
void CItem::StateFadeOut(void)
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
void CItem::CollisionPlayer(void)
{
	// �v���C���[���擾
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
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

			// �X�e�[�^�X�t�^
			pPlayer->GiveStatus(m_type);

			// �p�[�e�B�N������
			my_particle::Create(pos, my_particle::TYPE_ENEMY_FADE);

			// �I������
			Uninit();
			continue;
		}
	}
}

//==========================================================================
// �v���C���[�A�C�e���̍X�V
//==========================================================================
void CItem::UpdateTypePlayer(void)
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CItem::Draw(void)
{
	// �`��
	CObjectX::Draw(GetColor().a);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CItem::SetState(STATE state, int nCntState)
{
	m_state = state;			// ���
	m_nCntState = nCntState;	// ��ԑJ�ڃJ�E���^�[
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CItem::STATE CItem::GetState(void)
{
	return m_state;
}

//==========================================================================
// �����擾
//==========================================================================
int CItem::GetNumAll(void)
{
	return m_nNumAll;
}
