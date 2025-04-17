#pragma once
#include<map>

class Loader
{
public:
	static Loader* GetInstance();//インスタンスゲット

	enum Kind
	{
		//3D
		PlayerModel,					//プレイヤー
		ArmEnemyModel,					//腕の敵
		WallModel,						//壁
		BossEnemyModel,					//ボス
		WoodModel,						//木
		ArmEnemyStage,					//腕の敵戦ステージ
		BossEnemyStage,					//ボス戦ステージ
		SkyDome,						//スカイドーム
		Enemy2Model,					//2番目の敵モデル
		RockModel1,						//岩1
		RockModel2,						//岩2
		RockModel3,						//岩3
		RockModel4,						//岩4

		//2D
		GameOverBackImage,				//ゲームオーバー背景
		TitleBackImage,					//タイトル背景
		RankingArmEnemyImage,			//ランキング画面の腕の敵背景
		ClearTextBackImage,				//クリア画面のテキスト背景
		TutorialBackImage,				//操作説明背景
		DemoMovie,						//デモ映像

		//BGM
		TitleBGM,						//タイトルBGM
		GameOverBGM,					//ゲームオーバーBGM
		ArmEnemyBGM,					//腕の敵戦
		GameClearBGM,					//ゲームクリアBGM
		RankingBGM,						//ランキング画面BGM

		//SE
		MowinDownSE,					//振り下ろし攻撃SE
		NormalAttackSE,					//通常攻撃SE
		SwingSE,						//振りまわりSE
		HitWeakPointSE,					//弱点ヒットSE
		CrickSE,						//決定SE
		CursorMoveSE,					//カーソル移動SE
		RockHitSE,						//岩衝突SE

		//エフェクト
		WeakPointEffect,				//弱点
		HitWeakPointEffect,				//弱点ヒットエフェクト
		WarningEffect,					//岩が落ちてくる場所の注意エフェクト
		RockHitEffect,					//岩が落ちた場所に出すエフェクト
	};

	//ハンドルゲット
	int GetHandle(Kind kind) { return handle[kind]; }
	//ロード
	void Load();

private:
	//インスタンス
	static Loader* loader;

	//ハンドルマップ
	std::map<Kind, int> handle;
};