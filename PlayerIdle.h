#pragma once
#include"PlayerStateProcessBase.h"

class PlayerIdle :public PlayerStateProcessBase
{
public:
	PlayerIdle(int modelHandle, VECTOR prevtargetLookDirection);
	~PlayerIdle()override;

	void Initialize()override;

	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)override;
private:
	const float PlayAnimationSpeed = 0.2f;	//アニメーション再生速度

};