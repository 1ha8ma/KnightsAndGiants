#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Utility.h"
#include"Camera.h"
#include"ArmEnemy.h"
#include"ArmEnemyStage.h"
#include"Player.h"
#include"GameScene.h"
#include"Time.h"
#include"ArmEnemyStartScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
ArmEnemyStartScene::ArmEnemyStartScene()
{
	//インスタンス化
	camera = new Camera();
	player = new Player();
	stage = new ArmEnemyStage();
	enemy = new ArmEnemy();

	//変数初期化
	flame = 0;
	blackBandAlpha = 0;
	cameraPosition = InitCameraPosition;
	cameraLookPosition = InitCameraLookPosition;
}

/// <summary>
/// デストラクタ
/// </summary>
ArmEnemyStartScene::~ArmEnemyStartScene()
{
	
}

/// <summary>
/// 初期化
/// </summary>
void ArmEnemyStartScene::Initialize()
{
	camera->StartSceneInitialize(cameraPosition, cameraLookPosition);
}

/// <summary>
/// 更新
/// </summary>
/// <returns>次のシーン</returns>
SceneBase* ArmEnemyStartScene::Update()
{
	//更新
	camera->UpdateStartScene(cameraPosition, cameraLookPosition);

	cameraPosition.z -= CameraMoveZSpeed;

	if (cameraLookPosition.y < CameraMaxLookPosY)
	{
		cameraLookPosition.y += CameraLookPosUpYSpeed;
	}

	//黒帯を濃くする
	if (blackBandAlpha < MaxBlackBandAlpha)
	{
		blackBandAlpha += AlphaIncrease;
		if (blackBandAlpha >= MaxBlackBandAlpha)
		{
			blackBandAlpha = MaxBlackBandAlpha;
		}
	}

	//シーン変更
	if (flame == ChangeSceneflame)
	{
		player->StartSceneEnd();
		return new GameScene(stage, enemy, camera, player, Time::StageTag::ArmEnemy);
	}

	flame++;
	return this;
}

/// <summary>
/// 描画
/// </summary>
void ArmEnemyStartScene::Draw()
{
	stage->Draw();
	player->Draw();
	enemy->Draw();

	//上下黒帯
	//透明度変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackBandAlpha);
	DrawBox(0, 0, SCREEN_W, 100, GetColor(0, 0, 0), TRUE);
	DrawBox(0, SCREEN_H - 100, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
}