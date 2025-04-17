#pragma once
#include"StageBase.h"
#include"CollisionData.h"

class CollisionManager;
class StageObject;
class Wall;
class BGM;

class ArmEnemyStage :public StageBase
{
public:
	ArmEnemyStage();
	~ArmEnemyStage()override;

	void InitializeGameOver()override;
	void InitializeGameClear()override;
	void Update()override;
	void Draw()override;
private:
	//壁当たり判定
	const float StageLeft = -2450.0f;
	const float StageRight = 7150.0f;
	const float StageFront = 4530.0f;
	const float StageBack = -4770.0f;
	
	const VECTOR StagePosition = VGet(0, 0, 0);				//ステージポジション
	const float StageScale = 25.0f;							//ステージモデルスケール
	const float SkyDomeScale = 130.0f;						//スカイドームスケール
	const VECTOR SkyDomePosition = VGet(1000, 0, -1000);	//スカイドームポジション

	CollisionManager* collisionManager;
	CollisionData collisionData;
	StageObject* wood;
	StageObject* rock1;
	StageObject* rock2;
	StageObject* rock3;
	Wall* wall;
	BGM* bgm;

	int skyDomeHandle;	//スカイドームハンドル
};