#pragma once
#include"SceneBase.h"

class Input;
class BGM;
class SoundEffect;

class TitleScene :public SceneBase
{
public:
	TitleScene();
	~TitleScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	//タイトル状態
	enum class State :int
	{
		Title,		//タイトル名と入力指示
		Select,		//シーンセレクト
	};

	//カーソルのさしている場所
	enum class Cursor :int
	{
		ArmEnemyStage,	//腕の敵ステージ
		CheckRanking,	//ランキング確認
		Tutorial,		//チュートリアル
	};
	
	const float InitTrianglePosY = 220;				//カーソル三角の初期位置
	const int StartStringAlphaMin = 100;			//スタート文字の最小濃さ
	const int AlphaIncrease = 2;					//濃くする速さ
	const int ChangeStageStringAlphaIncrease = 5;	//タイトルから場面選択に行く時の文字のきえる速さ
	const float CursorSpeed = 100;					//カーソル移動速度

	//他クラス
	Input* input;
	BGM* bgm;
	SoundEffect* se;

	State state;				//タイトルの状態
	int cursor;					//カーソルのさしている場所
	int titleBackImage;			//背景画像
	int canInputB;				//Bボタン入力可能
	int inputOrderAlpha;		//ボタン指示透明度
	bool inputOrderflg;			//入力指示の透明度
	float trianglePosY;			//三角形カーソルのポジション
	bool canInputStick;			//スティック入力可能
	bool stateChange;			//タイトル内のステート変更
	int stringAlpha;			//文字の透明度
};