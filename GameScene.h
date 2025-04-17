#pragma once
#include"SceneBase.h"
#include"Time.h"

class Camera;
class CollisionManager;
class EnemyBase;
class StageBase;
class Player;
class GameUI;

class GameScene :public SceneBase
{
public:
	GameScene(StageBase* stage, EnemyBase* enemy,Camera* camera,Player* player,Time::StageTag tag);
	~GameScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	Camera* camera;
	CollisionManager* collisionManager;
	StageBase* stage;
	Player* player;
	EnemyBase* enemy;
	GameUI* ui;
	Time* time;

	bool gameOver;
	bool gameClear;
};