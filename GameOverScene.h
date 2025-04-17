#pragma once
#include"SceneBase.h"

class Input;
class BGM;
class StageBase;
class EnemyBase;
class Camera;
class Player;

class GameOverScene :public SceneBase
{
public:
	GameOverScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player);
	~GameOverScene()override;

	//シーンごとの初期化
	void Initialize() override;
	//シーンごとの更新
	SceneBase* Update() override;
	//シーンごとの描画
	void Draw() override;

private:
	const int AlphaIncrease = 2;					//濃くする速さ
	const int DrawGameOverStringflame = 200;		//ゲームオーバーの文字を表示し始めるフレーム

	//他クラス
	Input* input;
	BGM* bgm;
	StageBase* stage;
	EnemyBase* enemy;
	Camera* camera;
	Player* player;

	int flame;			//ゲームオーバーに入ってからの時間
	bool drawGameOver;	//ゲームオーバー表示フラグ
	int backImage;		//背景画像
	bool canInputB;		//Bボタン入力可能
	int backImageAlpha;	//背景透過度
};