#pragma once
#include"ArmEnemyMoveBase.h"

class ArmEnemyFallDown :public ArmEnemyMoveBase
{
public:
	ArmEnemyFallDown(int modelHandle);
	~ArmEnemyFallDown()override;

	bool UpdateFallDown(Camera* camera)override;
	void Draw()override;

private:
	const float CameraShakingPower = 3.0;			//�J�����h�ꋭ��
	const int CameraShakingChangeDirflame = 10;		//�J�����㉺�h��ύX�t���[��
	const int CameraShakinPlayflame = 50;			//�J�����h��Đ��t���[��
	const int JoyPadVibPower = 300;					//�R���g���[���[�U������
	const int JoyPadVibflame = 1200;				//�R���g���[���[�U���t���[��
	const float State0RotateXSpeed = 0.0035f;		//state0X����]���x
	const float State0ChangeNextStateRotX = 0.7f;	//state0����state1�ɕύX���鎞��X����]��
	const float State1RotateXSpeed = 0.01f;			//state1X����]���x
	const float State1ChangeNextStateRotX = -1.3f;	//state1����state1�ɕύX���鎞��X����]��
};