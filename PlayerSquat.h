#pragma once
#include"PlayerStateProcessBase.h"

class PlayerSquat :public PlayerStateProcessBase 
{
public:
	PlayerSquat(int modelHandle,VECTOR prevtargetLookDirection);
	~PlayerSquat()override;

	void Initialize()override {};
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision) override;

private:
	const float PlayAnimationSpeed = 1.0f;	//�A�j���[�V�����Đ����x

	bool changeState;		//��ԕύX
};