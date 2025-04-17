#pragma once
#include"ArmEnemyMoveBase.h"

class SoundEffect;
class ArmEnemyAttackRock;

class ArmEnemyDropRock :public ArmEnemyMoveBase
{
public:
	ArmEnemyDropRock(int modelHandle,VECTOR prevRotate);
	~ArmEnemyDropRock()override;

	bool Update(Camera* camera, VECTOR playerPosition)override;
	void Draw()override;

private:
	const float CameraShakingPower = 1.0f;			//�J�����h�ꋭ��
	const int CameraShakingChangeDirflame = 10;		//�J�����㉺�h��ύX�t���[��
	const int CameraShakinPlayflame = 50;			//�J�����h��Đ��t���[��
	const int JoyPadVibPower = 200;					//�R���g���[���[�U������
	const int JoyPadVibflame = 900;					//�R���g���[���[�U���t���[��
	const float InitRotateX = -0.5f;				//����X����]
	const float State0RotateXSpeed = 0.01f;			//state0X����]���x
	const float State0RotateZSpeed = 0.01f;			//state0Z����]���x
	const float State0changeNextStateRotX = 0.2f;	//state0����1�ɕύX����Ƃ���X����]��
	const float State1RotateXSpeed = 0.02f;			//state1X����]���x
	const float State1RotateZSpeed = 0.005f;		//state1Z����]���x
	const float State1ChangeNextStateRotX = -1.4f;	//state1����2�ɕύX���鎞��X����]��
	const float State2RotateXSpeed = 0.005f;		//state2X����]���x
	const float State2ChangeNextStateRotX = -0.2f;	//state2����3�ɕύX���鎞��X����]��

	SoundEffect* se;
	ArmEnemyAttackRock* rock;				//��

	bool moveEnd;							//�����I��
};