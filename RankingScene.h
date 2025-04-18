#pragma once
#include"SceneBase.h"
#include"Time.h"

class Input;
class BGM;
class SoundEffect;

class RankingScene :public SceneBase
{
public:
	RankingScene();
	~RankingScene()override;

	//初期化
	void Initialize()override;
	//更新
	SceneBase* Update()override;
	//描画
	void Draw()override;

private:
	enum class CheckEnemy :int
	{
		ArmEnemy,	//腕の敵
	};

	//他クラス
	Time* time;
	Input* input;
	BGM* bgm;
	SoundEffect* se;

	//変数
	int armEnemyBack;		//腕の敵背景
	bool canInputStick;		//スティック連続入力防止
	bool canInputA;			//Aボタン連続入力防止
	int checkRanking;		//確認する敵
};