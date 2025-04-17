#pragma once
#include"CollisionData.h"
#include"Camera.h"

class PlayerStateProcessBase
{
public:
	PlayerStateProcessBase(int modelHandle);
	virtual ~PlayerStateProcessBase() {};

	/// <summary>
	/// �����Ŏg�p����v���C���[�̃f�[�^
	/// </summary>
	struct UsePlayerData
	{
		int inputState;						//�{�^�����͏��
		DINPUT_JOYSTATE stickState;			//�X�e�B�b�N���͏��
		VECTOR position;					//�|�W�V����
		VECTOR capsuleStart;				//�J�v�Z���n�_
		VECTOR capsuleEnd;					//�J�v�Z���I�_
		VECTOR centerPosition;				//�J�v�Z���̒��_
		float capsuleRadius;				//�J�v�Z�����a
		float wholeBodyCapsuleHalfLength;	//�J�v�Z���̒����̔���
		float angle;						//�����p�x
		bool onFoot;						//�v���C���[�̑��������Ă��邩
		VECTOR lookDirection;				//�����Ă������
	};

	//�t���[���ԍ�
	enum PlayerFrameNumber
	{
		Hips = 1,					//���ӂ�
		RightArmFrameNumber = 45,	//�E�r�̃t���[���ԍ�
		RightHandFrameNumber = 50,	//�E��̃t���[���ԍ�(���w)
	};

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() abstract;
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="inputstate">�{�^�����͏��</param>
	/// <param name="stickstate">�X�e�B�b�N���͏��</param>
	/// <param name="camera">�J����</param>
	/// <returns>�X�e�[�g����̃X�e�[�g�ύX�w��</returns>
	//virtual bool Update(VECTOR position, float angle, int inputstate, DINPUT_JOYSTATE stickstate, const Camera& camera, CollisionData objectCollision) { return false; }

	virtual bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision) { return false; }

	/// <summary>
	/// �Q�[���I�[�o�[�V�[���X�V
	/// </summary>
	virtual void UpdateGameOver() {};

	/// <summary>
	/// �`��(�m�F�p)
	/// </summary>
	virtual void Draw() {};


	//Get
	VECTOR GetmoveVec() { return moveVec; }
	VECTOR GetLookDirection() { return newLookDirection; }
	MATRIX GetRotateMatrix() { return rotateMatrix; }

	//�˂��h���p
	float GetCameraZoom() { return cameraZoom; }
	float GetArmRotateZ() { return armRotateZ; }

protected:
	//�A�j���[�V�����ԍ�
	enum PlayerAnimationNumber
	{
		Run,			//����
		Jump,			//�W�����v
		NormalAttack,	//�ʏ�U��
		Climb,			//�o��
		Squat,			//���Ⴊ��
		FallDown,		//�|���
		Idle,			//�ҋ@
		Falling,		//����
	};
	
	//�A�j���[�V�����Đ�
	void PlayAnimation(float playSpeed,bool stop);
	//�A�j���[�V�����Đ�(1��̂�)
	bool PlayAnimationOnce(float playSpeed);
	//�A�j���[�V�����t�Đ�
	bool ReversePlaybackAnimation(float playSpeed, float backPoint);
	//�A�j���[�V�����̏I��
	void DetachAnimation(int attachIndex);

	//���f���E�A�j���[�V����
	int modelHandle;			//���f���n���h��
	int nowPlayAnim;			//�Đ����̃A�j���[�V����
	float animTotalTime;		//�A�j���[�V�����̑��Đ�����
	float nowAnimPlayTime;		//���݂̃A�j���[�V�����Đ�����

	//�ړ�
	VECTOR moveVec;				//�ړ��x�N�g��
	VECTOR newLookDirection;	//������̃v���C���[�̌����ׂ�����
	MATRIX rotateMatrix;		//���f����]�s��

	//�˂��h���U���p
	float cameraZoom;
	float armRotateZ;
};