#pragma once
#include"ArmEnemyMoveBase.h"

class SoundEffect;

class ArmEnemySwing :public ArmEnemyMoveBase
{
public:
	ArmEnemySwing(int modelHandle,VECTOR prevRotate);
	~ArmEnemySwing()override;

	bool Update(Camera* camera,VECTOR playerPosition)override;
	void Draw()override {};

private:
	const int JoyPadVibPower = 150;				//�R���g���[���[�U������
	const int JoyPadVibflame = -1;				//�R���g���[���[�U���t���[��(��~�w��������܂�)
	const float RotateZSpeed = 0.007f;			//Z��]���x
	const float State0ChangeNextStateRotZ = 0.6f;	//state0�̎���state�ɐi��Z����]��
	const float State1ChangeNextStateRotZ = -0.7f;	//state1�̎���state�ɐi��Z����]��
	const float State2ChangeNextStateRotZ = 0.0f;	//state2�̎���state�ɐi��Z����]��

	SoundEffect* se;

	bool moveEnd;
};