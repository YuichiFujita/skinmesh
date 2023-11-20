//=============================================================================
// 
//  ��Q���e���� [bullet_obstacle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "bullet_obstacle.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "explosion.h"
#include "3D_effect.h"
#include "sound.h"
#include "particle.h"
#include "enemy.h"
#include "calculation.h"
#include "game.h"
#include "bulletmanager.h"
#include "player.h"
#include "impactwave.h"
#include "effect_thunderring.h"
#include "bulletexplosion.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define BULLETMODEL		"data\\MODEL\\obstacleballet_01.x"
#define MOVE_SPEED		(15.0f)							// �ړ����x

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �֐��|�C���^
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBulletObstacle::CBulletObstacle(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_nLife = 0;			// ����
	m_nLifeMax = 0;			// �����̍ő�l
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nCntEmission = 0;		// �������̃J�E���^�[
	m_fRadius = 0.0f;		// ���a
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBulletObstacle::~CBulletObstacle()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBulletObstacle *CBulletObstacle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const float fSize)
{
	// �����p�̃I�u�W�F�N�g
	CBulletObstacle *pBullet = NULL;

	if (pBullet == NULL)
	{// NULL��������

		// �������̊m��
		pBullet = DEBUG_NEW CBulletObstacle;

		if (pBullet != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pBullet->SetPosition(pos);

			// �ʒu���蓖��
			pBullet->SetRotation(rot);

			// �ړ��ʐݒ�
			pBullet->SetMove(move);

			// �T�C�Y�ݒ�
			pBullet->m_fRadius = fSize;

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
HRESULT CBulletObstacle::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_nLifeMax = 60 * 5;
	m_nLife = m_nLifeMax;	// ����

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_BULLET);

	// ����������
	hr = CObjectX::Init(BULLETMODEL);
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBulletObstacle::Uninit(void)
{
	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBulletObstacle::Update(void)
{
	// �ʒu�X�V
	UpdatePos();

	// �v���C���[�Ƃ̔���
	CollisionPlayer();

	// �������Z
	m_nLife--;

	if (m_nLife <= 0)
	{// �������s������

		// �����̐���
		//CExplosion::Create(GetPosition());

		// �I������
		Uninit();
		return;
	}
}

//==========================================================================
// �ړ�����
//==========================================================================
void CBulletObstacle::UpdatePos(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ʒu�X�V
	pos += move;

	// ��]
	rot.y += D3DX_PI * 0.025f;
	rot.x += D3DX_PI * (Random(5, 25) * 0.001f);

	if (m_nLife % 2 == 0)
	{
		CEffect3D::Create(pos, D3DXVECTOR3(Random(-10, 10) * 0.1f + -move.x, -move.y, Random(-10, 10) * 0.1f + -move.z), D3DXCOLOR(1.0f, 0.4f, 0.1f, 1.0f), (float)Random(80, 120), 12, CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKE);
		CEffect3D::Create(pos, D3DXVECTOR3(Random(-10, 10) * 0.1f + -move.x, -move.y, Random(-10, 10) * 0.1f + -move.z), D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), (float)Random(60, 100), 12, CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKE);
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);
}

//==========================================================================
// �v���C���[�Ƃ̔���
//==========================================================================
void CBulletObstacle::CollisionPlayer(void)
{
	// �v���C���[���擾
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);
		if (pPlayer == NULL)
		{// NULL��������
			return;
		}

		// �v���C���[�̏��擾
		D3DXVECTOR3 PlayerPosition = pPlayer->GetCenterPosition();
		D3DXVECTOR3 PlayerRotation = pPlayer->GetRotation();
		float fPlayerRadius = pPlayer->GetRadius();

		// ���擾
		D3DXVECTOR3 pos = GetPosition();

		if (SphereRange(pos, PlayerPosition, m_fRadius, fPlayerRadius))
		{// �������Ă�����

			// �q�b�g����
			pPlayer->Hit(1);

			// �I������
			Uninit();
		}
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CBulletObstacle::Draw(void)
{
	// �`��
	CObjectX::Draw();
}

