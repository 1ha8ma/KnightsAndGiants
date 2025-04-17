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
	const int JoyPadVibPower = 150;				//コントローラー振動強さ
	const int JoyPadVibflame = -1;				//コントローラー振動フレーム(停止指示が来るまで)
	const float RotateZSpeed = 0.007f;			//Z回転速度
	const float State0ChangeNextStateRotZ = 0.6f;	//state0の次のstateに進むZ軸回転量
	const float State1ChangeNextStateRotZ = -0.7f;	//state1の次のstateに進むZ軸回転量
	const float State2ChangeNextStateRotZ = 0.0f;	//state2の次のstateに進むZ軸回転量

	SoundEffect* se;

	bool moveEnd;
};