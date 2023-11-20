//=============================================================================
// 
//  �I�u�W�F�N�g���� [object.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object.h"
#include "manager.h"
#include "3D_effect.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CObject::m_nNumAll = 0;	// ����
int CObject::m_nNumPriorityAll[mylib_const::PRIORITY_NUM] = {};
CObject *CObject::m_pTop[mylib_const::PRIORITY_NUM]= {};	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObject *CObject::m_pCur[mylib_const::PRIORITY_NUM]= {};	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject::CObject(int nPriority)
{
	if (nPriority < 0 || nPriority >= mylib_const::PRIORITY_NUM)
	{// �͈͊O��������

		return;
	}

	// �l�̃N���A
	m_pos = mylib_const::DEFAULT_VECTOR3;		// �ʒu
	m_posOld = mylib_const::DEFAULT_VECTOR3;	// �O��̈ʒu
	m_rot = mylib_const::DEFAULT_VECTOR3;		// ����
	m_move = mylib_const::DEFAULT_VECTOR3;		// �ړ���

	m_pPrev = NULL;	// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	m_pNext = NULL;	// ���̃I�u�W�F�N�g�ւ̃|�C���^
	m_nPriority = nPriority;	// �D�揇��
	m_type = TYPE_NONE;			// ���
	m_bDeath = false;			// ���S�t���O
	m_bDisp = true;			// �`��t���O
	memset(&m_pEffect[0], NULL, sizeof(m_pEffect));	// �G�t�F�N�g�̃|�C���^
	m_nNumEffectParent = 0;		// �G�t�F�N�g�̐e�ݒ肵����
	m_bHitstopMove = false;		// �q�b�g�X�g�b�v���ɓ������̃t���O
	m_nNumAll++;				// �������Z
	m_nNumPriorityAll[nPriority]++;

	// �Ō����ۑ�
	CObject *pObjCur = m_pCur[nPriority];

	if (pObjCur != NULL)
	{// �Ō�������݂��Ă�����

		pObjCur->m_pNext = this;				// �Ō���̎��Ɏ������g����

		m_pCur[nPriority] = this;				// �Ō�����������g�ɍX�V
		m_pCur[nPriority]->m_pPrev = pObjCur;	// �Ō���̑O�ɉߋ��̍Ō������
	}
	else
	{// �Ō��������������

		m_pCur[nPriority] = this;	// �Ō�����������g�ɍX�V
	}

	if (m_pTop[nPriority] == NULL)
	{// �擪�ɉ����Ȃ�������

		m_pTop[nPriority] = this;	// �擪�Ɏ������g�̃|�C���^��n��
	}

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject::~CObject()
{

}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̔j��
//==========================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		// �擪��ۑ�
		CObject *pObject = m_pTop[nCntPriority];

		while (pObject != NULL)
		{// NULL������܂Ŗ������[�v

			// ���̃I�u�W�F�N�g���ꎞ�ۑ�
			CObject *pObjNext = pObject->m_pNext;

			if (pObject->m_type != TYPE_NONE)
			{// NONE����Ȃ����

				// �I������
				pObject->Uninit();
			}

			// ���̃I�u�W�F�N�g����
			pObject = pObjNext;
		}

		//*******************************
		// ���S����
		//*******************************
		// �擪��ۑ�
		pObject = m_pTop[nCntPriority];

		while (pObject != NULL)
		{// NULL������܂Ŗ������[�v

			// ���̃I�u�W�F�N�g���ꎞ�ۑ�
			CObject *pObjNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{// ���S�t���O�������Ă�����

				// ���S���S����
				pObject->Death();
			}
			else
			{
				int n = 0;
			}

			// ���̃I�u�W�F�N�g����
			pObject = pObjNext;
		}
	}
}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//==========================================================================
void CObject::UpdateAll(void)
{
#if _DEBUG
	// �G�f�B�b�g�̏��擾
	CEdit *pEdit = CManager::GetInstance()->GetEdit();
#endif

	// �q�b�g�X�g�b�v����
	bool bHitstop = CManager::GetInstance()->IsHitStop();

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		// �擪��ۑ�
		CObject *pObject = m_pTop[nCntPriority];

		while (pObject != NULL)
		{// NULL������܂Ŗ������[�v

			// ���̃I�u�W�F�N�g���ꎞ�ۑ�
			CObject *pObjNext = pObject->m_pNext;

#if _DEBUG
			if (pObject->m_bDeath == false && pEdit != NULL && (pObject->m_type == TYPE_EDIT || pObject->m_type == TYPE_XFILE || pObject->m_type == TYPE_ELEVATION) &&
				(pObject->m_bHitstopMove == true || (pObject->m_bHitstopMove == false && bHitstop == false)))
			{// �G�f�B�b�g��Ԃ�������G�f�B�b�g�̂ݍX�V

				// �X�V����
				pObject->Update();
			}
			else if (pObject->m_bDeath == false && pEdit == NULL && pObject->m_type != TYPE_NONE &&
				(pObject->m_bHitstopMove == true || (pObject->m_bHitstopMove == false && bHitstop == false)))
			{// �G�f�B�b�g��Ԃ���Ȃ� && �^�C�v��NONE�ȊO

				// �X�V����
				pObject->Update();
			}
#else
			if (pObject->m_bDeath == false && pObject->m_type != TYPE_NONE &&
				(pObject->m_bHitstopMove == true || (pObject->m_bHitstopMove == false && bHitstop == false)))
			{// �^�C�v��NONE�ȊO

				// �X�V����
				pObject->Update();
			}
#endif

			// ���̃I�u�W�F�N�g����
			pObject = pObjNext;
		}

		//*******************************
		// ���S����
		//*******************************
		// �擪��ۑ�
		pObject = m_pTop[nCntPriority];

		while (pObject != NULL)
		{// NULL������܂Ŗ������[�v

			// ���̃I�u�W�F�N�g���ꎞ�ۑ�
			CObject *pObjNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{// ���S�t���O�������Ă�����

				// ���S���S����
				pObject->Death();
			}

			// ���̃I�u�W�F�N�g����
			pObject = pObjNext;
		}
	}
}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̕`�揈��
//==========================================================================
void CObject::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		// �擪��ۑ�
		CObject *pObject = m_pTop[nCntPriority];

		while (pObject != NULL)
		{// NULL������܂Ŗ������[�v

			// ���̃I�u�W�F�N�g���ꎞ�ۑ�
			CObject *pObjNext = pObject->m_pNext;

			if (pObject->m_bDisp == true && pObject->m_bDeath == false && pObject->m_type != TYPE_NONE)
			{// NONE����Ȃ����

				// �`�揈��
				pObject->Draw();
			}

			// ���̃I�u�W�F�N�g����
			pObject = pObjNext;
		}
	}
}

