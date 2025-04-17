#pragma once
#include"PlayerStateProcessBase.h"

class PlayerFallDown :public PlayerStateProcessBase
{
public:
	PlayerFallDown(int modelHandle);
	~PlayerFallDown()override;

	void Initialize()override {};
	void UpdateGameOver()override;

private:
	const float PlayAnimationSpeed = 0.2f;	//アニメーション再生速度

	bool animationEnd;
};