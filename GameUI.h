#pragma once

class GameUI
{
public:
	GameUI(int enemyHP,int playerHP,int playerGrip,float maxPiercingRotateZ);
	~GameUI();

	void Update(int enemyHP, int playerHP, int playerGrip,bool onPiercingGauge,float piercingRotateZ);
	void Draw();

private:
	const float PlayerHPLength = 300.0f;	//描画プレイヤーHPゲージの長さ
	const float EnemyHPLength = 500.0f;		//描画敵HPゲージの長さ
	const float PlayerGripSize = 50.0f;		//描画プレイヤー握力の大きさ
	const int TakeDamageChangeColorflame = 40;
	const int ColorChangeLowSpeed = 4;		//色の変更速度
	const int ColorChangeHighSpeed = 5;		

	float startEnemyHP;			//開始時敵HP
	float startPlayerHP;		//開始時プレイヤーHP
	float startPlayerGrip;		//開始時プレイヤー握力
	float maxPiercingRotateZ;	//最大突き刺し溜め

	float prevEnemyHP;			//1フレーム前の敵のHP
	float prevPlayerHP;			//1フレーム前のプレイヤーのHP

	float enemyHP;				//現在の敵HP
	float playerHP;				//現在のプレイヤーHP
	float playerGrip;			//現在のプレイヤー握力
	float piercingRotateZ;		//突き刺し腕の回転度

	float enemyHPPer;			//敵HP割合
	float playerHPPer;			//プレイヤーHP割合
	float playerGripPer;		//プレイヤー握力割合
	float piercingPer;

	bool enemyTakeDamage;		//敵がダメージを受けた
	bool playerTakeDamage;		//プレイヤーがダメージを受けた
	int enemyTakeDamageFlame;	//敵がダメージを受けた後のHPバー演出のフレーム
	int playerTakeDamageFlame;	//プレイヤーがダメージを受けた後のHPバー演出のフレーム
	bool onPiercingGauge;		//突き刺しゲージを表示

	int enemyColorR, enemyColorG, enemyColorB;
	int playerColorR, playerColorG, playerColorB;
};