//==========================================================================
// �G�t�F�N�g�ݒ�
//==========================================================================
int CObject::SetEffectParent(CEffect3D *pEffect3D)
{
	int nIdx = 0;
	for (int nCntEffect = 0; nCntEffect < mylib_const::MAX_OBJ; nCntEffect++)
	{
		if (m_pEffect[nCntEffect] != NULL)
		{// NULL����Ȃ�������
			continue;
		}

		// �G�t�F�N�g�̃|�C���^��n��
		m_pEffect[nCntEffect] = pEffect3D;
		nIdx = nCntEffect;

		// �G�t�F�N�g�̐e�ݒ肵�������Z
		m_nNumEffectParent++;
		break;
	}

	return nIdx;
}

//==========================================================================
// �e�ݒ肵���G�t�F�N�g�̐��擾
//==========================================================================
int CObject::GetEffectParentNum(void)
{
	return m_nNumEffectParent;
}

//==========================================================================
// �e�ݒ肵���G�t�F�N�g�擾
//==========================================================================
CEffect3D *CObject::GetEffectParent(int nIdx)
{
	return m_pEffect[nIdx];
}

//==========================================================================
// �G�t�F�N�g�̉��
//==========================================================================
void CObject::ReleaseEffect(int nIdx)
{
	// �G�t�F�N�gNULL�ɂ���
	m_pEffect[nIdx] = NULL;

	// �G�t�F�N�g�̐e�ݒ肵�������Z
	m_nNumEffectParent--;
}

//==========================================================================
// �G�t�F�N�g�̉��
//==========================================================================
void CObject::UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < mylib_const::MAX_OBJ; nCntEffect++)
	{
		if (m_pEffect[nCntEffect] == NULL)
		{// NULL��������
			continue;
		}
		m_pEffect[nCntEffect]->UninitParent();
		m_pEffect[nCntEffect] = NULL;
	}
}

//==========================================================================
// �I�u�W�F�N�g�̎��S����
//==========================================================================
void CObject::Release(void)
{
	// �G�t�F�N�g�̉��
	UninitEffect();

	// ���S�t���O�𗧂Ă�
	m_bDeath = true;
}

