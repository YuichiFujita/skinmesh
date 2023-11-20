//=============================================================================
// 
//  �e���� [bullet.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "bullet.h"
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
#define WIDTH			(30.0f)							// ����
#define HEIGHT			(30.0f)							// �c��
#define MOVE_SPEED		(15.0f)							// �ړ����x
#define ANIM_SPEED		(4)								// �ǂݍ��݊Ԋu
#define MAX_PATTERN_U	(1)								// U�̕�����
#define MAX_PATTERN_V	(1)								// V�̕�����
#define MAX_PATTERN		(MAX_PATTERN_U)					// �A�j���[�V�����p�^�[���̍ő吔
#define MOVE_U			(1.0f / (float)MAX_PATTERN_U)	// U���W�ړ���
#define MOVE_V			(1.0f / (float)MAX_PATTERN_V)	// V���W�ړ���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CBullet::m_apTextureFile[TYPE_MAX] =	// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\sunder_031.png",
	"data\\TEXTURE\\sunder_031.png",
};
int CBullet::m_nNumAll = 0;		// �e�̑���

//==========================================================================
// �֐��|�C���^
//==========================================================================
CBullet::STATE_FUNC CBullet::m_FuncList[] =
{
	&CBullet::StateNone,
	&CBullet::StateDamage,
};

