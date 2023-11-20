//=============================================================================
// 
//  �v���C���[�̗̑̓Q�[�W�w�b�_�[ [hp_gauge_player.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _HP_GAUGE_PLAYER_H_
#define _HP_GAUGE_PLAYER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�̗̑̓Q�[�W�N���X��`
class CHP_GaugePlayer : public CObject
{
public:

	CHP_GaugePlayer(int nPriority = 7);
	~CHP_GaugePlayer();

	static CHP_GaugePlayer *Create(D3DXVECTOR3 pos, int nMaxLife);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);
	void SetVtx(int nCntGauge);

	void Kill(void);
	void SetLife(int nLife);

private:

	// �񋓌^��`
	typedef enum
	{
		VTXTYPE_BLACK = 0,	// ���Q�[�W
		VTXTYPE_PINK,		// �s���N�Q�[�W
		VTXTYPE_FRAM,		// �g
		VTXTYPE_MAX
	}VTXTYPE;

	// �\���̒�`
	struct SHP_Gauge
	{
		CObject2D *pObj2D;	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
		float fMaxWidth;			// ���̍ő�l
		float fWidthDest;			// ���̍���
		float fMaxHeight;			// �����̍ő�l
	};

	// �����o�֐�
	void ChangeColor(int nCntGauge);		// �F�ύX
	void GaugeDecrement(int nCntGauge);		// ��������

	// �����o�ϐ�
	static const char *m_apTextureFile[];	// �e�N�X�`���̃t�@�C��
	SHP_Gauge m_HPGauge[VTXTYPE_MAX];		// HP�Q�[�W�̏��
	int m_nCntTkTk;							// �`�J�`�J�̃J�E���g
	int m_nLife;							// �̗�
	int m_nMaxLife;							// �ő�̗�
	int m_nTexIdx[VTXTYPE_MAX];				// �e�N�X�`���̃C���f�b�N�X�ԍ�
};


#endif