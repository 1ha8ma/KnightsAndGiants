#pragma once
#include"ArmEnemyMoveBase.h"

class SoundEffect;
class ArmEnemyAttackRock;

class ArmEnemyDropRock :public ArmEnemyMoveBase
{
public:
	ArmEnemyDropRock(int modelHandle,VECTOR prevRotate);
	~ArmEnemyDropRock()override;

	bool Update(Camera* camera, VECTOR playerPosition)override;
	void Draw()override;

private:
	const float CameraShakingPower = 1.0f;			//カメラ揺れ強さ
	const int CameraShakingChangeDirflame = 10;		//カメラ上下揺れ変更フレーム
	const int CameraShakinPlayflame = 50;			//カメラ揺れ再生フレーム
	const int JoyPadVibPower = 200;					//コントローラー振動強さ
	const int JoyPadVibflame = 900;					//コントローラー振動フレーム
	const float InitRotateX = -0.5f;				//初期X軸回転
	const float State0RotateXSpeed = 0.01f;			//state0X軸回転速度
	const float State0RotateZSpeed = 0.01f;			//state0Z軸回転速度
	const float State0changeNextStateRotX = 0.2f;	//state0から1に変更するときのX軸回転量
	const float State1RotateXSpeed = 0.02f;			//state1X軸回転速度
	const float State1RotateZSpeed = 0.005f;		//state1Z軸回転速度
	const float State1ChangeNextStateRotX = -1.4f;	//state1から2に変更する時のX軸回転量
	const float State2RotateXSpeed = 0.005f;		//state2X軸回転速度
	const float State2ChangeNextStateRotX = -0.2f;	//state2から3に変更する時のX軸回転量

	SoundEffect* se;
	ArmEnemyAttackRock* rock;				//岩

	bool moveEnd;							//動き終了
};