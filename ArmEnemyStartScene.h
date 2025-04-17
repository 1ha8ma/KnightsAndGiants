#pragma once
#include"SceneBase.h"

class Camera;
class Player;
class ArmEnemy;
class ArmEnemyStage;

class ArmEnemyStartScene:public SceneBase
{
public:
	ArmEnemyStartScene();
	~ArmEnemyStartScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	const int MaxBlackBandAlpha = 255;										//黒帯最大濃さ
	const int AlphaIncrease = 3;											//濃くする速さ
	const float CameraMaxLookPosY = 6000.0f;								//最大LookPosition高さ
	const float CameraLookPosUpYSpeed = 20.0f;								//LookPositionの高さを上げる速度
	const float CameraMoveZSpeed = 4.0f;
	const VECTOR InitCameraPosition = VGet(5000.0f, 2500.0f, -1500.0f);		//カメラ初期位置
	const VECTOR InitCameraLookPosition = VGet(500.0f, 1000.0f, 1000.0f);	//カメラ初期注視点
	const int ChangeSceneflame = 400;										//スタートシーンから変更するフレーム

	//他クラス
	Camera* camera;
	Player* player;
	ArmEnemy* enemy;
	ArmEnemyStage* stage;

	int flame;							//開始からのフレーム数
	int blackBandAlpha;					//黒帯の透明度
	VECTOR cameraPosition;				//カメラポジション
	VECTOR cameraLookPosition;			//カメラ注視点
};