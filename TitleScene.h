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
	const int TitleFontSize = 150;					//タイトルフォントサイズ]
	const int TitlePosX = 200;						//タイトル文字ポジションX
	const int TitlePosY = 300;						//タイトル文字ポジションY
	const int SelectSceneDrawPosX = 100;			//シーン選択のシーンの文字のポジションX
	const int SelectSceneDrawPosYTop = 200;			//シーン選択のシーンの文字の一番上のポジションY
	const int SelectSceneDrawPosYDistance = 100;	//2つのシーンの文字の幅
	const int TrianglePosX = 20;					//三角形カーソルのポジションX
	const int TrianglePosXTop = 70;					//三角形カーソルの先端ポジションX
	const int TriangleAddLengthY = 20;				//三角形カーソルのY座標に伸ばす長さ

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