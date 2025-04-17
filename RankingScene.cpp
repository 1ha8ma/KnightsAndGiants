#include"DxLib.h"
#include"Utility.h"
#include"Loader.h"
#include"Input.h"
#include"BGM.h"
#include"SoundEffect.h"
#include"TitleScene.h"
#include"RankingScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
RankingScene::RankingScene()
{
	//インスタンス化
	time = time->GetInstance();
	input = new Input();
	bgm = new BGM(BGM::BGMKind::Ranking);
	se = new SoundEffect();
	Loader* loader = loader->GetInstance();

	//変数初期化
	armEnemyBack = loader->GetHandle(Loader::Kind::RankingArmEnemyImage);
	canInputStick = false;
	canInputA = false;
	checkRanking = (int)CheckEnemy::ArmEnemy;
}

/// <summary>
/// デストラクタ
/// </summary>
RankingScene::~RankingScene()
{
	bgm->StopBGM();

	delete input;
	delete bgm;
	delete se;
}

/// <summary>
/// 初期化
/// </summary>
void RankingScene::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <returns>次のシーン</returns>
SceneBase* RankingScene::Update()
{
	bgm->PlayBGM(BGM::BGMKind::Ranking);

	//連続入力確認
	DINPUT_JOYSTATE stick = input->GetStickInput();
	if (stick.X == 0)
	{
		canInputStick = true;
	}
	if (!canInputA && (Input::InputNumber::AButton & input->GetInputState()) != Input::InputNumber::AButton)
	{
		canInputA = true;
	}

	//左右入力で見る敵を切り替え

	//終了ボタン(A)でタイトルに戻る
	if (canInputA && (Input::InputNumber::AButton & input->GetInputState()) == Input::InputNumber::AButton)
	{
		se->PlaySE(SoundEffect::SEKind::Crick);
		return new TitleScene();
	}
	return this;
}

/// <summary>
/// 描画
/// </summary>
void RankingScene::Draw()
{
	switch (checkRanking)
	{
	case ((int)CheckEnemy::ArmEnemy):
	{
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, armEnemyBack, TRUE);

		SetFontSize(70);
		DrawString(640, 30, "謎の腕", GetColor(255, 255, 255));
		SetFontSize(50);
		DrawFormatString(600, 250, GetColor(255, 255, 255), "1st・・・%d　秒", time->GetRanking(Time::StageTag::ArmEnemy)[0]);
		DrawFormatString(600, 350, GetColor(255, 255, 255), "2nd・・・%d　秒", time->GetRanking(Time::StageTag::ArmEnemy)[1]);
		DrawFormatString(600, 450, GetColor(255, 255, 255), "3rd・・・%d　秒", time->GetRanking(Time::StageTag::ArmEnemy)[2]);
		DrawFormatString(600, 550, GetColor(255, 255, 255), "4th・・・%d　秒", time->GetRanking(Time::StageTag::ArmEnemy)[3]);
		DrawFormatString(600, 650, GetColor(255, 255, 255), "5th・・・%d　秒", time->GetRanking(Time::StageTag::ArmEnemy)[4]);
	}
	break;
	}

	DrawString(50, 840, "タイトルに戻る [A]", GetColor(255, 255, 255));
}