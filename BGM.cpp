#include"DxLib.h"
#include"Loader.h"
#include"BGM.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="kind">最初に使用するBGM</param>
BGM::BGM(BGMKind kind)
{
	Loader* loader = loader->GetInstance();

	//ハンドル読み込み・音量調整
	handle[BGMKind::Title] = loader->GetHandle(Loader::Kind::TitleBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::Title / 100, handle[BGMKind::Title]);
	handle[BGMKind::GameOver] = loader->GetHandle(Loader::Kind::GameOverBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::GameOver / 100, handle[BGMKind::GameOver]);
	handle[BGMKind::ArmEnemyStage] = loader->GetHandle(Loader::Kind::ArmEnemyBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::ArmEnemyStage / 100, handle[BGMKind::ArmEnemyStage]);
	handle[BGMKind::GameClear] = loader->GetHandle(Loader::Kind::GameClearBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::GameClear / 100, handle[BGMKind::GameClear]);
	handle[BGMKind::Ranking] = loader->GetHandle(Loader::Kind::RankingBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::Ranking / 100, handle[BGMKind::Ranking]);

	//初期化
	playingBGM = handle[kind];
}

/// <summary>
/// デストラクタ
/// </summary>
BGM::~BGM()
{

}

/// <summary>
/// BGM再生
/// </summary>
/// <param name="kind">再生する種類</param>
void BGM::PlayBGM(BGMKind kind)
{
	//再生中でなければ再生
	if (!CheckSoundMem(playingBGM))
	{
		playingBGM = handle[kind];
		PlaySoundMem(playingBGM, DX_PLAYTYPE_LOOP, true);
	}
}

/// <summary>
/// BGMを止める
/// </summary>
void BGM::StopBGM()
{
	if (CheckSoundMem(playingBGM))
	{
		StopSoundMem(playingBGM);
	}
}