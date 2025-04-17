#pragma once
#include<map>

class BGM
{
public:
	//BGMの種類
	enum class BGMKind :int
	{
		Title,				//タイトル
		GameOver,			//負けシーン
		GameClear,			//勝利シーン
		ArmEnemyStage,		//腕の敵戦
		Ranking,			//ランキング画面
	};

	BGM(BGMKind kind);
	~BGM();

	//BGM再生
	void PlayBGM(BGMKind kind);

	//BGMを止める
	void StopBGM();

private:
	//音量割合
	enum class Ratio :int
	{
		Title = 100,
		GameOver = 100,
		ArmEnemyStage = 80,
		GameClear = 100,
		Ranking = 80,
	};

	const int MaxVolume = 255;		//最大音量

	std::map<BGMKind, int> handle;	//ハンドル
	int playingBGM;					//再生中のBGM
};