#pragma once
#include"ArmEnemyMoveBase.h"

class ArmEnemyHandUp :public ArmEnemyMoveBase
{
public:
	ArmEnemyHandUp(int modelHandle,VECTOR prevRotate);
	~ArmEnemyHandUp()override;

	bool Update(Camera* camera,VECTOR playerPosition)override;
	void Draw()override { ; }

private:
	const float RotateXSpeed = 0.01f;				//X����]���x
	const float State0ChangeNextStateRotX = 0.8f;	//state0����state1�ɕύX���鎞��X����]��
	const float State1ChangeNextStateRotX = 0.0f;	//state1����state2�ɕύX���鎞��X����]��
};