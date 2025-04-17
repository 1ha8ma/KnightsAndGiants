#include"DxLib.h"
#include"Utility.h"
#include"Loader.h"
#include"Input.h"
#include"SoundEffect.h"
#include"TitleScene.h"
#include"TutorialScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
TutorialScene::TutorialScene()
{
	//インスタンス化
	input = new Input();
	se = new SoundEffect();
	Loader* loader = loader->GetInstance();

	//変数初期化
	backImage = loader->GetHandle(Loader::Kind::TutorialBackImage);
	canInputA = false;
}

/// <summary>
/// デストラクタ
/// </summary>
TutorialScene::~TutorialScene()
{
	delete input;
}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <returns>次のシーン</returns>
SceneBase* TutorialScene::Update()
{
	//Bボタン入力可能
	if (!canInputA && (Input::InputNumber::AButton & input->GetInputState()) != Input::InputNumber::AButton)
	{
		canInputA = true;
	}

	//Bボタン入力
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
void TutorialScene::Draw()
{
	//背景
	DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, backImage, TRUE);

	//説明
	SetFontSize(40);
	DrawString(200, 150, "左スティック・・・移動", GetColor(0, 0, 0));
	DrawString(200, 230, "右スティック・・・視点移動", GetColor(0, 0, 0));
	DrawString(200, 310, "A・・・ジャンプ", GetColor(0, 0, 0));
	DrawString(200, 390, "X・・・攻撃", GetColor(0, 0, 0));
	DrawString(200, 470, "LB・・・ターゲットカメラ", GetColor(0, 0, 0));
	DrawString(200, 550, "RB・・・掴まり、しゃがみ", GetColor(0, 0, 0));
	DrawString(200, 650, "RB + X (しゃがみ + 攻撃)・・・下突き刺し攻撃", GetColor(0, 0, 0));

	//入力指示
	SetFontSize(50);
	DrawString(100, 800, "タイトルに戻る [A]", GetColor(0, 0, 0));
}