//=============================================================================
// 
//  �I�u�W�F�N�g�w�b�_�[ [object.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

class CObject2D;
class CObject3D;
class CObject3DMesh;
class CObjectX;
class CObjectChara;
class CEffect3D;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�N���X��`
class CObject
{
public:

	// �񋓌^��`
	enum TYPE
	{
		TYPE_NONE = 0,		// ����
		TYPE_PLAYER,		// �v���C���[
		TYPE_ENEMY,			// �G
		TYPE_BOSS,			// �{�X
		TYPE_BULLET,		// �e
		TYPE_EXPLOSION,		// ����
		TYPE_BG,			// �w�i
		TYPE_EFFECT2D,		// 2D�G�t�F�N�g
		TYPE_EFFECT3D,		// 3D�G�t�F�N�g
		TYPE_SCORE,			// �X�R�A
		TYPE_TIMER,			// �^�C�}�[
		TYPE_BLOCK,			// �u���b�N
		TYPE_ITEM,			// �A�C�e��
		TYPE_FIELD,			// �t�B�[���h
		TYPE_HPGAUGE,		// HP�Q�[�W
		TYPE_SHADOW,		// �e
		TYPE_MAP,			// �}�b�v
		TYPE_XFILE,			// X�t�@�C��
		TYPE_STAGE,			// X�t�@�C��
		TYPE_MODEL,			// ���f��
		TYPE_EDIT,			// �G�f�B�b�g
		TYPE_MESHFIELD,		// ���b�V���t�B�[���h
		TYPE_ELEVATION,		// �N���t�B�[���h
		TYPE_ELEVATION_OUTSIDE,		// �N���t�B�[���h
		TYPE_WATERFIELD,	// ���̃t�B�[���h
		TYPE_MAGMA,		// �}�O�}�t�B�[���h
		TYPE_MESHWALL,		// ���b�V���E�H�[��
		TYPE_MESHCYLINDER,	// ���b�V���V�����_�[
		TYPE_MESHDOME,		// ���b�V���h�[��
		TYPE_MESHDONUTS,	// ���b�V���h�[�i�c
		TYPE_MESHSPHERE,	// ���b�V���X�t�B�A
		TYPE_FOG,			// �t�H�O
		TYPE_BALLAST,		// ���I
		TYPE_FADE,			// �t�F�[�h
		TYPE_NUMBER,		// ����
		TYPE_OBJECT2D,		// �I�u�W�F�N�g2D
		TYPE_OBJECT3D,		// �I�u�W�F�N�g3D
		TYPE_OBJECTMESH,		// �I�u�W�F�N�g3D
		TYPE_OBJECTX,		// �I�u�W�F�N�gX
		TYPE_OBJECTBILLBOARD,		// �I�u�W�F�N�g�r���{�[�h
		TYPE_MAX
	};

	CObject(int nPriority = mylib_const::DEFAULT_PRIORITY);
	virtual ~CObject();

	// �h���N���X�ŃI�[�o�[���C�h���ꂽ�֐�����ΕK�v(�������z�֐�)
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	virtual void SetVtx(void);
	virtual void SetPosition(const D3DXVECTOR3 pos);		// �ʒu�ݒ�
	virtual D3DXVECTOR3 GetPosition(void) const;			// �ʒu�擾
	virtual void SetOldPosition(const D3DXVECTOR3 posOld);	// �ߋ��̈ʒu�ݒ�
	virtual D3DXVECTOR3 GetOldPosition(void) const;			// �ߋ��̈ʒu�擾
	virtual void SetOriginPosition(const D3DXVECTOR3 pos);	// ���̈ʒu�ݒ�
	virtual D3DXVECTOR3 GetOriginPosition(void) const;		// ���̈ʒu�擾
	virtual void SetMove(const D3DXVECTOR3 move);			// �ړ��ʐݒ�
	virtual D3DXVECTOR3 GetMove(void) const;				// �ړ��ʎ擾
	virtual void SetRotation(const D3DXVECTOR3 rot);		// �����ݒ�
	virtual D3DXVECTOR3 GetRotation(void) const;			// �����擾
	virtual bool Hit(const int nValue);						// �Q���̃o�[�`�����q�b�g����
	virtual bool Hit(const int nValue, D3DXVECTOR3 move);

	int GetPriority(void) { return m_nPriority; }	// �D�揇�ʎ擾
	void SetEnableDisp(bool bDisp) { m_bDisp = bDisp; }		// �`��ݒ�

	int SetEffectParent(CEffect3D *pEffect3D);	// �G�t�F�N�g�ݒ�
	CEffect3D *GetEffectParent(int nIdx);	// �e�ݒ肵���G�t�F�N�g�擾
	int GetEffectParentNum(void);			// �e�ݒ肵���G�t�F�N�g�̐��擾
	void ReleaseEffect(int nIdx);			// �G�t�F�N�g�̉��
	void UninitEffect(void);				// �G�t�F�N�g�̉��

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static const int GetNumAll(void) { return m_nNumAll; }
	static const int GetNumPriorityAll(int nPriority) { return m_nNumPriorityAll[nPriority]; }

	static CObject *GetTop(int nPriority);	// �擪�̃I�u�W�F�N�g�擾
	CObject *GetNext(void);					// ���̃I�u�W�F�N�g�擾
	bool IsDeath(void);						// ���S�̔���
	void SetType(const TYPE type);
	TYPE GetType(void) const;
	void SetEnableHitstopMove(void) { m_bHitstopMove = true; }	// �q�b�g�X�g�b�v���ɓ����t���O�L��
	virtual CObject *GetObject(void);
	virtual CObject2D *GetObject2D(void);
	virtual CObject3D *GetObject3D(void);
	virtual CObject3DMesh *GetObject3DMesh(void);
	virtual CObjectX *GetObjectX(void);
	virtual CObjectChara *GetObjectChara(void);

protected:

	void Release(void);	// �J������
	void Death(void);	// ���S����

private:

	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_posOld;		// �O��̈ʒu
	D3DXVECTOR3 m_posOrigin;	// ���̈ʒu
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_move;			// �ړ���

	int m_nPriority;		// �D�揇��
	static int m_nNumAll;	// �I�u�W�F�N�g�̑���
	static int m_nNumPriorityAll[mylib_const::PRIORITY_NUM];
	TYPE m_type;			// ���
	static CObject *m_pTop[mylib_const::PRIORITY_NUM];	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_pCur[mylib_const::PRIORITY_NUM];	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pPrev;	// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^
	bool m_bDeath;		// ���S�t���O
	bool m_bDisp;		// �`��t���O
	CEffect3D *m_pEffect[mylib_const::MAX_OBJ];	// �G�t�F�N�g�̃|�C���^
	int m_nNumEffectParent;	// �G�t�F�N�g�̐e�ݒ肵����
	bool m_bHitstopMove;		// �q�b�g�X�g�b�v���ɓ������̃t���O

};

#endif