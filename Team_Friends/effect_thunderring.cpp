//=============================================================================
// 
//  ���̃����O�G�t�F�N�g���� [effect_thunderring.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "effect_thunderring.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE			"data\\TEXTURE\\anim_thunderring.png"
#define SIZE			(400.0f)						// ����
#define ANIM_SPEED		(1)								// �ǂݍ��݊Ԋu
#define MAX_PATTERN_U	(20)							// U�̕�����
#define MAX_PATTERN_V	(1)								// V�̕�����
#define MAX_PATTERN		(MAX_PATTERN_U)					// �A�j���[�V�����p�^�[���̍ő吔
#define MOVE_U			(1.0f / (float)MAX_PATTERN_U)	// U���W�ړ���
#define MOVE_V			(1.0f / (float)MAX_PATTERN_V)	// V���W�ړ���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CThunderRing::m_nNumAll = 0;		// �e�̑���
int CThunderRing::m_nTexIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CThunderRing::CThunderRing(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_nCntAnim = 0;			// �A�j���[�V�����J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����p�^�[��No.
	m_bAutoDeath = false;	// �����폜�̃t���O
	m_bFinish = false;		// �A�j���[�V�������I���������

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CThunderRing::~CThunderRing()
{
	
}

//==========================================================================
// �e�N�X�`���ǂݍ���
//==========================================================================
void CThunderRing::LoadTexture(void)
{
	// �e�N�X�`���̓ǂݍ���
	CManager::GetInstance()->GetTexture()->Regist(TEXTURE);
}

//==========================================================================
// ��������
//==========================================================================
CThunderRing *CThunderRing::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size)
{
	// �����p�̃I�u�W�F�N�g
	CThunderRing *pExplosion = NULL;

	if (pExplosion == NULL)
	{// NULL��������

		// �������̊m��
		pExplosion = DEBUG_NEW CThunderRing;

		if (pExplosion != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pExplosion->SetPosition(pos);

			// �T�C�Y�ݒ�
			pExplosion->SetSize(size);

			// ����������
			pExplosion->Init();
		}

		return pExplosion;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CThunderRing::Init(void)
{
	// �e��ϐ��̏�����
	m_nCntAnim = 0;			// �A�j���[�V�����J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����p�^�[��No.
	m_bAutoDeath = true;	// �����폜�ɂ���
	SetColor(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));

	// ��ނ̐ݒ�
	SetType(TYPE_EXPLOSION);

	// �e�N�X�`���̊��蓖��
	if (m_nTexIdx == 0)
	{
		m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);
	}

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// ����������
	CObjectBillboard::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CThunderRing::Uninit(void)
{
	// �I������
	CObjectBillboard::Uninit();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CThunderRing::Update(void)
{
	// �J�E���g���X�V
	m_nCntAnim = (m_nCntAnim + 1) % ANIM_SPEED;

	// �p�^�[���X�V
	if ((m_nCntAnim % ANIM_SPEED) == 0)
	{
		// �p�^�[��No.���X�V
		m_nPatternAnim = (m_nPatternAnim + 1) % MAX_PATTERN;

		if (m_nPatternAnim == 0)
		{// �p�^�[�������������

			// �I�����
			m_bFinish = true;

			if (m_bAutoDeath == true)
			{
				// �I�u�W�F�N�g�j��
				Uninit();
				return;
			}
		}
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CThunderRing::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// Z�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �r���{�[�h�̕`��
	CObjectBillboard::Draw();

	//// Z�e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CThunderRing::SetVtx(void)
{
	// ���_�ݒ�
	CObjectBillboard::SetVtx();

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * MOVE_U,			0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim + 1) * MOVE_U,	0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * MOVE_U,			1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim + 1) * MOVE_U,	1.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	GetVtxBuff()->Unlock();
}

//==========================================================================
// �����擾
//==========================================================================
int CThunderRing::GetNumAll(void)
{
	return m_nNumAll;
}
