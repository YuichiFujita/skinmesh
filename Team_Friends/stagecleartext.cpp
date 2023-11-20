//=============================================================================
// 
// �X�e�[�W�N���A�e�L�X�g���� [stagecleartext.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "stagecleartext.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE	"data\\TEXTURE\\stageclear_01.png"
#define TIME_SCALEUP		(25)
#define TIME_SCALEDOWN		(10)
#define TIME_SCALENONE		(10)
#define TIME_FADEOUT		(120)
#define SIZE_SCALEORIGIN	(0.3f)
#define SIZE_SCALEUP		(0.6f)
#define SIZE_SCALEDOWN		(0.25f)

//==========================================================================
// �֐��|�C���^
//==========================================================================
CStageClearText::STATE_FUNC CStageClearText::m_StateFuncList[] =
{
	&CStageClearText::StateNone,
	&CStageClearText::StateScaleUP,
	&CStageClearText::StateScaleDOWN,
	&CStageClearText::StateScaleNone,
	&CStageClearText::StateFadeOut,
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStageClearText::CStageClearText(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nCntState = 0;		// ��ԑJ�ڃJ�E���^�[
	m_state = STATE_NONE;	// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStageClearText::~CStageClearText()
{

}

//==========================================================================
// ��������
//==========================================================================
CStageClearText *CStageClearText::Create(const D3DXVECTOR3 pos)
{
	// �����p�̃I�u�W�F�N�g
	CStageClearText *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CStageClearText;

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

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
HRESULT CStageClearText::Init(void)
{
	HRESULT hr;

	// ����������
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// �T�C�Y�擾
	D3DXVECTOR2 size = CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx) * SIZE_SCALEORIGIN;

	// �T�C�Y�ݒ�
	SetSize(D3DXVECTOR2(0.0f, 0.0f));
	SetSizeOrigin(size);

	m_nCntState = TIME_SCALEUP;
	m_state = STATE_SCALEUP;

	return S_OK;
}

//==================================================================================
// �I������
//==================================================================================
void CStageClearText::Uninit(void)
{
	// �I������
	CObject2D::Uninit();
}

//==================================================================================
// �X�V����
//==================================================================================
void CStageClearText::Update(void)
{
	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();

	if (IsDeath())
	{
		return;
	}

	// ���_���W�̐ݒ�
	SetVtx();
}

//==================================================================================
// �����Ȃ����
//==================================================================================
void CStageClearText::StateNone(void)
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
	}
}

//==================================================================================
// �g����
//==================================================================================
void CStageClearText::StateScaleUP(void)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = GetSize();
	D3DXVECTOR2 DestImageSize = CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx) * SIZE_SCALEUP;

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// ���X�ɉ���
	size.x = EasingEaseIn(0.0f, DestImageSize.x, 1.0f - (float)m_nCntState / (float)TIME_SCALEUP);
	size.y = EasingEaseIn(0.0f, DestImageSize.y, 1.0f - (float)m_nCntState / (float)TIME_SCALEUP);

	if (m_nCntState <= 0)
	{
		m_nCntState = TIME_SCALEDOWN;
		m_state = STATE_SCALEDOWN;
	}

	// �T�C�Y�ݒ�
	SetSize(size);
}

//==================================================================================
// �k�����
//==================================================================================
void CStageClearText::StateScaleDOWN(void)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = GetSize();
	D3DXVECTOR2 ImageSize = CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx);

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// ���X�Ɍ���
	size.x = EasingEaseOut(ImageSize.x * SIZE_SCALEUP, ImageSize.x * SIZE_SCALEDOWN, 1.0f - (float)m_nCntState / (float)TIME_SCALEDOWN);
	size.y = EasingEaseOut(ImageSize.y * SIZE_SCALEUP, ImageSize.y * SIZE_SCALEDOWN, 1.0f - (float)m_nCntState / (float)TIME_SCALEDOWN);

	if (m_nCntState <= 0)
	{
		m_nCntState = TIME_SCALENONE;
		m_state = STATE_SCALENONE;
	}

	// �T�C�Y�ݒ�
	SetSize(size);
}

//==================================================================================
// �������
//==================================================================================
void CStageClearText::StateScaleNone(void)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = GetSize();
	D3DXVECTOR2 sizeOrigin = GetSizeOrigin();
	D3DXVECTOR2 ImageSize = CManager::GetInstance()->GetTexture()->GetImageSize(m_nTexIdx);

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// ���X�Ɍ���
	size.x = EasingEaseOut(ImageSize.x * SIZE_SCALEDOWN, sizeOrigin.x, 1.0f - (float)m_nCntState / (float)TIME_SCALENONE);
	size.y = EasingEaseOut(ImageSize.y * SIZE_SCALEDOWN, sizeOrigin.y, 1.0f - (float)m_nCntState / (float)TIME_SCALENONE);

	if (m_nCntState <= 0)
	{
		m_nCntState = TIME_FADEOUT;
		m_state = STATE_FADEOUT;
	}

	// �T�C�Y�ݒ�
	SetSize(size);
}

//==================================================================================
// �t�F�[�h�A�E�g���
//==================================================================================
void CStageClearText::StateFadeOut(void)
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
		Uninit();
		return;
	}
}

//==================================================================================
// �`�揈��
//==================================================================================
void CStageClearText::Draw(void)
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
void CStageClearText::SetVtx(void)
{
	// ���_�ݒ�
	CObject2D::SetVtx();
}

