#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"CollisionManager.h"
#include"Camera.h"
#include"EnemyBase.h"
#include"StageBase.h"
#include"Player.h"
#include"GameUI.h"
#include"GameOverScene.h"
#include"GameClearScene.h"
#include"GameScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="stage">ステージインスタンス</param>
/// <param name="enemy">敵インスタンス</param>
GameScene::GameScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player, Time::StageTag tag)
{
	//インスタンス化
	collisionManager = collisionManager->GetInstance();
	this->camera = camera;
	this->stage = stage;
	this->player = player;
	this->player->InitializeGame();
	this->enemy = enemy;
	this->time = time->GetInstance();
	time->SetTag(tag);
	ui = new GameUI(this->enemy->GetHP(), player->GetHP(), player->GetGripPoint(), 1.3f);

	//変数初期化
	gameOver = false;
	gameClear = false;
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	delete ui;
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize()
{
	collisionManager->Initialize();

	camera->GameInitialize(player->GetPosition());

	//タイマースタート
	time->StartTimer();
}

/// <summary>
/// 更新
/// </summary>
/// <returns>次のシーン</returns>
SceneBase* GameScene::Update()
{
	//エフェクトカメラ同期
	Effekseer_Sync3DSetting();

	//クラス更新
	stage->Update();
	camera->UpdateGame(player->GetPositionUseCamera(), enemy->GetTargetCameraPosition(), player->GetCameraZoom());
	gameOver = player->UpdateGame(camera);
	gameClear = enemy->Update(player->GetPosition(), camera);
	ui->Update(enemy->GetHP(), player->GetHP(), player->GetGripPoint(), player->GetOnPiercingGauge(), player->GetPiercingArmRotateZ());

	//当たり判定
	collisionManager->Update();

	//Effekseer更新
	UpdateEffekseer3D();

	//ゲームオーバー
	if (gameOver)
	{
		time->StopTimer(false);
		return new GameOverScene(stage, enemy, camera, player);
	}
	//ゲームクリア
	if (gameClear)
	{
		bool rankIn = time->StopTimer(true);
		int thisTime = time->GetTime();
		return new GameClearScene(stage, enemy, camera, player, rankIn, thisTime);
	}

	return this;
}

/// <summary>
/// 描画
/// </summary>
void GameScene::Draw()
{
	stage->Draw();
	player->Draw();
	enemy->Draw();
	ui->Draw();
}