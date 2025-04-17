#include"DxLib.h"
#include"GameUI.h"

GameUI::GameUI(int enemyHP, int playerHP, int playerGrip, float maxPiercingRotateZ)
{
	//���݂̃X�e�[�^�X������
	this->enemyHP = enemyHP;
	this->playerHP = playerHP;
	this->playerGrip = playerGrip;

	//�J�n���X�e�[�^�X
	startEnemyHP = enemyHP;
	startPlayerHP = playerHP;
	startPlayerGrip = playerGrip;
	this->maxPiercingRotateZ = maxPiercingRotateZ;

	//1�t���[���O�̃X�e�[�^�X
	prevEnemyHP = enemyHP;
	prevPlayerHP = playerHP;

	//���̑��ϐ�������
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
/// �f�X�g���N�^
/// </summary>
GameUI::~GameUI()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="enemyHP">�GHP</param>
/// <param name="playerHP">�v���C���[HP</param>
/// <param name="playerGrip">�v���C���[����</param>
void GameUI::Update(int enemyHP, int playerHP, int playerGrip, bool onPiercingGauge, float piercingRotateZ)
{
	//�X�e�[�^�X�擾
	this->enemyHP = enemyHP;
	this->playerHP = playerHP;
	this->playerGrip = playerGrip;
	this->onPiercingGauge = onPiercingGauge;
	this->piercingRotateZ = piercingRotateZ;

	//�_���[�W���󂯂����m�F
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

	//�_���[�W���󂯂Ă����牉�o
	if (enemyTakeDamage)
	{
		//�F�ύX
		enemyColorR += ColorChangeLowSpeed;
		if (enemyColorR > 224) { enemyColorR = 224; }
		enemyColorG += ColorChangeHighSpeed;
		if (enemyColorG > 255) { enemyColorG = 255; }
		enemyColorB += ColorChangeHighSpeed;
		if (enemyColorB > 255) { enemyColorB = 255; }

		enemyTakeDamageFlame++;

		//���t���[���o������
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
		//�F�ύX
		playerColorR += ColorChangeHighSpeed;
		if (playerColorR > 255) { playerColorR = 255; }
		playerColorG += ColorChangeLowSpeed;
		if (playerColorG > 224) { playerColorG = 224; }
		playerColorB += ColorChangeHighSpeed;
		if (playerColorB > 255) { playerColorB = 255; }

		playerTakeDamageFlame++;

		//���t���[���o������
		if (playerTakeDamageFlame >= TakeDamageChangeColorflame)
		{
			playerColorR = 75;
			playerColorG = 75;
			playerColorB = 75;
			playerTakeDamage = false;
		}
	}

	//�˂��h�����Ȃ�
	if (onPiercingGauge)
	{
		piercingPer = this->piercingRotateZ / this->maxPiercingRotateZ;
	}

	//�����v�Z
	enemyHPPer = this->enemyHP / startEnemyHP;
	playerHPPer = this->playerHP / startPlayerHP;
	playerGripPer = this->playerGrip / startPlayerGrip;

	//1�t���[���O�Ƃ��ċL��
	prevEnemyHP = enemyHP;
	prevPlayerHP = playerHP;
}

/// <summary>
/// �`��
/// </summary>
void GameUI::Draw()
{
	//�GHP
	DrawBox(50, 37, 50 + EnemyHPLength, 58, GetColor(enemyColorR,enemyColorG,enemyColorB), TRUE);
	DrawBox(50, 40, 50 + EnemyHPLength * enemyHPPer, 55, GetColor(0, 191, 255), TRUE);
	//�v���C���[HP
	DrawBox(1200, 797, 1200 + PlayerHPLength, 828, GetColor(playerColorR,playerColorG,playerColorB), TRUE);
	DrawBox(1200, 800, 1200 + PlayerHPLength * playerHPPer, 825, GetColor(220, 20, 60), TRUE);
	//�v���C���[����
	DrawCircle(1450, 730, PlayerGripSize + 3, GetColor(75, 75, 75), TRUE);
	DrawCircle(1450, 730, PlayerGripSize * playerGripPer, GetColor(240, 230, 140), TRUE);
	if (onPiercingGauge)
	{
		DrawCircle(1450, 730, PlayerGripSize * piercingPer, GetColor(127, 255, 212), TRUE);
	}
}