CBullet::COLLISION_FUNC CBullet::m_CollisionFuncList[] =	// �����蔻��̃��X�g
{
	&CBullet::CollisionEnemy,	// �G�Ƃ̔���
	&CBullet::CollisionPlayer,	// �v���C���[�Ƃ̔���
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBullet::CBullet(int nPriority) : CMeshSphere(nPriority), m_nLifeMax(1)
{
	// �l�̃N���A
	m_type = TYPE_PLAYER;
	m_state = STATE_NONE;			// ���
	m_MoveType = MOVETYPE_NORMAL;	// �ړ��̎��
	m_nCntState = 0;			// ��ԑJ�ڃJ�E���^�[
	m_nLife = 0;
	m_nIdxBulletManager = 0;			// �e�}�l�[�W���̃C���f�b�N�X�ԍ�
	m_nCntEmission = 0;	// �������̃J�E���^�[
	m_pMeshSphereEffect = NULL;		// ���b�V���X�t�B�A�̃G�t�F�N�g
	m_pEffectThunderRing = NULL;	// ���̃����O�̃G�t�F�N�g
	memset(&m_pBulletAppearance[0], NULL, sizeof(m_pBulletAppearance));	// �����ڂ����̒e

	// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	m_nTexIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBullet::~CBullet()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBullet *CBullet::Create(TYPE type, MOVETYPE movetype, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const float fSize)
{
	// �����p�̃I�u�W�F�N�g
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{// NULL��������

		// �������̊m��
		pBullet = DEBUG_NEW CBullet;

		if (pBullet != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ��ސݒ�
			pBullet->m_type = type;

			// �ړ����
			pBullet->m_MoveType = movetype;

			// �ʒu���蓖��
			pBullet->SetPosition(pos);

			// �ʒu���蓖��
			pBullet->SetRotation(rot);

			// �ړ��ʐݒ�
			pBullet->SetMove(move);

			// �T�C�Y�ݒ�
			pBullet->SetWidthLen(fSize);
			pBullet->SetHeightLen(fSize);
			pBullet->SetWidthBlock(16);
			pBullet->SetHeightBlock(16);

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
HRESULT CBullet::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_nLifeMax = 60 * 5;
	m_nLife = m_nLifeMax;	// ����
	SetColor(D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f));

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_type]);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_BULLET);

	// ����������
	hr = CMeshSphere::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ���蓖��
	m_nIdxBulletManager = CGame::GetBulletManager()->Regist(this);

	// �e�N�X�`���̊��蓖��
	int nTex = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\effect\\effect000.jpg");

	// �����ڂ����̒e
	for (int i= 0; i < mylib_const::BULLETAPPEARANCE_NUM; i++)
	{
		if (m_pBulletAppearance[i] != NULL)
		{
			continue;
		}
		m_pBulletAppearance[i] = CObjectBillboard::Create(GetPosition(), mylib_const::DEFAULT_VECTOR3);
		m_pBulletAppearance[i]->SetSize(D3DXVECTOR2(GetWidthLen(), GetHeightLen()));
		m_pBulletAppearance[i]->SetColor(D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f));
		m_pBulletAppearance[i]->BindTexture(nTex);
		m_pBulletAppearance[i]->SetType(CObject::TYPE_NONE);
	}


	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBullet::Uninit(void)
{
	// �폜
	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME && CGame::GetBulletManager() != NULL)
	{// �e�}�l�[�W���̍폜
		CGame::GetBulletManager()->Delete(m_nIdxBulletManager);
	}

	if (m_pMeshSphereEffect != NULL)
	{// ���b�V���X�t�B�A�̃G�t�F�N�g
		m_pMeshSphereEffect = NULL;
	}

	if (m_pEffectThunderRing != NULL)
	{// ���̃����O�̃G�t�F�N�g
		m_pEffectThunderRing = NULL;
	}

	// �����ڂ����̒e
	for (int i = 0; i < mylib_const::BULLETAPPEARANCE_NUM; i++)
	{
		if (m_pBulletAppearance[i] != NULL)
		{
			m_pBulletAppearance[i]->Uninit();
			m_pBulletAppearance[i] = NULL;
		}
	}

	// �I������
	CMeshSphere::Uninit();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CBullet::Update(void)
{
	// �ʒu�X�V
	UpdatePos();

	// ��ԕʏ���
	(this->*(m_FuncList[m_state]))();

	// �����蔻�菈��
	(this->*(m_CollisionFuncList[m_type]))();

	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	if (m_type == TYPE_ENEMY)
	{
		m_nCntEmission = (m_nCntEmission + 1) % 100;	// �������̃J�E���^�[

		if (m_nCntEmission == 10 && m_pMeshSphereEffect != NULL)
		{
			float fSize = GetWidthLen();
			m_pMeshSphereEffect->SetSizeDest(fSize + 10.0f);
		}
		if (m_nCntEmission == 15 && m_pMeshSphereEffect != NULL)
		{
			m_pMeshSphereEffect->Uninit();
			m_pMeshSphereEffect = NULL;
		}

		if (m_nCntEmission == 0)
		{
			if (m_pMeshSphereEffect != NULL)
			{
				m_pMeshSphereEffect->Uninit();
				m_pMeshSphereEffect = NULL;
			}

			float fSize = GetWidthLen();

			// ���b�V���X�t�B�A����
			m_pMeshSphereEffect = CMeshSphere::Create(GetPosition(), fSize, m_nTexIdx, 7);

			// ���ݒ�
			m_pMeshSphereEffect->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.7f));
			m_pMeshSphereEffect->SetWidthLen(fSize);
			m_pMeshSphereEffect->SetHeightLen(fSize);
			m_pMeshSphereEffect->SetSizeDest(fSize + 100.0f);

			if (m_pEffectThunderRing != NULL)
			{// ���̃����O�̃G�t�F�N�g
				m_pEffectThunderRing->Uninit();
				m_pEffectThunderRing = NULL;
			}

			// ���̃����O����
			m_pEffectThunderRing = CThunderRing::Create(GetPosition(), D3DXVECTOR2(fSize + 260.0f, fSize + 260.0f));

			// �����ŏ����Ȃ��悤�ɂ���
			m_pEffectThunderRing->SetDesableAutoDeath();
		}
	}
	else if (m_type == TYPE_PLAYER)
	{
		// ���a
		float fSize = GetWidthLen();

		if (m_pMeshSphereEffect == NULL)
		{
			// ���b�V���X�t�B�A����
			m_pMeshSphereEffect = CMeshSphere::Create(GetPosition(), fSize, m_nTexIdx, 7);

			// ���ݒ�
			m_pMeshSphereEffect->SetColor(D3DXCOLOR(1.0f, 0.2f, 1.0f, 0.7f));
			m_pMeshSphereEffect->SetWidthLen(fSize);
			m_pMeshSphereEffect->SetHeightLen(fSize);
			m_pMeshSphereEffect->SetSizeDest(fSize + 100.0f);
		}

		if (m_pEffectThunderRing == NULL)
		{
			// ���̃����O����
			m_pEffectThunderRing = CThunderRing::Create(GetPosition(), D3DXVECTOR2(fSize + 360.0f, fSize + 360.0f));

			// �F�ݒ�
			m_pEffectThunderRing->SetColor(D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f));

			// �����ŏ����Ȃ��悤�ɂ���
			m_pEffectThunderRing->SetDesableAutoDeath();
		}
	}

	if (m_pMeshSphereEffect != NULL)
	{
		m_pMeshSphereEffect->SetPosition(GetPosition());
	}

	if (m_pEffectThunderRing != NULL)
	{// ���̃����O�̃G�t�F�N�g

		if (m_pEffectThunderRing->IsFinish() == true)
		{
			m_pEffectThunderRing->Uninit();
			m_pEffectThunderRing = NULL;
		}
		else
		{
			m_pEffectThunderRing->SetPosition(GetPosition());
		}
	}

	// �����ڂ����̒e�̍X�V
	for (int i = 0; i < mylib_const::BULLETAPPEARANCE_NUM; i++)
	{
		if (m_pBulletAppearance[i] == NULL)
		{
			continue;
		}
		m_pBulletAppearance[i]->Update();
		m_pBulletAppearance[i]->SetPosition(GetPosition());
	}

	// �������Z
	m_nLife--;

	if (m_nLife <= 0)
	{// �������s������

		// �e�̍폜
		if (m_pMeshSphereEffect != NULL)
		{
			m_pMeshSphereEffect->Uninit();
			m_pMeshSphereEffect = NULL;
		}

		if (m_pEffectThunderRing != NULL)
		{// ���̃����O�̃G�t�F�N�g
			m_pEffectThunderRing->Uninit();
			m_pEffectThunderRing = NULL;
		}

		if (m_type == TYPE_PLAYER)
		{
			// �e�̐�������
			CBulletExplosion::Create(GetPosition(), GetWidthLen() + 200.0f);
		}
		else
		{
			// �����̐���
			CExplosion::Create(GetPosition());
		}

		Uninit();
		return;
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �ړ�����
//==========================================================================
void CBullet::UpdatePos(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ʒu�X�V
	pos += move;

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
void CBullet::StateNone(void)
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
void CBullet::StateDamage(void)
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
		m_state = STATE_NONE;

		if (m_pMeshSphereEffect != NULL)
		{
			m_pMeshSphereEffect->Uninit();
			m_pMeshSphereEffect = NULL;
		}

		if (m_pEffectThunderRing != NULL)
		{// ���̃����O�̃G�t�F�N�g
			m_pEffectThunderRing->Uninit();
			m_pEffectThunderRing = NULL;
		}
	}
}

