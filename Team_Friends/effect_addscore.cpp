//=============================================================================
// 
//  �e���� [effect_addscore.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "effect_addscore.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"
#include "calculation.h"
#include "game.h"
#include "score.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE			"data\\TEXTURE\\effect\\Star01.png"
#define SIZE			(30.0f)							// �T�C�Y
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
CEffectAddScore::CEffectAddScore(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_state;				// ���
	m_nCntState = 0;			// ��ԑJ�ڃJ�E���^�[
	m_nLife = 0;				// ����
	m_nLifeMax = 0;				// �����̍ő�l
	m_nTexIdx = 0;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	memset(&m_pEffect[0], NULL, sizeof(m_pEffect));	// �����ڂ����̒e
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEffectAddScore::~CEffectAddScore()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CEffectAddScore *CEffectAddScore::Create(const D3DXVECTOR3 pos)
{
	// �����p�̃I�u�W�F�N�g
	CEffectAddScore *pBullet = NULL;

	if (pBullet == NULL)
	{// NULL��������

		// �������̊m��
		pBullet = DEBUG_NEW CEffectAddScore;

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
HRESULT CEffectAddScore::Init(void)
{
	// �e��ϐ��̏�����
	m_nLifeMax = 60 * 5;
	m_nLife = m_nLifeMax;	// ����

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_BALLAST);

	// �����ڂ����̒e
	float fff = (D3DX_PI * 2.0f) / ADDSCORE_CIRCLE;
	for (int nCntCircle = 0; nCntCircle < ADDSCORE_CIRCLE; nCntCircle++)
	{
		if (m_pEffect[nCntCircle] != NULL)
		{
			continue;
		}

		// ����̌���
		float fRot = fff * nCntCircle;

		// �o���o������
		float fRotRand = (float)Random(-10, 10) / 100.0f;

		float fMove = Random(100, 200) * 0.1f;
		fMove *= 2.5f;

		//�ړ��ʂ̐ݒ�
		D3DXVECTOR3 move;
		move.x = sinf(fRot + fRotRand) * fMove;
		move.z = 0.0f;
		move.y = cosf(fRot + fRotRand) * fMove;

		float fRadius = 45.0f;

		// �����_���Ŕ��a����
		int nRand = Random(0, 2);
		if (nRand == 0)
		{
			fRadius *= 0.95f;
		}
		else if (nRand == 1)
		{
			fRadius *= 0.98f;
		}
		else if (nRand == 2)
		{
			fRadius *= 0.92f;
		}

		m_pEffect[nCntCircle] = CObject2D::Create(7);
		m_pEffect[nCntCircle]->SetPosition(GetPosition());
		m_pEffect[nCntCircle]->SetMove(move);
		m_pEffect[nCntCircle]->SetSize(D3DXVECTOR2(fRadius, fRadius));
		m_pEffect[nCntCircle]->SetColor(D3DXCOLOR(1.0f, 0.2f, 1.0f, 1.0f));
		m_pEffect[nCntCircle]->BindTexture(m_nTexIdx);
		m_pEffect[nCntCircle]->SetType(CObject::TYPE_NONE);
	}


	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEffectAddScore::Uninit(void)
{
	for (int nCntCircle = 0; nCntCircle < ADDSCORE_CIRCLE; nCntCircle++)
	{
		if (m_pEffect[nCntCircle] == NULL)
		{
			continue;
		}
		m_pEffect[nCntCircle]->Uninit();
		m_pEffect[nCntCircle] = NULL;
	}

	// �I������
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEffectAddScore::Update(void)
{
	// �ʒu�X�V
	UpdatePos();
}

//==========================================================================
// �ړ�����
//==========================================================================
void CEffectAddScore::UpdatePos(void)
{
	// �X�R�A�̈ʒu�擾
	D3DXVECTOR3 ScorePos = CGame::GetScore()->GetPosition();

	for (int nCntCircle = 0; nCntCircle < ADDSCORE_CIRCLE; nCntCircle++)
	{
		if (m_pEffect[nCntCircle] == NULL)
		{
			continue;
		}

		// ���擾
		D3DXVECTOR3 pos = m_pEffect[nCntCircle]->GetPosition();
		D3DXVECTOR3 rot = m_pEffect[nCntCircle]->GetRotation();
		D3DXVECTOR3 move = m_pEffect[nCntCircle]->GetMove();
		D3DXVECTOR2 size = m_pEffect[nCntCircle]->GetSize();

		// �ڕW�̊p�x�����߂�
		float fRotDest = atan2f((pos.x - ScorePos.x), (pos.y - ScorePos.y));

		// �ڕW�Ƃ̍���
		float fRotDiff = fRotDest - rot.z;
		RotNormalize(fRotDiff);

		//�p�x�̕␳������
		rot.z += fRotDiff * 0.1f;

		// �p�x�̐��K��
		RotNormalize(rot.z);

		// �ړ��ʕ␳
		move.x += ((sinf(D3DX_PI + rot.z) * 6.0f) - move.x) * 0.15f;
		move.y += ((cosf(D3DX_PI + rot.z) * 6.0f) - move.y) * 0.15f;

		// �ʒu�X�V
		pos += move;

		if (CircleRange2D(pos, ScorePos, size.x, 50.0f))
		{
			m_pEffect[nCntCircle]->Uninit();
			m_pEffect[nCntCircle] = NULL;
			continue;
		}

		// ���ݒ�
		m_pEffect[nCntCircle]->SetPosition(pos);
		m_pEffect[nCntCircle]->SetRotation(rot);
		m_pEffect[nCntCircle]->SetMove(move);
		m_pEffect[nCntCircle]->SetSize(size);

		m_pEffect[nCntCircle]->Update();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CEffectAddScore::Draw(void)
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

	for (int nCntCircle = 0; nCntCircle < ADDSCORE_CIRCLE; nCntCircle++)
	{
		if (m_pEffect[nCntCircle] == NULL)
		{
			continue;
		}
		m_pEffect[nCntCircle]->Draw();
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

