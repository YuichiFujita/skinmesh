//=============================================================================
//
// �J�������� [camera.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERA_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "scene.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����N���X��`
class CCamera
{
private:

	// �񋓌^��`
	enum STATE
	{
		CAMERASTATE_NONE = 0,	// �ʏ�
		CAMERASTATE_SHAKE,		// �U��
	};

public:

	// �Ǐ]�̎��
	enum CHASETYPE
	{
		CHASETYPE_NORMAL = 0,	// �ʏ�
		CHASETYPE_MAP,			// �}�b�v�ɒǏ]
		CHASETYPE_NONE,			// �Ǐ]�Ȃ�
		CHASETYPE_MAX
	};

	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetRotation(const D3DXVECTOR3 rot);	// �����ݒ�
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	D3DXVECTOR3 GetPositionV(void) const;		// �J�����̈ʒu�擾
	D3DXVECTOR3 GetPositionR(void) const;		// �J�����̒����_�擾
	void SetOriginDistance(float fDistance);	// ���ɂȂ�J�����̋����ݒ�
	float GetOriginDistance(void);				// ���ɂȂ�J�����̋����擾
	void SetDestRotation(const D3DXVECTOR3 rot);	// �ڕW�̌����ݒ�
	D3DXVECTOR3 GetDestRotation(void);				// �ڕW�̌����擾
	void SetTargetPosition(const D3DXVECTOR3 pos);	// �Ǐ]�ڕW�̈ʒu�ݒ�
	D3DXVECTOR3 GetTargetPosition(void);			// �Ǐ]�ڕW�̈ʒu�擾
	void SetTargetRotation(const D3DXVECTOR3 rot);	// �Ǐ]�ڕW�̌����ݒ�
	D3DXVECTOR3 GetTargetRotation(void);			// �Ǐ]�ڕW�̌����擾

	void SetShake(int nTime, float fLength, float fLengthY);	// �U���ݒ�
	void SetLenDest(float fLength, int nCntTime = 120, float DecrementValue = 2.0f, float fCorrection = 0.1f);	// �ڕW�̒����ݒ�
	void SetPlayerChaseIndex(int nIdx);				// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��ݒ�
	int GetPlayerChaseIndex(void);					// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��擾
	void SetEnableFollow(bool bFollow);	// �Ǐ]�̔���ݒ�
	bool IsFollow(void);				// �Ǐ]��Ԏ擾
	void SetViewPort(D3DXVECTOR3 pos, D3DXVECTOR2 size);		// �r���[�|�[�g�̐ݒ�
	void Reset(CScene::MODE mode);	// ���Z�b�g
	void ResetBoss(void);	// ���Z�b�g
private:

	// �����o�ϐ�
	void UpdateByMode(void);	// ���[�h�ʍX�V����
	void MoveCameraInput(void);
	void MoveCameraStick(void);
	void MoveCameraMouse(void);
	void MoveCameraVR(void);
	void MoveCameraV(void);
	void MoveCameraR(void);
	void MoveCameraDistance(void);
	void SetCameraV(void);
	void SetCameraVTitle(void);
	void SetCameraVGame(void);
	void SetCameraVResult(void);
	void SetCameraR(void);
	void SetCameraRTitle(void);
	void SetCameraRGame(void);
	void SetCameraRResult(void);
	void MoveCameraFollow(void);
	void Shake(void);
	void UpdateState(void);
	void UpdateSpotLightVec(void);

	// ���Z�b�g
	void ResetGame(void);
	void ResetTitle(void);
	void ResetResult(void);
	void ResetRanking(void);

	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;		// �r���[�|�[�g
	D3DXVECTOR3 m_posV;				// ���_(�J�����̈ʒu)
	D3DXVECTOR3 m_posR;				// �����_(�������ꏊ)
	D3DXVECTOR3 m_posVDest;			// �ڕW�̎��_
	D3DXVECTOR3 m_posRDest;			// �ڕW�̒����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_move;				// �ړ���
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_rotVDest;			// �ڕW�̎��_�̌���
	D3DXVECTOR3 m_TargetPos;		// �Ǐ]�ڕW�̈ʒu
	D3DXVECTOR3 m_TargetRot;		// �Ǐ]�ڕW�̌���
	float m_fDistance;				// ����
	float m_fDestDistance;			// �ڕW�̋���
	float m_fOriginDistance;		// ���̋���
	float m_nShakeLength;			// �h��̒���
	float m_nShakeLengthY;			// Y�̗h��̒���
	float m_fMoveShake;				// �h��̈ړ���
	float m_fMoveShakeY;			// Y�̗h��̈ړ���
	float m_fHeightMaxDest;			// �J�����̍ő卂���̖ڕW
	float m_fHeightMax;				// �J�����̍ő卂��
	float m_fDiffHeight;			// �����̍���
	float m_fDiffHeightSave;		// �����̍����ۑ��p
	float m_fDiffHeightDest;		// �ڕW�̍����̍���
	float m_fChaseLerpStart;		// �Ǐ]�␳�̏����l
	float m_fChaseDistance;			// �Ǐ]�̊Ԋu
	float m_fDestChaseDistance;		// �ڕW�̒Ǐ]�̊Ԋu
	bool m_bFollow;					// �Ǐ]���邩�ǂ���
	int m_nCntState;				// ��ԃJ�E���^�[
	int m_nCntDistance;				// �����J�E���^�[
	int m_nOriginCntDistance;		// ���̋����J�E���^�[
	float m_fDistanceCorrection;	// �����̊����␳�W��
	float m_fDistanceDecrementValue;// �����̌����W��
	STATE m_state;					// ���
	CHASETYPE m_ChaseType;			// �Ǐ]�̎��
	CHASETYPE m_OldChaseType;		// �O��̒Ǐ]�̎��
	int m_nCntChaseType;			// �Ǐ]�̃J�E���^�[
	int m_nChasePlayerIndex;		// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ�
};

#endif