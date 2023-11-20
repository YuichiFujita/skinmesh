//=============================================================================
// 
// �G�t�F�N�g���� [3D_effect.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EFFECT_3D_H_
#define _EFFECT_3D_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �e�N���X��`
class CEffect3D : public CObjectBillboard
{
public:

	// �񋓌^��`
	typedef enum
	{
		MOVEEFFECT_ADD = 0,		// ���Z
		MOVEEFFECT_SUB,			// ���Z
		MOVEEFFECT_SUPERSUB,	// �����Z
		MOVEEFFECT_NONE,		// �ω��Ȃ�
		MOVEEFFECT_MAX
	}MOVEEFFECT;

	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�G�t�F�N�g
		TYPE_SMOKE,			// ���G�t�F�N�g
		TYPE_SMOKEBLACK,	// ����
		TYPE_BLACK,			// ���G�t�F�N�g
		TYPE_JUJI,			// �\���G�t�F�N�g
		TYPE_SAND,			// ���G�t�F�N�g
		TYPE_SANDBLACK,		// ���G�t�F�N�g
		TYPE_LINE,			// ���G�t�F�N�g
		TYPE_GRASS,			// ���G�t�F�N�g
		TYPE_SWEAT,			// ���G�t�F�N�g
		TYPE_THUNDER,		// ���G�t�F�N�g
		TYPE_MAX
	}TYPE;

	CEffect3D(int nPriority = 5);
	~CEffect3D();

	static CEffect3D *Create(void);
	static CEffect3D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type, const float fAddSizeValue = 0.0f);
	static void LoadTexture(void);	// �e�N�X�`���ǂݍ���

	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	void SetUp(D3DXVECTOR3 setup, CObject *pObj, int nParentIdx);	// �Z�b�g�A�b�v
	void UpdatePosition(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �ʒu�X�V
	void UninitParent(void);	// �e�̔j��
	static int GetNumAll(void);	// �����擾

private:

	// �����o�֐�
	void SubSize(void);
	void SuperSubSize(void);
	void AddSize(void);

	// �����o�ϐ�
	D3DXVECTOR3 m_posOrigin;		// ���_
	D3DXVECTOR3 m_updatePosition;	// �X�V��̈ʒu
	D3DXVECTOR3 m_setupPosition;	// �Z�b�g�A�b�v�ʒu
	D3DXCOLOR m_colOrigin;		// �F
	float m_fRadius;			// ���a
	float m_fMaxRadius;			// �ő唼�a
	float m_fAddSizeValue;		// �T�C�Y�ύX��
	float m_fSetupRotation;		// �Z�b�g�A�b�v�̌���
	float m_fSetupVec;			// �Z�b�g�A�b�v�̋���
	int m_nLife;				// ����
	int m_nMaxLife;				// �ő����(�Œ�)
	int m_moveType;				// �ړ��̎��
	int m_nParentIdx;			// �e�̃C���f�b�N�X�ԍ�
	bool m_bAddAlpha;			// ���Z�����̔���
	TYPE m_nType;				// ���
	CObject *m_pParent;			// �e�̃|�C���^
	static const char *m_apTextureFile[];			// �e�N�X�`���̃t�@�C��
	static int m_nNumAll;	// ����
	static int m_nTexIdx[TYPE_MAX];				// �e�N�X�`���̃C���f�b�N�X�ԍ�
};

#endif