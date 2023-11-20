//=============================================================================
// 
//  �e���� [explosion.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE			"data\\TEXTURE\\explosion_01.png"
#define WIDTH			(250.0f)							// ����
#define HEIGHT			(250.0f)							// �c��
#define ANIM_SPEED		(1)								// �ǂݍ��݊Ԋu
#define MAX_PATTERN_U	(14)							// U�̕�����
#define MAX_PATTERN_V	(1)								// V�̕�����
#define MAX_PATTERN		(MAX_PATTERN_U)					// �A�j���[�V�����p�^�[���̍ő吔
#define MOVE_U			(1.0f / (float)MAX_PATTERN_U)	// U���W�ړ���
#define MOVE_V			(1.0f / (float)MAX_PATTERN_V)	// V���W�ړ���

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CExplosion::m_nNumAll = 0;		// �e�̑���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CExplosion::CExplosion(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_nCntAnim = 0;			// �A�j���[�V�����J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����p�^�[��No.
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CExplosion::~CExplosion()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CExplosion *CExplosion::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CExplosion *pExplosion = NULL;

	if (pExplosion == NULL)
	{// NULL��������

		// �������̊m��
		pExplosion = DEBUG_NEW CExplosion;

		if (pExplosion != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pExplosion->Init();

			// ������
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_EXPLOSION);
		}

		return pExplosion;
	}

	return NULL;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CExplosion *CExplosion::Create(const D3DXVECTOR3 pos)
{
	// �����p�̃I�u�W�F�N�g
	CExplosion *pExplosion = NULL;

	if (pExplosion == NULL)
	{// NULL��������

		// �������̊m��
		pExplosion = DEBUG_NEW CExplosion;

		if (pExplosion != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pExplosion->SetPosition(pos);

			// ����������
			pExplosion->Init();

			// ������
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_EXPLOSION);
		}

		return pExplosion;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CExplosion::Init(void)
{
	// �e��ϐ��̏�����
	SetSize(D3DXVECTOR2(0.0f, 0.0f));	// �T�C�Y
	m_nCntAnim = 0;			// �A�j���[�V�����J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����p�^�[��No.

	// ��ނ̐ݒ�
	SetType(TYPE_EXPLOSION);

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// ����������
	CObjectBillboard::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CExplosion::Uninit(void)
{
	// �I������
	CObjectBillboard::Uninit();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CExplosion::Update(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �T�C�Y
	D3DXVECTOR2 size = GetSize();

	// �g��
	size.x += (WIDTH - size.x) * 0.2f;
	size.y += (WIDTH - size.y) * 0.2f;

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

	// �ʒu�ݒ�
	SetPosition(pos);

	// �T�C�Y�ݒ�
	SetSize(size);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CExplosion::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �r���{�[�h�̕`��
	CObjectBillboard::Draw();

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
void CExplosion::SetVtx(void)
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
int CExplosion::GetNumAll(void)
{
	return m_nNumAll;
}
