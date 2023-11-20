//=============================================================================
// 
// �G�t�F�N�g���� [2D_effect.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EFFECT_2D_H_
#define _EFFECT_2D_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �e�N���X��`
class CEffect2D : public CObject2D
{
public:

	// �񋓌^��`
	typedef enum
	{
		MOVEEFFECT_ADD = 0,		// ���Z
		MOVEEFFECT_SUB,			// ���Z
		MOVEEFFECT_SUPERSUB,	// �����Z
		MOVEEFFECT_NONE,		// �ω��Ȃ�
		MOVEEFFECT_GENSUI,		// ����
		MOVEEFFECT_MAX
	}MOVEEFFECT;

	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�G�t�F�N�g
		TYPE_SMOKE,			// ���G�t�F�N�g
		TYPE_SMOKEBLACK,	// ����
		TYPE_BLACK,			// ���G�t�F�N�g
		TYPE_JUJI,			// �\���G�t�F�N�g
		TYPE_JUJI2,			// �\���G�t�F�N�g
		TYPE_MAX
	}TYPE;

	CEffect2D(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CEffect2D();

	static HRESULT Load(void);
	static void Unload(void);
	static CEffect2D *Create(void);
	static CEffect2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type);
	static CEffect2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 rot, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type);

	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	static int GetNumAll(void);

private:
	void SubSize(void);
	void SuperSubSize(void);
	void AddSize(void);
	void Gensui(void);

	D3DXCOLOR m_colOrigin;	// �F
	float m_fRadius;		// ���a
	float m_fMaxRadius;		// �ő唼�a
	int m_nLife;			// ����
	int m_nMaxLife;			// �ő����(�Œ�)
	int m_moveType;			// �ړ��̎��
	bool m_bAddAlpha;		// ���Z�����̔���
	TYPE m_nType;			// ���
	static const char *m_apTextureFile[];			// �e�N�X�`���̃t�@�C��
	static int m_nNumAll;	// ����
	int m_nTexIdx[TYPE_MAX];				// �e�N�X�`���̃C���f�b�N�X�ԍ�
};

#endif