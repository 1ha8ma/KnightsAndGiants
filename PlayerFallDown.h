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
	const float PlayAnimationSpeed = 0.2f;	//�A�j���[�V�����Đ����x

	bool animationEnd;
};