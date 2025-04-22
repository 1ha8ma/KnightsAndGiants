#include"DxLib.h"
#include"Loader.h"
#include"CollisionManager.h"
#include"BGM.h"
#include"StageObject.h"
#include"Wall.h"
#include"ArmEnemyStage.h"

/// <summary>
/// コンストラクタ
/// </summary>
ArmEnemyStage::ArmEnemyStage()
{
	//インスタンス化
	Loader* loader = loader->GetInstance();
	wall = new Wall();
	wood = new StageObject(loader->GetHandle(Loader::Kind::WoodModel), WoodAddCapsuleStart, WoodAddCapsuleEnd, WoodCapsuleRadius, WoodPosition, WoodRotate, WoodScale);
	rock1 = new StageObject(loader->GetHandle(Loader::Kind::RockModel1), Rock1AddCapsuleStart, Rock1AddCapsuleEnd, Rock1CapsuleRadius, Rock1Position, Rock1Rotate, Rock1Scale);
	rock2 = new StageObject(loader->GetHandle(Loader::Kind::RockModel2), Rock2AddCapsuleStart, Rock2AddCapsuleEnd, Rock2CapsuleRadius, Rock2Position, Rock2Rotate, Rock2Scale);
	rock3 = new StageObject(loader->GetHandle(Loader::Kind::RockModel3), Rock3AddCapsuleStart, Rock3AddCapsuleEnd, Rock3CapsuleRadius, Rock3Position, Rock3Rotate, Rock3Scale);
	bgm = new BGM(BGM::BGMKind::ArmEnemyStage);

	//変数初期化
	modelHandle = loader->GetHandle(Loader::Kind::ArmEnemyStage);
	skyDomeHandle = loader->GetHandle(Loader::Kind::SkyDome);
	stagePosition = StagePosition;

	//ステージ設定
	MV1SetScale(modelHandle, VGet(StageScale, StageScale, StageScale));
	MV1SetPosition(modelHandle, stagePosition);

	//スカイドーム設定
	MV1SetScale(skyDomeHandle, VGet(SkyDomeScale, SkyDomeScale, SkyDomeScale));
	MV1SetPosition(skyDomeHandle, SkyDomePosition);

	//当たり判定
	collisionManager = collisionManager->GetInstance();
	collisionData.tag = ObjectTag::Stage;
	collisionData.stageLeft = StageLeft;
	collisionData.stageRight = StageRight;
	collisionData.stageFront = StageFront;
	collisionData.stageBack = StageBack;

	collisionManager->AddCollisionData(&collisionData);
}

/// <summary>
/// デストラクタ
/// </summary>
ArmEnemyStage::~ArmEnemyStage()
{
	bgm->StopBGM();
	collisionManager->RemoveCollisionData(&collisionData);
	delete wood;
	delete wall;
	delete rock1;
	delete rock2;
	delete rock3;
	delete bgm;
}

/// <summary>
/// ゲームオーバーシーン初期化
/// </summary>
void ArmEnemyStage::InitializeGameOver()
{
	bgm->StopBGM();
}

/// <summary>
/// ゲームクリア初期化
/// </summary>
void ArmEnemyStage::InitializeGameClear()
{
	bgm->StopBGM();
}

/// <summary>
/// 更新
/// </summary>
void ArmEnemyStage::Update()
{
	bgm->PlayBGM(BGM::BGMKind::ArmEnemyStage);
}

/// <summary>
/// 描画
/// </summary>
void ArmEnemyStage::Draw()
{
	MV1DrawModel(skyDomeHandle);
	MV1DrawModel(modelHandle);
	wall->Draw();
	wood->Draw();
	rock1->Draw();
	rock2->Draw();
	rock3->Draw();
}