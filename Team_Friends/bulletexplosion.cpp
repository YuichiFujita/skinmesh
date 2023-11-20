//=============================================================================
// 
//  �e�������� [bulletexplosion.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "bulletexplosion.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"
#include "enemy.h"
#include "calculation.h"
#include "game.h"
#include "player.h"
#include "effect_thunderring.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE			"data\\TEXTURE\\sunder_03.png"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CBulletExplosion::m_nNumAll = 0;		// �e�̑���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBulletExplosion::CBulletExplosion(int nPriority) : CMeshSphere(nPriority)
{
	// �l�̃N���A
	m_nLife = 0;	// ����
	m_nLifeMax = 0;	// �����̍ő�l
	m_nTexIdx = 0;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_fDestSize = 0.0f;		// �ڕW�̃T�C�Y
	rotmove = mylib_const::DEFAULT_VECTOR3;	// �����̈ړ���
	m_pEffectThunderRing = NULL;	// ���̃����O�̃G�t�F�N�g
	memset(&m_pBulletAppearance[0], NULL, sizeof(m_pBulletAppearance));	// �����ڂ����̒e

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBulletExplosion::~CBulletExplosion()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBulletExplosion *CBulletExplosion::Create(const D3DXVECTOR3 pos, const float fSize)
{
	// �����p�̃I�u�W�F�N�g
	CBulletExplosion *pBullet = NULL;

	if (pBullet == NULL)
	{// NULL��������

		// �������̊m��
		pBullet = DEBUG_NEW CBulletExplosion;

		if (pBullet != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pBullet->SetPosition(pos);

			// �T�C�Y�ݒ�
			pBullet->m_fDestSize = fSize;		// �ڕW�̃T�C�Y
			pBullet->SetWidthLen(0.0f);
			pBullet->SetHeightLen(0.0f);
			pBullet->SetWidthBlock(16);
			pBullet->SetHeightBlock(16);

			// ���˕Ԃ��q�b�g
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_BOUNCEHIT);

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
HRESULT CBulletExplosion::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_nLifeMax = 40;
	m_nLife = m_nLifeMax;	// ����
	SetColor(D3DXCOLOR(1.0f, 0.2f, 1.0f, 0.7f));
	rotmove.x = D3DX_PI * (Random(50, 100) * 0.001f);
	rotmove.y = D3DX_PI * (Random(50, 100) * 0.001f);
	rotmove.z = rotmove.x;

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

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

	if (m_pEffectThunderRing == NULL)
	{
		// ���a
		float fSize = GetWidthLen();

		// ���̃����O����
		m_pEffectThunderRing = CThunderRing::Create(GetPosition(), D3DXVECTOR2(fSize + 150.0f, fSize + 150.0f));

		// �F�ݒ�
		m_pEffectThunderRing->SetColor(D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f));

		// �����ŏ����Ȃ��悤�ɂ���
		m_pEffectThunderRing->SetDesableAutoDeath();
	}

	// �e�N�X�`���̊��蓖��
	int nTex = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\effect\\effect000.jpg");

	// �����ڂ����̒e
	for (int i = 0; i < mylib_const::BULLETAPPEARANCE_NUM; i++)
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
void CBulletExplosion::Uninit(void)
{
	if (m_pEffectThunderRing != NULL)
	{// ���̃����O�̃G�t�F�N�g

		// �I������
		m_pEffectThunderRing->Uninit();
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
void CBulletExplosion::Update(void)
{
	// �ʒu�X�V
	UpdatePos();

	// �����蔻��
	CollisionEnemy();

	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	if (m_pEffectThunderRing != NULL)
	{// ���̃����O�̃G�t�F�N�g

		// �ʒu�X�V
		m_pEffectThunderRing->SetPosition(GetPosition());

		// �T�C�Y�X�V
		float fSize = GetWidthLen();
		m_pEffectThunderRing->SetSize(D3DXVECTOR2(fSize + 550.0f, fSize + 550.0f));
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
		float fSize = GetWidthLen();
		m_pBulletAppearance[i]->SetSize(D3DXVECTOR2(fSize, fSize) * 0.9f);
	}

	// �������Z
	m_nLife--;

	if (m_nLife <= 0)
	{// �������s������
		Uninit();
		return;
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �ړ�����
//==========================================================================
void CBulletExplosion::UpdatePos(void)
{
	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �T�C�Y�擾
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	// �ڕW�̃T�C�Y�֍X�V
	fWidthLen += (m_fDestSize - fWidthLen) * 0.25f;
	fHeightLen += (m_fDestSize - fHeightLen) * 0.25f;

	// ��]
	rot += rotmove;

	// �����ݒ�
	SetRotation(rot);

	// �T�C�Y�ݒ�
	SetWidthLen(fWidthLen);
	SetHeightLen(fHeightLen);
}

//==========================================================================
// �G�Ƃ̔���
//==========================================================================
void CBulletExplosion::CollisionEnemy(void)
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

	int nUse = 0;
	for (int nCntEnemy = 0; nUse < nNumEnemy; nCntEnemy++)
	{
		if (ppEnemy[nCntEnemy] == NULL)
		{// NULL��������
			continue;
		}
		nUse++;

		// �G�̏��擾
		D3DXVECTOR3 EnemyPosition = ppEnemy[nCntEnemy]->GetPosition();
		float fEnemyRadius = ppEnemy[nCntEnemy]->GetRadius();

		if (SphereRange(pos, EnemyPosition, fRadius, fEnemyRadius))
		{// �������Ă�����

			// �q�b�g����
			ppEnemy[nCntEnemy]->Hit(mylib_const::DMG_BOUNCE);
		}
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CBulletExplosion::Draw(void)
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
void CBulletExplosion::SetVtx(void)
{
	// ���_�ݒ�
	CMeshSphere::SetVtx();
}

//==========================================================================
// �����擾
//==========================================================================
int CBulletExplosion::GetNumAll(void)
{
	return m_nNumAll;
}
