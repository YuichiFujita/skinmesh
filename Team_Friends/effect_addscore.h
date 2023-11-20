//=============================================================================
// 
//  �e�w�b�_�[ [effect_addscore.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EFFECT_ADDSCORE_H_
#define _EFFECT_ADDSCORE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"


#define ADDSCORE_CIRCLE	(20)

//==========================================================================
// �N���X��`
//==========================================================================
// �e�N���X��`
class CEffectAddScore : public CObject
{
public:

	CEffectAddScore(int nPriority = 7);
	~CEffectAddScore();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CEffectAddScore *Create(const D3DXVECTOR3 pos);

private:
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_DMG,		// ���˕Ԃ�
		STATE_MAX
	};

	void UpdatePos(void);		// �ړ�

	STATE m_state;				// ���
	int m_nCntState;			// ��ԑJ�ڃJ�E���^�[
	int m_nLife;				// ����
	int m_nLifeMax;				// �����̍ő�l
	int m_nTexIdx;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	CObject2D *m_pEffect[ADDSCORE_CIRCLE];	// �����ڂ����̒e
};


#endif