#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"
#include"Utility.h"
#include"Input.h"
#include"BGM.h"
#include"StageBase.h"
#include"EnemyBase.h"
#include"Camera.h"
#include"Player.h"
#include"TitleScene.h"
#include"GameClearScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="stage">ステージ</param>
/// <param name="enemy">敵</param>
/// <param name="camera">カメラ</param>
/// <param name="player">プレイヤー</param>
GameClearScene::GameClearScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player, bool rankIn,int time)
{
	//インスタンス化
	input = new Input();
	bgm = new BGM(BGM::BGMKind::GameClear);
	Loader* loader = loader->GetInstance();

	//クラス引継ぎ
	this->stage = stage;
	this->enemy = enemy;
	this->camera = camera;
	this->player = player;

	//変数初期化
	this->rankIn = rankIn;
	this->time = time;
	clearTextBackImage = loader->GetHandle(Loader::Kind::ClearTextBackImage);
	canInputB = false;
	blackBandAlpha = 0;
	enemyMoveEnd = false;
	textBackAlpha = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
GameClearScene::~GameClearScene()
{
	bgm->StopBGM();
	delete camera;
	delete stage;
	delete player;
	delete enemy;
	delete bgm;
	delete input;
}

/// <summary>
/// 初期化
/// </summary>
void GameClearScene::Initialize()
{
	stage->InitializeGameClear();
	enemy->InitializeFallDown();
	camera->InitializeGameClear(InitCameraPos, enemy->GetTargetCameraPosition());
}

/// <summary>
/// シーン更新
/// </summary>
/// <returns>次のシーン</returns>
SceneBase* GameClearScene::Update()
{
	//エフェクトカメラ同期
	Effekseer_Sync3DSetting();

	bgm->PlayBGM(BGM::BGMKind::GameClear);
	enemyMoveEnd = false;
	enemyMoveEnd = enemy->UpdateFallDown(camera);
	camera->UpdateGameClear(enemy->GetTargetCameraPosition());

	//黒帯を濃くする
	if (blackBandAlpha < MaxBlackBandAlpha)
	{
		blackBandAlpha += AlphaIncrease;
		if (blackBandAlpha >= MaxBlackBandAlpha)
		{
			blackBandAlpha = MaxBlackBandAlpha;
		}
	}

	//テキストボックスを濃くする
	if (enemyMoveEnd && textBackAlpha < MaxTextBackAlpha)
	{
		textBackAlpha += AlphaIncrease;
		if (textBackAlpha >= MaxTextBackAlpha)
		{
			textBackAlpha = MaxTextBackAlpha;
		}
	}

	//Effekseer更新
	UpdateEffekseer3D();

	//Bボタン入力可能
	if (!canInputB && (Input::InputNumber::BButton & input->GetInputState()) != Input::InputNumber::BButton)
	{
		canInputB = true;
	}

	//Bボタン入力
	if (enemyMoveEnd && canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
	{
		return new TitleScene();
	}
	return this;
}

/// <summary>
/// 描画
/// </summary>
void GameClearScene::Draw()
{
	stage->Draw();
	enemy->Draw();
	player->Draw();

	if (enemyMoveEnd)
	{
		//結果表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, textBackAlpha);
		DrawExtendGraph(400, 100, 1200, 800, clearTextBackImage, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);

		//テキスト背景が最大まで濃くなったら表示
		if (textBackAlpha == MaxTextBackAlpha)
		{
			SetFontSize(80);
			DrawString(650, 230, "討伐完了", GetColor(255, 255, 255));
			if (rankIn)
			{
				SetFontSize(60);
				DrawString(670, 400, "ランクイン", GetColor(255, 255, 255));
			}
			SetFontSize(50);
			DrawFormatString(600, 500, GetColor(255, 255, 255), "タイム・・・%d 秒", time);
			SetFontSize(30);
			DrawString(700, 700, "タイトルに戻る [B]", GetColor(255, 255, 255));
		}
	}

	//上下黒帯
	//透明度変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackBandAlpha);
	DrawBox(0, 0, SCREEN_W, 100, GetColor(0, 0, 0), TRUE);
	DrawBox(0, SCREEN_H - 100, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
}