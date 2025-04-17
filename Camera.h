#pragma once

class Input;
class Calculation;

class Camera
{
public:
	Camera();

	//スタートシーン初期化
	void StartSceneInitialize(VECTOR position,VECTOR lookPosition);
	//スタートシーン更新
	void UpdateStartScene(VECTOR position, VECTOR lookPosition);

	//ゲームシーン初期化
	void GameInitialize(VECTOR playerpos);
	//更新
	void UpdateGame(VECTOR playerPosition,VECTOR targetCameraPosition,float addPlayerDistance);

	//ゲームオーバーシーン初期化
	void InitializeGameOver(VECTOR playerPosition);
	//ゲームオーバーシーン更新
	void UpdateGameOver();

	//ゲームクリアシーン初期化
	void InitializeGameClear(VECTOR cameraPosition, VECTOR enemyPosition);
	//ゲームクリアシーン更新
	void UpdateGameClear(VECTOR enemyPosition);
	
	//演出
	void PlayShakingVertical(float shakingPower, int shakingDirectionChangeflame, int flame);

	//GetSet
	float GetangleH() { return angleH; }
	float GetangleV() { return angleV; }
	VECTOR GetLookPosition() { return lookPosition; }
	VECTOR GetPosition() { return position; }

private:
	const float CameraUpLimit = -1.2f;	//上を向く上限 -0.7
	const float CameraDownLimit = 1.0f;	//下を向く上限 1.0

	//演出更新
	void UpdateProduction(VECTOR playerPosition);
	//上下揺れ
	void ShakingVertical(VECTOR playerPosition);

	//他クラス
	Input* input;
	Calculation* calculation;

	float Near;							//描画可能になる近い距離
	float Far;							//描画可能な遠さ
	float AngleSpeed;					//旋回角度
	float CameraPlayerTargetHeight;		//プレイヤーのどれだけ高いところを見るか
	float PlayerDistance;				//プレイヤーとの距離
	VECTOR position;					//ポジション
	VECTOR lookPosition;				//注視点
	float angleH;						//平面角度
	float angleV;						//垂直角度

	//演出
	bool productionflg;					//演出中フラグ
	int totalflame;						//演出フレーム
	float t;							//Lerp用
	bool lerpflg;						//Lerp開始フラグ
	float LerpSpeed;					//Lerp速度

	//縦揺れ
	int changeflame;					//切り替えまでのフレーム
	bool shakingDirection;				//揺れる方向フラグ
	bool shakingVerticalflg;			//縦揺れフラグ
	float shakingPower;					//揺れの強さ
	int shakingDirectionChangeflame;	//揺れる方向を変更するフレーム
	int playflame;						//再生時間

	//ゲームオーバー
	float LeaveSpeed;					//プレイヤーから離れる速度
};