//==========================================================================
// �I�u�W�F�N�g�̔j���E���S����
//==========================================================================
void CObject::Death(void)
{
	// ����̔ԍ��ۑ�
	int nPriority = m_nPriority;

	// �I�u�W�F�N�g��ۑ�
	CObject *pObject = this;
	CObject *pObjNext = pObject->m_pNext;
	CObject *pObjPrev = pObject->m_pPrev;

	if (pObject == NULL)
	{// NULL��������
		return;
	}

	// �O�̃I�u�W�F�N�g�Ǝ��̃I�u�W�F�N�g���q��
	if (pObjNext == NULL)
	{// �������Ō���̎�

		if (pObjPrev != NULL)
		{// �O�̃I�u�W�F�N�g�������

			// �O�̃I�u�W�F�N�g�̎���NULL�ɂ���
			pObjPrev->m_pNext = NULL;

			// �Ō���������̑O�̃I�u�W�F�N�g�ɂ���
			m_pCur[nPriority] = pObjPrev;
		}
		else
		{// �O�̃I�u�W�F�N�g���Ȃ���

			// �Ō����NULL�ɂ���
			m_pCur[nPriority] = NULL;
		}
	}
	else
	{// �Ō������Ȃ��Ƃ�

		if (pObjPrev != NULL)
		{// �������擪����Ȃ���

			// �O�̃I�u�W�F�N�g�̎���, �����̎��ɂ���
			pObjPrev->m_pNext = pObject->m_pNext;
		}

	}

	if (pObjPrev == NULL)
	{// �������擪�̎�

		if (pObjNext != NULL)
		{// ���̃I�u�W�F�N�g�����鎞

			// ���̃I�u�W�F�N�g�̑O��NULL�ɂ���
			pObjNext->m_pPrev = NULL;

			// �擪�������̎��̃I�u�W�F�N�g�ɂ���
			m_pTop[nPriority] = pObjNext;
		}
		else
		{// ���̃I�u�W�F�N�g���Ȃ���

			// �擪��NULL�ɂ���
			m_pTop[nPriority] = NULL;
		}
	}
	else
	{// �擪����Ȃ��Ƃ�

		if (pObjNext != NULL)
		{// �������Ō������Ȃ���

			// ���̃I�u�W�F�N�g�̑O��, �����̑O�ɂ���
			pObjNext->m_pPrev = pObject->m_pPrev;
		}
	}

	//if (m_pReleaseNext == pObject)
	//{// �{�����ɏ����I�u�W�F�N�g�ƍ�������I�u�W�F�N�g�������Ƃ�
	//	m_pReleaseNext = pObjNext;
	//}

	// �������̊J��
	delete pObject;
	pObject = NULL;

	// ���������炷
	m_nNumAll--;
	m_nNumPriorityAll[nPriority]--;
}


//==========================================================================
// �Q���̃o�[�`�����q�b�g����
//==========================================================================
bool CObject::Hit(const int nValue)
{
	// ����łȂ�
	return false;
}

//==========================================================================
// �Q���̃o�[�`�����q�b�g����
//==========================================================================
bool CObject::Hit(const int nValue, D3DXVECTOR3 move)
{
	// ����łȂ�
	return false;
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject::SetVtx(void)
{

}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObject::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObject::GetPosition(void) const
{
	return m_pos;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObject::SetOldPosition(const D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObject::GetOldPosition(void) const
{
	return m_posOld;
}

//==========================================================================
// ���̈ʒu�ݒ�
//==========================================================================
void CObject::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// ���̈ʒu�擾
//==========================================================================
D3DXVECTOR3 CObject::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// �ړ��ʐݒ�
//==========================================================================
void CObject::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//==========================================================================
// �ړ��ʎ擾
//==========================================================================
D3DXVECTOR3 CObject::GetMove(void) const
{
	return m_move;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CObject::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// �����擾
//==========================================================================
D3DXVECTOR3 CObject::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// ��ނ̐ݒ�
//==========================================================================
void CObject::SetType(const TYPE type)
{
	m_type = type;
}

//==========================================================================
// ��ނ̎擾
//==========================================================================
CObject::TYPE CObject::GetType(void) const
{
	return m_type;
}

//==========================================================================
// �I�u�W�F�N�g�̎擾
//==========================================================================
CObject *CObject::GetObject(void)
{
	return this;
}

//==========================================================================
// �擪�̃I�u�W�F�N�g�擾
//==========================================================================
CObject *CObject::GetTop(int nPriority)
{
	return m_pTop[nPriority];
}

//==========================================================================
// ���̃I�u�W�F�N�g�擾
//==========================================================================
CObject *CObject::GetNext(void)
{
	return this->m_pNext;
}

//==========================================================================
// ���S�̔���
//==========================================================================
bool CObject::IsDeath(void)
{
	return m_bDeath;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CObject::GetObject2D(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3D *CObject::GetObject3D(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�g3D���b�V���I�u�W�F�N�g�̎擾
//==========================================================================
CObject3DMesh *CObject::GetObject3DMesh(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�gX�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectX *CObject::GetObjectX(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectChara *CObject::GetObjectChara(void)
{
	return NULL;
}