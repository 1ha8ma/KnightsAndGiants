#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"
#include"Utility.h"
#include"Input.h"
#include"BGM.h"
#include"StageBase.h"
#include"EnemyBase.h"
#include"Camera.h"
#include"Player.h"
#include"TitleScene.h"
#include"GameClearScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="stage">�X�e�[�W</param>
/// <param name="enemy">�G</param>
/// <param name="camera">�J����</param>
/// <param name="player">�v���C���[</param>
GameClearScene::GameClearScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player, bool rankIn,int time)
{
	//�C���X�^���X��
	input = new Input();
	bgm = new BGM(BGM::BGMKind::GameClear);
	Loader* loader = loader->GetInstance();

	//�N���X���p��
	this->stage = stage;
	this->enemy = enemy;
	this->camera = camera;
	this->player = player;

	//�ϐ�������
	this->rankIn = rankIn;
	this->time = time;
	clearTextBackImage = loader->GetHandle(Loader::Kind::ClearTextBackImage);
	canInputB = false;
	blackBandAlpha = 0;
	enemyMoveEnd = false;
	textBackAlpha = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameClearScene::~GameClearScene()
{
	bgm->StopBGM();
	delete camera;
	delete stage;
	delete player;
	delete enemy;
	delete bgm;
	delete input;
}

/// <summary>
/// ������
/// </summary>
void GameClearScene::Initialize()
{
	stage->InitializeGameClear();
	enemy->InitializeFallDown();
	camera->InitializeGameClear(InitCameraPos, enemy->GetTargetCameraPosition());
}

/// <summary>
/// �V�[���X�V
/// </summary>
/// <returns>���̃V�[��</returns>
SceneBase* GameClearScene::Update()
{
	//�G�t�F�N�g�J��������
	Effekseer_Sync3DSetting();

	bgm->PlayBGM(BGM::BGMKind::GameClear);
	enemyMoveEnd = false;
	enemyMoveEnd = enemy->UpdateFallDown(camera);
	camera->UpdateGameClear(enemy->GetTargetCameraPosition());

	//���т�Z������
	if (blackBandAlpha < MaxBlackBandAlpha)
	{
		blackBandAlpha += AlphaIncrease;
		if (blackBandAlpha >= MaxBlackBandAlpha)
		{
			blackBandAlpha = MaxBlackBandAlpha;
		}
	}

	//�e�L�X�g�{�b�N�X��Z������
	if (enemyMoveEnd && textBackAlpha < MaxTextBackAlpha)
	{
		textBackAlpha += AlphaIncrease;
		if (textBackAlpha >= MaxTextBackAlpha)
		{
			textBackAlpha = MaxTextBackAlpha;
		}
	}

	//Effekseer�X�V
	UpdateEffekseer3D();

	//B�{�^�����͉\
	if (!canInputB && (Input::InputNumber::BButton & input->GetInputState()) != Input::InputNumber::BButton)
	{
		canInputB = true;
	}

	//B�{�^������
	if (enemyMoveEnd && canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
	{
		return new TitleScene();
	}
	return this;
}

/// <summary>
/// �`��
/// </summary>
void GameClearScene::Draw()
{
	stage->Draw();
	enemy->Draw();
	player->Draw();

	if (enemyMoveEnd)
	{
		//���ʕ\��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, textBackAlpha);
		DrawExtendGraph(400, 100, 1200, 800, clearTextBackImage, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);

		//�e�L�X�g�w�i���ő�܂ŔZ���Ȃ�����\��
		if (textBackAlpha == MaxTextBackAlpha)
		{
			SetFontSize(80);
			DrawString(650, 230, "��������", GetColor(255, 255, 255));
			if (rankIn)
			{
				SetFontSize(60);
				DrawString(670, 400, "�����N�C��", GetColor(255, 255, 255));
			}
			SetFontSize(50);
			DrawFormatString(600, 500, GetColor(255, 255, 255), "�^�C���E�E�E%d �b", time);
			SetFontSize(30);
			DrawString(700, 700, "�^�C�g���ɖ߂� [B]", GetColor(255, 255, 255));
		}
	}

	//�㉺����
	//�����x�ύX
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackBandAlpha);
	DrawBox(0, 0, SCREEN_W, 100, GetColor(0, 0, 0), TRUE);
	DrawBox(0, SCREEN_H - 100, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
}