#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"CollisionManager.h"
#include"Camera.h"
#include"EnemyBase.h"
#include"StageBase.h"
#include"Player.h"
#include"GameUI.h"
#include"GameOverScene.h"
#include"GameClearScene.h"
#include"GameScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="stage">�X�e�[�W�C���X�^���X</param>
/// <param name="enemy">�G�C���X�^���X</param>
GameScene::GameScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player, Time::StageTag tag)
{
	//�C���X�^���X��
	collisionManager = collisionManager->GetInstance();
	this->camera = camera;
	this->stage = stage;
	this->player = player;
	this->player->InitializeGame();
	this->enemy = enemy;
	this->time = time->GetInstance();
	time->SetTag(tag);
	ui = new GameUI(this->enemy->GetHP(), player->GetHP(), player->GetGripPoint(), 1.3f);

	//�ϐ�������
	gameOver = false;
	gameClear = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameScene::~GameScene()
{
	delete ui;
}

/// <summary>
/// ������
/// </summary>
void GameScene::Initialize()
{
	collisionManager->Initialize();

	camera->GameInitialize(player->GetPosition());

	//�^�C�}�[�X�^�[�g
	time->StartTimer();
}

/// <summary>
/// �X�V
/// </summary>
/// <returns>���̃V�[��</returns>
SceneBase* GameScene::Update()
{
	//�G�t�F�N�g�J��������
	Effekseer_Sync3DSetting();

	//�N���X�X�V
	stage->Update();
	camera->UpdateGame(player->GetPositionUseCamera(), enemy->GetTargetCameraPosition(), player->GetCameraZoom());
	gameOver = player->UpdateGame(camera);
	gameClear = enemy->Update(player->GetPosition(), camera);
	ui->Update(enemy->GetHP(), player->GetHP(), player->GetGripPoint(), player->GetOnPiercingGauge(), player->GetPiercingArmRotateZ());

	//�����蔻��
	collisionManager->Update();

	//Effekseer�X�V
	UpdateEffekseer3D();

	//�Q�[���I�[�o�[
	if (gameOver)
	{
		time->StopTimer(false);
		return new GameOverScene(stage, enemy, camera, player);
	}
	//�Q�[���N���A
	if (gameClear)
	{
		bool rankIn = time->StopTimer(true);
		int thisTime = time->GetTime();
		return new GameClearScene(stage, enemy, camera, player, rankIn, thisTime);
	}

	return this;
}

/// <summary>
/// �`��
/// </summary>
void GameScene::Draw()
{
	stage->Draw();
	player->Draw();
	enemy->Draw();
	ui->Draw();
}