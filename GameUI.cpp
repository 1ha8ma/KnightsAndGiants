#include"DxLib.h"
#include"GameUI.h"

GameUI::GameUI(int enemyHP, int playerHP, int playerGrip, float maxPiercingRotateZ)
{
	//現在のステータス初期化
	this->enemyHP = enemyHP;
	this->playerHP = playerHP;
	this->playerGrip = playerGrip;

	//開始時ステータス
	startEnemyHP = enemyHP;
	startPlayerHP = playerHP;
	startPlayerGrip = playerGrip;
	this->maxPiercingRotateZ = maxPiercingRotateZ;

	//1フレーム前のステータス
	prevEnemyHP = enemyHP;
	prevPlayerHP = playerHP;

	//その他変数初期化
	enemyTakeDamage = false;
	playerTakeDamage = false;
	enemyTakeDamageFlame = 0;
	playerTakeDamageFlame = 0;
	enemyColorR = 75;
	enemyColorG = 75;
	enemyColorB = 75;
	playerColorR = 75;
	playerColorG = 75;
	playerColorB = 75;
}

/// <summary>
/// デストラクタ
/// </summary>
GameUI::~GameUI()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="enemyHP">敵HP</param>
/// <param name="playerHP">プレイヤーHP</param>
/// <param name="playerGrip">プレイヤー握力</param>
void GameUI::Update(int enemyHP, int playerHP, int playerGrip, bool onPiercingGauge, float piercingRotateZ)
{
	//ステータス取得
	this->enemyHP = enemyHP;
	this->playerHP = playerHP;
	this->playerGrip = playerGrip;
	this->onPiercingGauge = onPiercingGauge;
	this->piercingRotateZ = piercingRotateZ;

	//ダメージを受けたか確認
	if (enemyHP != prevEnemyHP)
	{
		enemyTakeDamageFlame = 0;
		enemyTakeDamage = true;
	}
	if (playerHP != prevPlayerHP)
	{
		playerTakeDamageFlame = 0;
		playerTakeDamage = true;
	}

	//ダメージを受けていたら演出
	if (enemyTakeDamage)
	{
		//色変更
		enemyColorR += ColorChangeLowSpeed;
		if (enemyColorR > 224) { enemyColorR = 224; }
		enemyColorG += ColorChangeHighSpeed;
		if (enemyColorG > 255) { enemyColorG = 255; }
		enemyColorB += ColorChangeHighSpeed;
		if (enemyColorB > 255) { enemyColorB = 255; }

		enemyTakeDamageFlame++;

		//一定フレーム経ったら
		if (enemyTakeDamageFlame >= TakeDamageChangeColorflame)
		{
			enemyColorR = 75;
			enemyColorG = 75;
			enemyColorB = 75;
			enemyTakeDamage = false;
		}
	}
	if (playerTakeDamage)
	{
		//色変更
		playerColorR += ColorChangeHighSpeed;
		if (playerColorR > 255) { playerColorR = 255; }
		playerColorG += ColorChangeLowSpeed;
		if (playerColorG > 224) { playerColorG = 224; }
		playerColorB += ColorChangeHighSpeed;
		if (playerColorB > 255) { playerColorB = 255; }

		playerTakeDamageFlame++;

		//一定フレーム経ったら
		if (playerTakeDamageFlame >= TakeDamageChangeColorflame)
		{
			playerColorR = 75;
			playerColorG = 75;
			playerColorB = 75;
			playerTakeDamage = false;
		}
	}

	//突き刺し中なら
	if (onPiercingGauge)
	{
		piercingPer = this->piercingRotateZ / this->maxPiercingRotateZ;
	}

	//割合計算
	enemyHPPer = this->enemyHP / startEnemyHP;
	playerHPPer = this->playerHP / startPlayerHP;
	playerGripPer = this->playerGrip / startPlayerGrip;

	//1フレーム前として記憶
	prevEnemyHP = enemyHP;
	prevPlayerHP = playerHP;
}

/// <summary>
/// 描画
/// </summary>
void GameUI::Draw()
{
	//敵HP
	DrawBox(50, 37, 50 + EnemyHPLength, 58, GetColor(enemyColorR,enemyColorG,enemyColorB), TRUE);
	DrawBox(50, 40, 50 + EnemyHPLength * enemyHPPer, 55, GetColor(0, 191, 255), TRUE);
	//プレイヤーHP
	DrawBox(1200, 797, 1200 + PlayerHPLength, 828, GetColor(playerColorR,playerColorG,playerColorB), TRUE);
	DrawBox(1200, 800, 1200 + PlayerHPLength * playerHPPer, 825, GetColor(220, 20, 60), TRUE);
	//プレイヤー握力
	DrawCircle(1450, 730, PlayerGripSize + 3, GetColor(75, 75, 75), TRUE);
	DrawCircle(1450, 730, PlayerGripSize * playerGripPer, GetColor(240, 230, 140), TRUE);
	if (onPiercingGauge)
	{
		DrawCircle(1450, 730, PlayerGripSize * piercingPer, GetColor(127, 255, 212), TRUE);
	}
}