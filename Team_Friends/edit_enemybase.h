//=============================================================================
// 
//  �G�̋��_�G�f�B�^�[�w�b�_�[ [edit_enemybase.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_ENEMYBASE_H_
#define _EDIT_ENEMYBASE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "camera.h"

class CEnemy;

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����Ǐ]��ޕύX�G�f�B�b�g�N���X��`
class CEditEnemyBase
{
public:

	CEditEnemyBase();
	~CEditEnemyBase();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(const char *pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Release(void);
	static CEditEnemyBase *Create(void);	// ��������

private:

	void Control(D3DXVECTOR3 &pos);	// ����
	void Grab(void);	// �͂�
	void Delete(void);	// �폜
	void ChangeType(void);

	int m_nIdxMapPoint;			// �}�b�v�|�C���g�̃C���f�b�N�X�ԍ�
	float m_fPointRatio;		// �ړ�����
	float m_fMoveValue;			// �ړ���
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_rot;			// �ʒu
	int m_nEnemyType;			// �G�̎��
	int m_nStage;				// �X�e�[�W
	int m_nRush;				// ���b�V���p���ǂ���
	
	CEnemy *m_pEnemy[mylib_const::MAX_PATTEN_ENEMY];	// �G�ւ̃|�C���^
	CObjectX *m_apObjX;	// �I�u�W�F�N�gX
};



#endif