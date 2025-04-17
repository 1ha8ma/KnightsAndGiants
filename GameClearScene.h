#pragma once
#include"SceneBase.h"

class Input;
class BGM;
class StageBase;
class EnemyBase;
class Camera;
class Player;

class GameClearScene :public SceneBase
{
public:
	GameClearScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player, bool rankIn,int time);
	~GameClearScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	const int MaxTextBackAlpha = 200;						//テキスト背景の最大濃さ
	const int MaxBlackBandAlpha = 255;						//黒帯の最大濃さ
	const int AlphaIncrease = 3;							//濃くするスピード
	const VECTOR InitCameraPos = VGet(2000, 1000, -2000);	//初期カメラポジション
		
	//他クラス
	Input* input;
	BGM* bgm;
	StageBase* stage;
	EnemyBase* enemy;
	Camera* camera;
	Player* player;

	bool canInputB;			//B連続入力防止
	int blackBandAlpha;		//黒帯の透明度
	bool enemyMoveEnd;		//敵の動き終了
	bool rankIn;			//ランクイン
	int clearTextBackImage;	//テキスト背景
	int textBackAlpha;		//テキスト背景透明度
	int time;				//その回のタイム
};