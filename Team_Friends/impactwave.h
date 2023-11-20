//=============================================================================
// 
//  ���b�V���V�����_�[�w�b�_�[ [impactwave.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _IMPACTWAVE_H_
#define _IMPACTWAVE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���V�����_�[�N���X��`
class CImpactWave : public CObject3DMesh
{
public:
	//�񋓌^��`
	typedef enum
	{
		TYPE_BLACK = 0,
		TYPE_BLACK2,
		TYPE_ORANGE,
		TYPE_ORANGE2,
		TYPE_BLUE,
		TYPE_BLUE2,
		TYPE_GREEN,
		TYPE_GREEN2,
		TYPE_PURPLE,
		TYPE_PURPLE2,
		TYPE_PURPLE3,
		TYPE_PURPLE4,
		TYPE_PINK,
		TYPE_SMOKE,
		TYPE_GIZA,
		TYPE_GIZA2,
		TYPE_GIZABLACK,
		TYPE_GIZAWHITE,
		TYPE_GIZAWHITE2,
		TYPE_WAVEWHITE,
		TYPE_MAX
	}TYPE;

	CImpactWave(int nPriority = 6);
	~CImpactWave();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);

	// �ݒ�E�擾
	void SetLife(const int nLife);				// �����ݒ�
	int GetLife(void) const;					// �����擾
	void SetMaxLife(const int nLife);			// �ő�����ݒ�
	int GetMaxLife(void) const;					// �ő�����擾
	void SetColOrigin(const D3DXCOLOR col);		// ���̐F�ݒ�
	D3DXCOLOR GetColOrigin(void) const;			// ���̐F�擾
	void SetOutWidth(const float fOutWidth);	// �O���ݒ�
	float GetOutWidth(void) const;				// �O���擾
	void SetInWidth(const float fInWidth);		// �����ݒ�
	float GetInWidth(void) const;				// �����擾
	void SetRotWidth(const float fRot);			// �ꕪ��������̊p�x�ݒ�
	float GetRotWidth(void) const;				// �ꕪ��������̊p�x�擾
	void SetMoveSize(const float fMoveSize);	// �T�C�Y�̈ړ��ʐݒ�
	float GetMoveSize(void) const;				// �T�C�Y�̈ړ��ʎ擾
	void SetTexDivision(const int nDivision);	// �e�N�X�`���������ݒ�
	int GetTexDivision(void) const;				// �e�N�X�`���������擾
	void SetIsAddBlend(bool bAddBlend);			// ���Z�����̐ݒ�

	static const char *GetFileName(int nTexType);	// �t�@�C�����擾
	static CImpactWave *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fCenterDistance, int nLife, float fMove, int nTexType, bool bAddBlend);
	CImpactWave *GetMyObject(void);

private:
	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nLife;	// ����
	int m_nMaxLife;	// �ő����
	int m_nTexDivision;		// �e�N�X�`������
	D3DXCOLOR m_colOrigin;	// ���̐F
	float m_fOutWidth;		// ����(�O)
	float m_fInWidth;		// ��(��)
	float m_fRotWidth;		// 1������������̊p�x����
	float m_fHeight;		// ����
	float m_fMove;			// �L���鑬�x
	bool m_bAddBlend;		// ���Z�����̔���
	static const char *m_apFilename[TYPE_MAX];		//�t�@�C���ǂݍ���
};



#endif