//==========================================================================
// �v���C���[�Ƃ̔���
//==========================================================================
void CBullet::CollisionPlayer(void)
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
		float fRadius = GetWidthLen();

		if (SphereRange(pos, PlayerPosition, fRadius, fPlayerRadius))
		{// �������Ă�����

			// �q�b�g����
			pPlayer->Hit(1);

			if (m_pMeshSphereEffect != NULL)
			{
				m_pMeshSphereEffect->Uninit();
				m_pMeshSphereEffect = NULL;
			}

			if (m_pEffectThunderRing != NULL)
			{// ���̃����O�̃G�t�F�N�g
				m_pEffectThunderRing->Uninit();
				m_pEffectThunderRing = NULL;
			}
			// �I������
			Uninit();
			break;
		}
	}

}

//==========================================================================
// �G�Ƃ̔���
//==========================================================================
void CBullet::CollisionEnemy(void)
{
	// �G�}�l�[�W���擾
	CEnemyManager *pEnemyManager = CGame::GetEnemyManager();
	if (pEnemyManager == NULL)
	{// NULL��������
		return;
	}

	// �G���擾
	CEnemy **ppEnemy = pEnemyManager->GetEnemy();
	int nNumEnemy = pEnemyManager->GetNumAll();

	// ���擾
	D3DXVECTOR3 pos = GetPosition();
	float fRadius = GetWidthLen();
	bool bHit = false;

	int nUse = 0;
	for (int nCntEnemy = 0; nUse < nNumEnemy; nCntEnemy++)
	{
		if (ppEnemy[nCntEnemy] == NULL)
		{// NULL��������
			continue;
		}
		nUse++;

		// �G�̏��擾
		D3DXVECTOR3 EnemyPosition = ppEnemy[nCntEnemy]->GetCenterPosition();
		float fEnemyRadius = ppEnemy[nCntEnemy]->GetRadius();

		if (SphereRange(pos, EnemyPosition, fRadius, fEnemyRadius))
		{// �������Ă�����
			bHit = true;
			break;
		}
	}

	if (bHit == true)
	{// �������Ă���

		if (m_pMeshSphereEffect != NULL)
		{
			m_pMeshSphereEffect->Uninit();
			m_pMeshSphereEffect = NULL;
		}

		if (m_pEffectThunderRing != NULL)
		{// ���̃����O�̃G�t�F�N�g
			m_pEffectThunderRing->Uninit();
			m_pEffectThunderRing = NULL;
		}

		// �e�̐�������
		CBulletExplosion::Create(pos, fRadius + 200.0f);

		// �I������
		Uninit();
		return;
	}

}

//==========================================================================
// �v���C���[�e�̍X�V
//==========================================================================
void CBullet::UpdateTypePlayer(void)
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CBullet::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	// �r���{�[�h�̕`��
	CMeshSphere::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);




	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	// �����ڂ����̒e�̕`��
	for (int i = 0; i < mylib_const::BULLETAPPEARANCE_NUM; i++)
	{
		if (m_pBulletAppearance[i] == NULL)
		{
			continue;
		}
		m_pBulletAppearance[i]->Draw();
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CBullet::SetVtx(void)
{
	// ���_�ݒ�
	CMeshSphere::SetVtx();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CBullet::SetState(STATE state, int nCntState)
{
	m_state = state;			// ���
	m_nCntState = nCntState;	// ��ԑJ�ڃJ�E���^�[
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CBullet::STATE CBullet::GetState(void)
{
	return m_state;
}

//==========================================================================
// ��ސݒ�
//==========================================================================
void CBullet::SetType(TYPE type)
{
	m_type = type;

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_type]);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);
}

//==========================================================================
// ��ގ擾
//==========================================================================
CBullet::TYPE CBullet::GetType(void)
{
	return m_type;
}

//==========================================================================
// �����擾
//==========================================================================
int CBullet::GetNumAll(void)
{
	return m_nNumAll;
}
