#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"
#include"Utility.h"
#include"Input.h"
#include"BGM.h"
#include"TitleScene.h"
#include"StageBase.h"
#include"Player.h"
#include"EnemyBase.h"
#include"Camera.h"
#include"GameOverScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameOverScene::GameOverScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player)
{
	Loader* loader = loader->GetInstance();
	//�n���h�����[�h
	backImage = loader->GetHandle(Loader::Kind::GameOverBackImage);

	//�C���X�^���X��
	input = new Input();
	bgm = new BGM(BGM::BGMKind::GameOver);
	this->stage = stage;
	this->enemy = enemy;
	this->camera = camera;
	this->player = player;

	//private�ϐ�������
	canInputB = false;
	drawGameOver = false;
	flame = 0;
	backImageAlpha = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameOverScene::~GameOverScene()
{
	bgm->StopBGM();
	delete camera;
	delete stage;
	delete player;
	delete enemy;
	delete bgm;
}

/// <summary>
/// ������
/// </summary>
void GameOverScene::Initialize()
{
	VECTOR pos = player->GetPosition();
	stage->InitializeGameOver();
	camera->InitializeGameOver(pos);
	player->InitializeGameOver();
}

/// <summary>
/// �X�V
/// </summary>
/// <returns>���̃V�[��</returns>
SceneBase* GameOverScene::Update()
{
	//�G�t�F�N�g�J��������
	Effekseer_Sync3DSetting();

	//bgm�Đ�
	bgm->PlayBGM(BGM::BGMKind::GameOver);
	if (!drawGameOver)
	{
		camera->UpdateGameOver();
		player->UpdateGameOver();

		if (flame == DrawGameOverStringflame)
		{
			drawGameOver = true;
		}

		flame++;
	}
	else
	{
		backImageAlpha += AlphaIncrease;

		if (backImageAlpha >= MaxAlpha)
		{
			backImageAlpha = MaxAlpha;
		}

		//B�{�^�����͉\
		if (!canInputB && (Input::InputNumber::BButton & input->GetInputState()) != Input::InputNumber::BButton)
		{
			canInputB = true;
		}
		//B�{�^������
		if (backImageAlpha == MaxAlpha && canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
		{
			return new TitleScene();
		}
	}

	//Effekseer�X�V
	UpdateEffekseer3D();

	return this;
}

/// <summary>
/// �`��
/// </summary>
void GameOverScene::Draw()
{
	stage->Draw();
	enemy->Draw();
	player->Draw();
	if (drawGameOver)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, backImageAlpha);
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, backImage, true);
		SetFontSize(100);
		DrawString(500, 350, "Game Over", GetColor(240, 248, 255));
		SetFontSize(40);
		DrawString(600, 550, "�^�C�g���ɖ߂� [B]", GetColor(240, 248, 255));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
	}
}