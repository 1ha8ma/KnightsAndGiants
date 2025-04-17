#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"

//インスタンスを最初に空にする
Loader* Loader::loader = nullptr;

/// <summary>
/// インスタンスゲット
/// </summary>
/// <returns>インスタンス</returns>
Loader* Loader::GetInstance()
{
	//インスタンスが無ければ生成
	if (loader == nullptr)
	{
		loader = new Loader();
	}

	return loader;
}

/// <summary>
/// ロード
/// </summary>
void Loader::Load()
{
	//3D
	handle[ArmEnemyStage] = MV1LoadModel("Data/3D/ArmEnemyStage.mv1");
	handle[PlayerModel] = MV1LoadModel("Data/3D/Player.mv1");
	handle[WallModel] = MV1LoadModel("Data/3D/Wall.mv1");
	handle[ArmEnemyModel] = MV1LoadModel("Data/3D/monster.mv1");
	handle[WoodModel] = MV1LoadModel("Data/3D/Wood.mv1");
	handle[SkyDome] = MV1LoadModel("Data/3D/Dome_Y901.x");
	handle[Enemy2Model] = MV1LoadModel("Data/3D/Enemy2.mv1");
	handle[RockModel1] = MV1LoadModel("Data/3D/Rock1.mv1");
	handle[RockModel2] = MV1LoadModel("Data/3D/Rock2.mv1");
	handle[RockModel3] = MV1LoadModel("Data/3D/Rock3.mv1");
	handle[RockModel4] = MV1LoadModel("Data/3D/Rock4.mv1");

	//2D
	handle[GameOverBackImage] = LoadGraph("Data/2D/GameOverBack.jpg");
	handle[TitleBackImage] = LoadGraph("Data/2D/TitleBack.jpg");
	handle[RankingArmEnemyImage] = LoadGraph("Data/2D/RankingArmEnemy.png");
	handle[ClearTextBackImage] = LoadGraph("Data/2D/ClearTextBack.png");
	handle[TutorialBackImage] = LoadGraph("Data/2D/TutorialBack.jpg");
	handle[DemoMovie] = LoadGraph("Data/2D/DemoMovie.mp4");

	//BGM
	handle[TitleBGM] = LoadSoundMem("Data/Sound/BGM/TitleBGM.mp3");
	handle[GameOverBGM] = LoadSoundMem("Data/Sound/BGM/GameOverBGM.mp3");
	handle[ArmEnemyBGM] = LoadSoundMem("Data/Sound/BGM/ArmEnemyBGM.mp3");
	handle[GameClearBGM] = LoadSoundMem("Data/Sound/BGM/GameClearBGM.mp3");
	handle[RankingBGM] = LoadSoundMem("Data/Sound/BGM/RankingBGM.mp3");

	//SE
	handle[MowinDownSE] = LoadSoundMem("Data/Sound/SE/MowingDownSE.mp3");
	handle[NormalAttackSE] = LoadSoundMem("Data/Sound/SE/NormalAttackSE.mp3");
	handle[SwingSE] = LoadSoundMem("Data/Sound/SE/SwingSE.mp3");
	handle[HitWeakPointSE] = LoadSoundMem("Data/Sound/SE/HitWeakPointSE.mp3");
	handle[CursorMoveSE] = LoadSoundMem("Data/Sound/SE/CursorMoveSE.mp3");
	handle[CrickSE] = LoadSoundMem("Data/Sound/SE/CrickSE.mp3");
	handle[RockHitSE] = LoadSoundMem("Data/Sound/SE/RockHitSE.mp3");

	//エフェクト
	handle[WeakPointEffect] = LoadEffekseerEffect("Data/Effect/WeakPoint.efkefc", 1.0f);
	handle[HitWeakPointEffect] = LoadEffekseerEffect("Data/Effect/HitWeakPoint.efkefc", 1.0f);
	handle[WarningEffect] = LoadEffekseerEffect("Data/Effect/WarningEffect.efkefc", 1.0f);
	handle[RockHitEffect] = LoadEffekseerEffect("Data/Effect/RockHitEffect.efkefc", 1.0f);
}