//=============================================================================
// 
//  �G�X�|�[���G�t�F�N�g���� [effect_enemyspawn.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "effect_enemyspawn.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE			"data\\TEXTURE\\anim_enemyspawn.png"
#define SIZE			(300.0f)						// ����
#define ANIM_SPEED		(2)								// �ǂݍ��݊Ԋu
#define MAX_PATTERN_U	(15)							// U�̕�����
#define MAX_PATTERN_V	(1)								// V�̕�����
#define MAX_PATTERN		(MAX_PATTERN_U)					// �A�j���[�V�����p�^�[���̍ő吔
#define MOVE_U			(1.0f / (float)MAX_PATTERN_U)	// U���W�ړ���
#define MOVE_V			(1.0f / (float)MAX_PATTERN_V)	// V���W�ړ���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CEffectEnemySpawn::m_nTexIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEffectEnemySpawn::CEffectEnemySpawn(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_nCntAnim = 0;			// �A�j���[�V�����J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����p�^�[��No.
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEffectEnemySpawn::~CEffectEnemySpawn()
{
	
}

//==========================================================================
// �e�N�X�`���ǂݍ���
//==========================================================================
void CEffectEnemySpawn::LoadTexture(void)
{
	// �e�N�X�`���̓ǂݍ���
	CManager::GetInstance()->GetTexture()->Regist(TEXTURE);
}

//==========================================================================
// ��������
//==========================================================================
CEffectEnemySpawn *CEffectEnemySpawn::Create(const D3DXVECTOR3 pos)
{
	// �����p�̃I�u�W�F�N�g
	CEffectEnemySpawn *pExplosion = NULL;

	if (pExplosion == NULL)
	{// NULL��������

		// �������̊m��
		pExplosion = DEBUG_NEW CEffectEnemySpawn;

		if (pExplosion != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pExplosion->SetPosition(pos);

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
HRESULT CEffectEnemySpawn::Init(void)
{
	// �e��ϐ��̏�����
	SetSize(D3DXVECTOR2(SIZE, SIZE));	// �T�C�Y
	m_nCntAnim = 0;			// �A�j���[�V�����J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����p�^�[��No.
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ��ނ̐ݒ�
	SetType(TYPE_EXPLOSION);

	// �q�b�g�X�g�b�v���������I�u�W�F�N�g�Ƃ���
	SetEnableHitstopMove();

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
void CEffectEnemySpawn::Uninit(void)
{
	// �I������
	CObjectBillboard::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEffectEnemySpawn::Update(void)
{
	//// �ʒu�擾
	//D3DXVECTOR3 pos = GetPosition();

	//// �T�C�Y
	//D3DXVECTOR2 size = GetSize();

	//// �g��
	//size.x += (SIZE - size.x) * 0.1f;
	//size.y += (SIZE - size.y) * 0.1f;

	// �J�E���g���X�V
	m_nCntAnim = (m_nCntAnim + 1) % ANIM_SPEED;

	// �p�^�[���X�V
	if ((m_nCntAnim % ANIM_SPEED) == 0)
	{
		// �p�^�[��No.���X�V
		m_nPatternAnim = (m_nPatternAnim + 1) % MAX_PATTERN;

		if (m_nPatternAnim == 0)
		{// �p�^�[�������������

			// �I�u�W�F�N�g�j��
			Uninit();
			return;
		}
	}

	//// �ʒu�ݒ�
	//SetPosition(pos);

	//// �ʒu�ݒ�
	//SetSize(size);

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CEffectEnemySpawn::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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
void CEffectEnemySpawn::SetVtx(void)
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

