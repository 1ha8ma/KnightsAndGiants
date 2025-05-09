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
	const float RotateXSpeed = 0.01f;				//X²ρ]¬x
	const float State0ChangeNextStateRotX = 0.8f;	//state0©ηstate1ΙΟX·ιΜX²ρ]Κ
	const float State1ChangeNextStateRotX = 0.0f;	//state1©ηstate2ΙΟX·ιΜX²ρ]Κ
};