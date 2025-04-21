#include"DxLib.h"
#include"Loader.h"
#include"Input.h"
#include"Utility.h"
#include"CollisionData.h"
#include"BGM.h"
#include"SoundEffect.h"
#include"ArmEnemyStartScene.h"
#include"RankingScene.h"
#include"TutorialScene.h"
#include"TitleScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene()
{
	Loader* loader = loader->GetInstance();
	titleBackImage = loader->GetHandle(Loader::Kind::TitleBackImage);

	//インスタンス化
	input = new Input();
	bgm = new BGM(BGM::BGMKind::Title);
	se = new SoundEffect();

	//変数初期化
	canInputB = false;
	canInputStick = false;
	inputOrderAlpha = MaxAlpha;
	inputOrderflg = true;
	trianglePosY = InitTrianglePosY;
	stringAlpha = MaxAlpha;
	stateChange = false;
	state = State::Title;
	cursor = (int)Cursor::ArmEnemyStage;
}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
	bgm->StopBGM();
	delete input;
	delete bgm;
	delete se;
}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <returns>次のステート</returns>
SceneBase* TitleScene::Update()
{
	//bgm再生
	bgm->PlayBGM(BGM::BGMKind::Title);

	//Bボタン入力可能
	if (!canInputB && (Input::InputNumber::BButton & input->GetInputState()) != Input::InputNumber::BButton)
	{
		canInputB = true;
	}
	//スティック入力情報
	DINPUT_JOYSTATE stick = input->GetStickInput();
	if (stick.Y == 0)
	{
		canInputStick = true;
	}

	switch (state)
	{
	case State::Title:
	{
		//入力指示点滅
		if (inputOrderflg)
		{
			inputOrderAlpha -= AlphaIncrease;

			if (inputOrderAlpha <= StartStringAlphaMin)
			{
				inputOrderAlpha = StartStringAlphaMin;
				inputOrderflg = false;
			}
		}
		else
		{
			inputOrderAlpha += AlphaIncrease;

			if (inputOrderAlpha >= MaxAlpha)
			{
				inputOrderAlpha = MaxAlpha;
				inputOrderflg = true;
			}
		}

		//Bボタン入力
		if (canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
		{
			se->PlaySE(SoundEffect::SEKind::Crick);
			canInputB = false;
			stateChange = true;
		}

		//ステート変更
		if (stateChange)
		{
			stringAlpha -= ChangeStageStringAlphaIncrease;

			if (stringAlpha <= 0)
			{
				stringAlpha = 0;
				state = State::Select;
			}
		}
	}
	break;
	case State::Select:
	{
		if (stringAlpha < MaxAlpha)
		{
			stringAlpha += ChangeStageStringAlphaIncrease;
			if (stringAlpha >= MaxAlpha)
			{
				stringAlpha = MaxAlpha;
			}
		}

		//上入力
		if (stringAlpha == MaxAlpha && cursor != (int)Cursor::ArmEnemyStage && canInputStick && stick.Y < 0)
		{
			se->PlaySE(SoundEffect::SEKind::CursorMove);
			canInputStick = false;
			trianglePosY -= CursorSpeed;
			cursor--;
		}
		//下入力
		if (stringAlpha == MaxAlpha && cursor != (int)Cursor::Tutorial && canInputStick && stick.Y > 0)
		{
			se->PlaySE(SoundEffect::SEKind::CursorMove);
			canInputStick = false;
			trianglePosY += CursorSpeed;
			cursor++;
		}

		//Bボタン入力
		if (stringAlpha == MaxAlpha && canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
		{
			se->PlaySE(SoundEffect::SEKind::Crick);
			if (cursor == (int)Cursor::ArmEnemyStage)
			{
				return new ArmEnemyStartScene();
			}
			if (cursor == (int)Cursor::CheckRanking)
			{
				return new RankingScene();
			}
			if (cursor == (int)Cursor::Tutorial)
			{
				return new TutorialScene();
			}
		}
	}
	break;
	}

	return this;
}

/// <summary>
/// 描画
/// </summary>
void TitleScene::Draw()
{
	//背景
	DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, titleBackImage, TRUE);

	switch (state)
	{
	case State::Title:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, stringAlpha);
		//タイトル名
		SetFontSize(150);
		DrawString(200, 300, "騎士と巨人", GetColor(0, 0, 0));

		//ボタン指示
		//透明度変更
		if (!stateChange)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, inputOrderAlpha);
		}
		SetFontSize(50);
		DrawString(500, 500, "Press B Button", GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//透明度を元に戻す
	}
	break;
	case State::Select:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, stringAlpha);
		SetFontSize(50);
		DrawString(100, 200, "Game Start", GetColor(0, 0, 0));
		DrawString(100, 300, "ランキング", GetColor(0, 0, 0));
		DrawString(100, 400, "操作説明", GetColor(0, 0, 0));

		//三角形
		DrawTriangle(20, trianglePosY - 20, 20, trianglePosY + 20, 70, trianglePosY, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
	}
	break;
	}
}