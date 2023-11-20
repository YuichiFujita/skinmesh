//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _PARTICLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PARTICLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// ���O���
//==========================================================================
// �萔��`
namespace my_particle
{
	// �񋓌^��`
	typedef enum
	{
		TYPE_NONE = 0,		// ���̃p�[�e�B�N��
		TYPE_SMOKE,			// ���̃p�[�e�B�N��
		TYPE_SMOKE_RED,		// ���̃p�[�e�B�N����
		TYPE_SMOKE_YEL,		// ���̃p�[�e�B�N����
		TYPE_EXPLOSION,		// �����̃p�[�e�B�N��
		TYPE_MANYTXT_MINI,	// ��ʔ�����UI(�~�j)
		TYPE_RESULTMESSAGE,	// ���U���g���b�Z�[�W
		TYPE_OFFSETTING,	// �e�ł�����
		TYPE_SUPERATTACK,	// ���U��
		TYPE_ICE_SPAWN,		// �X�o��
		TYPE_CHECK,			// �m�F�p
		TYPE_CHECK2D,			// �m�F�p
		TYPE_SICKLELINE,	// �劙�̃��C��
		TYPE_MAGIC_EXPLOSION,	// ���@�̔���
		TYPE_EXPLOSION_REMAIN,	// ���@�̔����̎c��
		TYPE_EXPLOSION_REMAIN_F,	// ���@�̔����̎c��,��
		TYPE_EXPLOSION_START,	// ���@�̔����J�n
		TYPE_WAVESMOKE,		// �g�̉�
		TYPE_IMPACTWAVESMOKE,		// �~�`�g�̉�
		TYPE_ENEMYSPAWN,	// �G�X�|�[��
		TYPE_NATURALWAVE_SPAWN,	// ���R�̔g
		TYPE_ENEMY_FADE,	// �G�̃t�F�[�h
		TYPE_WATERIN,	// ����������
		TYPE_MOVESMOKE,	// �ړ��̉�
		TYPE_ENEMYKILL_COMBO,	// �R���{���̓G�|����
		TYPE_ADDSCORE,			// �X�R�A���Z��
		TYPE_MAX
	}TYPE;

	void Create(D3DXVECTOR3 pos, my_particle::TYPE nType);
}

#endif