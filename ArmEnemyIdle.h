#pragma once
#include"ArmEnemyMoveBase.h"

class ArmEnemyIdle :public ArmEnemyMoveBase
{
public:
	ArmEnemyIdle(int modelHandle,VECTOR prevRotate);
	~ArmEnemyIdle()override;

	bool Update(Camera* camera,VECTOR playerPosition)override;
	void Draw()override {};

private:
	const VECTOR BasicRotate = VGet(-0.4, 0.0f, 0.0f);	//��{�̉�]��
	const float State0RotateXSpeed = 0.005f;			//state0X����]���x
	const float State1RotateXSpeed = 0.001f;			//state1X����]���x

	//�㉺�h��̐؂�ւ��p
	bool shakingflg;
	int shakingflame;
};