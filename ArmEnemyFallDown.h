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
	const float CameraShakingPower = 3.0;			//カメラ揺れ強さ
	const int CameraShakingChangeDirflame = 10;		//カメラ上下揺れ変更フレーム
	const int CameraShakinPlayflame = 50;			//カメラ揺れ再生フレーム
	const int JoyPadVibPower = 300;					//コントローラー振動強さ
	const int JoyPadVibflame = 1200;				//コントローラー振動フレーム
	const float State0RotateXSpeed = 0.0035f;		//state0X軸回転速度
	const float State0ChangeNextStateRotX = 0.7f;	//state0からstate1に変更する時のX軸回転量
	const float State1RotateXSpeed = 0.01f;			//state1X軸回転速度
	const float State1ChangeNextStateRotX = -1.3f;	//state1からstate1に変更する時のX軸回転量
};