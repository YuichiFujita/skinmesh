//=============================================================================
// 
// �J�[�\������ [cursor.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "cursor.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "fade.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MOVE	(1.0f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CCursor::m_apTextureFile[mylib_const::MAX_PLAYER] =	// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\000.png",
	"data\\TEXTURE\\001.png",
	"data\\TEXTURE\\002.png",
	"data\\TEXTURE\\003.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCursor::CCursor(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nMyPlayerIdx = 0;	// �v���C���[�C���f�b�N�X�ԍ�
	m_nTexIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nSelectIdx = 0;	// �I�����̃C���f�b�N�X�ԍ�
	m_bDecide = false;	// ���܂������ǂ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCursor::~CCursor()
{

}

//==========================================================================
// ��������
//==========================================================================
CCursor *CCursor::Create(const D3DXVECTOR3 pos, int nIdx)
{
	// �����p�̃I�u�W�F�N�g
	CCursor *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CCursor;

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// �v���C���[�C���f�b�N�X
			pEffect->m_nMyPlayerIdx = nIdx;

			// ����������
			pEffect->Init();
			pEffect->SetPosition(pos);
		}

		return pEffect;
	}

	return NULL;
}

//==================================================================================
// ����������
//==================================================================================
HRESULT CCursor::Init(void)
{
	HRESULT hr;

	// ����������
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	m_nSelectIdx = -1;

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(m_apTextureFile[m_nMyPlayerIdx]);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// �T�C�Y�擾
	D3DXVECTOR2 size = CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx) * 0.2f;

	// �T�C�Y�ݒ�
	SetSize(size);

	return S_OK;
}

//==================================================================================
// �I������
//==================================================================================
void CCursor::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//==================================================================================
// �X�V����
//==================================================================================
void CCursor::Update(void)
{

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{
		return;
	}

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();


	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x < 0)
	{//���L�[�������ꂽ,���ړ�

		if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
		{//A+W,����ړ�
			move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
			move.y += cosf(-D3DX_PI * 0.75f) * MOVE;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
		{//A+S,�����ړ�
			move.x += sinf(-D3DX_PI * 0.25f) * MOVE;
			move.y += cosf(-D3DX_PI * 0.25f) * MOVE;
		}
		else
		{//A,���ړ�
			move.x += sinf(-D3DX_PI * 0.5f) * MOVE;
			move.y += cosf(-D3DX_PI * 0.5f) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).x > 0)
	{//D�L�[�������ꂽ,�E�ړ�

		if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
		{//D+W,�E��ړ�
			move.x += sinf(D3DX_PI * 0.75f) * MOVE;
			move.y += cosf(D3DX_PI * 0.75f) * MOVE;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
		{//D+S,�E���ړ�
			move.x += sinf(D3DX_PI * 0.25f) * MOVE;
			move.y += cosf(D3DX_PI * 0.25f) * MOVE;
		}
		else
		{//D,�E�ړ�
			move.x += sinf(D3DX_PI * 0.5f) * MOVE;
			move.y += cosf(D3DX_PI * 0.5f) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y > 0)
	{//W�������ꂽ�A��ړ�
		move.x += sinf(D3DX_PI * 1.0f) * MOVE;
		move.y += cosf(D3DX_PI * 1.0f) * MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(m_nMyPlayerIdx).y < 0)
	{//S�������ꂽ�A���ړ�
		move.x += sinf(D3DX_PI * 0.0f) * MOVE;
		move.y += cosf(D3DX_PI * 0.0f) * MOVE;
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_B, m_nMyPlayerIdx))
	{// �L�����Z��
		m_bDecide = false;
	}

	// �ړ�
	pos += move;

	// ����
	move.x += (0.0f - move.x) * 0.1f;
	move.y += (0.0f - move.y) * 0.1f;

	// �ʒu�E�ړ���
	SetPosition(pos);
	SetMove(move);

	// ���_���W�̐ݒ�
	SetVtx();
}

//==================================================================================
// �`�揈��
//==================================================================================
void CCursor::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObject2D::Draw();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CCursor::SetVtx(void)
{
	// ���_�ݒ�
	CObject2D::SetVtx();
}

