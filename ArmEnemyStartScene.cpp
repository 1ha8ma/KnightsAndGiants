#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Utility.h"
#include"Camera.h"
#include"ArmEnemy.h"
#include"ArmEnemyStage.h"
#include"Player.h"
#include"GameScene.h"
#include"Time.h"
#include"ArmEnemyStartScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ArmEnemyStartScene::ArmEnemyStartScene()
{
	//�C���X�^���X��
	camera = new Camera();
	player = new Player();
	stage = new ArmEnemyStage();
	enemy = new ArmEnemy();

	//�ϐ�������
	flame = 0;
	blackBandAlpha = 0;
	cameraPosition = InitCameraPosition;
	cameraLookPosition = InitCameraLookPosition;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArmEnemyStartScene::~ArmEnemyStartScene()
{
	
}

/// <summary>
/// ������
/// </summary>
void ArmEnemyStartScene::Initialize()
{
	camera->StartSceneInitialize(cameraPosition, cameraLookPosition);
}

/// <summary>
/// �X�V
/// </summary>
/// <returns>���̃V�[��</returns>
SceneBase* ArmEnemyStartScene::Update()
{
	//�X�V
	camera->UpdateStartScene(cameraPosition, cameraLookPosition);

	cameraPosition.z -= CameraMoveZSpeed;

	if (cameraLookPosition.y < CameraMaxLookPosY)
	{
		cameraLookPosition.y += CameraLookPosUpYSpeed;
	}

	//���т�Z������
	if (blackBandAlpha < MaxBlackBandAlpha)
	{
		blackBandAlpha += AlphaIncrease;
		if (blackBandAlpha >= MaxBlackBandAlpha)
		{
			blackBandAlpha = MaxBlackBandAlpha;
		}
	}

	//�V�[���ύX
	if (flame == ChangeSceneflame)
	{
		player->StartSceneEnd();
		return new GameScene(stage, enemy, camera, player, Time::StageTag::ArmEnemy);
	}

	flame++;
	return this;
}

/// <summary>
/// �`��
/// </summary>
void ArmEnemyStartScene::Draw()
{
	stage->Draw();
	player->Draw();
	enemy->Draw();

	//�㉺����
	//�����x�ύX
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackBandAlpha);
	DrawBox(0, 0, SCREEN_W, 100, GetColor(0, 0, 0), TRUE);
	DrawBox(0, SCREEN_H - 100, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
}