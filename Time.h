#pragma once
#include<vector>
#include<map>

class Time
{
public:
	static Time* GetInstance();		//インスタンスゲット

	//ステージの種類
	enum class StageTag :int
	{
		ArmEnemy,	//腕の敵戦
	};

	//タイマー開始
	void StartTimer();
	//タイマーストップ
	bool StopTimer(bool clear);

	//Get,Set
	void SetTag(StageTag tag) { this->tag = tag; }
	int GetTime() const{ return endTime; }
	std::vector<int> GetRanking(StageTag tag);

private:
	Time();
	~Time();

	static Time* timeInstance;	//インスタンス

	const int DisplayRankingNum = 5;	//表示するランキングの数

	//ランキング並べ替え
	bool SortRanking(std::vector<int>& ranking,int thisTime);

	//ランキング
	std::map<StageTag, std::vector<int>> ranking;
	//std::vector<int> armEnemyRanking;

	StageTag tag;			//その回のタグ
	int startTime;		//スタート時間
	int endTime;		//ゲーム終了時間
};