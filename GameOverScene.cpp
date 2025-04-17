#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"
#include"Utility.h"
#include"Input.h"
#include"BGM.h"
#include"TitleScene.h"
#include"StageBase.h"
#include"Player.h"
#include"EnemyBase.h"
#include"Camera.h"
#include"GameOverScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameOverScene::GameOverScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player)
{
	Loader* loader = loader->GetInstance();
	//ハンドルロード
	backImage = loader->GetHandle(Loader::Kind::GameOverBackImage);

	//インスタンス化
	input = new Input();
	bgm = new BGM(BGM::BGMKind::GameOver);
	this->stage = stage;
	this->enemy = enemy;
	this->camera = camera;
	this->player = player;

	//private変数初期化
	canInputB = false;
	drawGameOver = false;
	flame = 0;
	backImageAlpha = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
GameOverScene::~GameOverScene()
{
	bgm->StopBGM();
	delete camera;
	delete stage;
	delete player;
	delete enemy;
	delete bgm;
}

/// <summary>
/// 初期化
/// </summary>
void GameOverScene::Initialize()
{
	VECTOR pos = player->GetPosition();
	stage->InitializeGameOver();
	camera->InitializeGameOver(pos);
	player->InitializeGameOver();
}

/// <summary>
/// 更新
/// </summary>
/// <returns>次のシーン</returns>
SceneBase* GameOverScene::Update()
{
	//エフェクトカメラ同期
	Effekseer_Sync3DSetting();

	//bgm再生
	bgm->PlayBGM(BGM::BGMKind::GameOver);
	if (!drawGameOver)
	{
		camera->UpdateGameOver();
		player->UpdateGameOver();

		if (flame == DrawGameOverStringflame)
		{
			drawGameOver = true;
		}

		flame++;
	}
	else
	{
		backImageAlpha += AlphaIncrease;

		if (backImageAlpha >= MaxAlpha)
		{
			backImageAlpha = MaxAlpha;
		}

		//Bボタン入力可能
		if (!canInputB && (Input::InputNumber::BButton & input->GetInputState()) != Input::InputNumber::BButton)
		{
			canInputB = true;
		}
		//Bボタン入力
		if (backImageAlpha == MaxAlpha && canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
		{
			return new TitleScene();
		}
	}

	//Effekseer更新
	UpdateEffekseer3D();

	return this;
}

/// <summary>
/// 描画
/// </summary>
void GameOverScene::Draw()
{
	stage->Draw();
	enemy->Draw();
	player->Draw();
	if (drawGameOver)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, backImageAlpha);
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, backImage, true);
		SetFontSize(100);
		DrawString(500, 350, "Game Over", GetColor(240, 248, 255));
		SetFontSize(40);
		DrawString(600, 550, "タイトルに戻る [B]", GetColor(240, 248, 255));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
	}
}