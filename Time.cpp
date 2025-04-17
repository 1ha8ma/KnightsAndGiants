#include"DxLib.h"
#include"Time.h"

//インスタンス初期化
Time* Time::timeInstance = nullptr;

/// <summary>
/// インスタンスゲット
/// </summary>
/// <returns></returns>
Time* Time::GetInstance()
{
	//インスタンスが無ければ生成
	if (timeInstance == nullptr)
	{
		timeInstance = new Time();
	}

	return timeInstance;
}

/// <summary>
/// コンストラクタ
/// </summary>
Time::Time()
{
	//変数初期化
	startTime = 0;
	endTime = 0;
	for (int i = 0; i < DisplayRankingNum; i++)
	{
		ranking[StageTag::ArmEnemy].push_back(0);
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Time::~Time()
{
	ranking[StageTag::ArmEnemy].clear();
}

/// <summary>
/// タイマースタート
/// </summary>
/// <param name="tag"></param>
void Time::StartTimer()
{
	if (tag == StageTag::ArmEnemy)
	{
		//6個目のランキングがその回のタイム
		ranking[tag].push_back(0);
	}

	startTime = GetNowCount();
}

/// <summary>
/// タイマーストップ
/// </summary>
/// <param name="tag">ステージタグ</param>
/// <param name="clear">クリアしているか</param>
bool Time::StopTimer(bool clear)
{
	endTime = (GetNowCount() - startTime) / 1000;	//終了時間(秒)
	bool rankIn = false;

	if (clear)
	{
		ranking[tag].back() = endTime;
		rankIn = SortRanking(ranking[tag], endTime);
	}

	//最下位を消去
	ranking[tag].pop_back();

	return rankIn;
}

/// <summary>
/// ランキング並べ替え
/// </summary>
/// <param name="ranking">並べ替えをするランキング</param>
/// <param name="thisTime">その回のタイム</param>
/// <returns>ランキングに入ったか</returns>
bool Time::SortRanking(std::vector<int>& ranking,int thisTime)
{
	bool rankIn = false;

	//バブルソート(小→大)
	for (int i = 0; i < DisplayRankingNum; i++)
	{
		for (int j = 0; j < DisplayRankingNum - i; j++)
		{
			//0を除く
			if ((ranking[j] == 0 && ranking[j + 1] != 0) ||
				ranking[j] != 0 && ranking[j + 1] != 0 && ranking[j] > ranking[j + 1])
			{
				//今回のスコアが入った場合
				if (ranking[j + 1] == thisTime)
				{
					rankIn = true;
				}

				int temp = ranking[j];
				ranking[j] = ranking[j + 1];
				ranking[j + 1] = temp;
			}
		}
	}


	return rankIn;
}

/// <summary>
/// ランキングゲット
/// </summary>
/// <param name="checkTag">確認する敵の種類</param>
/// <returns>確認したい敵のランキング</returns>
std::vector<int> Time::GetRanking(StageTag checkTag)
{
	return ranking[tag];
}