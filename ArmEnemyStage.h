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
	
	//木
	const VECTOR WoodPosition = VGet(3700.0f, 1000.0f, 3500.0f);		//ポジション
	const VECTOR WoodAddCapsuleStart = VGet(-2000.0f, 1000.0f, 0.0f);	//カプセル始点に伸ばす分
	const VECTOR WoodAddCapsuleEnd = VGet(1000.0f, -500.0f, 0.0f);		//カプセル終点に伸ばす分
	const float WoodCapsuleRadius = 800.0f;;							//カプセル半径
	const VECTOR WoodRotate = VGet(-DX_PI_F / 2, 0.0f, -0.7f);			//回転
	const float WoodScale = 40.0f;										//描画スケール

	//岩1
	const VECTOR Rock1Position = VGet(500.0f, 2300.0f, 4400.0f);		//ポジション
	const VECTOR Rock1AddCapsuleStart = VGet(-500.0f, 0.0f, 50.0f);		//カプセル始点に伸ばす分
	const VECTOR Rock1AddCapsuleEnd = VGet(400.0f, 0.0f, 0.0f);			//カプセル終点に伸ばす分
	const float Rock1CapsuleRadius = 1300.0f;							//カプセル半径
	const VECTOR Rock1Rotate = VGet(0.0f, 0.0f, DX_PI_F / 2);			//回転
	const float Rock1Scale = 5.0f;										//描画スケール

	//岩2
	const VECTOR Rock2Position = VGet(3500.0f, 3000.0f, 4000.0f);		//ポジション
	const VECTOR Rock2AddCapsuleStart = VGet(200.0f, 400.0f, 0.0f);		//カプセル始点に伸ばす分
	const VECTOR Rock2AddCapsuleEnd = VGet(-600.0f, 400.0f, 0.0f);		//カプセル終点に伸ばす分
	const float Rock2CapsuleRadius = 1000.0f;							//カプセル半径
	const VECTOR Rock2Rotate = VGet(0.0f, 0.0f, 0.0f);					//回転
	const float Rock2Scale = 10.0f;										//描画スケール

	//岩3
	const VECTOR Rock3Position = VGet(2300.0f, 5800.0f, 5000.0f);			//ポジション
	const VECTOR Rock3AddCapsuleStart = VGet(-800.0f, 1000.0f, -500.0f);	//カプセル始点に伸ばす分
	const VECTOR Rock3AddCapsuleEnd = VGet(800.0f, -1000.0f, -500.0f);		//カプセル終点に伸ばす分
	const float Rock3CapsuleRadius = 1400.0f;								//カプセル半径
	const VECTOR Rock3Rotate = VGet(0.0f, 0.0f, DX_PI_F / 4);				//回転
	const float Rock3Scale = 6.0f;											//描画スケール


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