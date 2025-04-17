#pragma once
#include"SceneBase.h"

class Camera;
class Player;
class ArmEnemy;
class ArmEnemyStage;

class ArmEnemyStartScene:public SceneBase
{
public:
	ArmEnemyStartScene();
	~ArmEnemyStartScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	const int MaxBlackBandAlpha = 255;										//���эő�Z��
	const int AlphaIncrease = 3;											//�Z�����鑬��
	const float CameraMaxLookPosY = 6000.0f;								//�ő�LookPosition����
	const float CameraLookPosUpYSpeed = 20.0f;								//LookPosition�̍������グ�鑬�x
	const float CameraMoveZSpeed = 4.0f;
	const VECTOR InitCameraPosition = VGet(5000.0f, 2500.0f, -1500.0f);		//�J���������ʒu
	const VECTOR InitCameraLookPosition = VGet(500.0f, 1000.0f, 1000.0f);	//�J�������������_
	const int ChangeSceneflame = 400;										//�X�^�[�g�V�[������ύX����t���[��

	//���N���X
	Camera* camera;
	Player* player;
	ArmEnemy* enemy;
	ArmEnemyStage* stage;

	int flame;							//�J�n����̃t���[����
	int blackBandAlpha;					//���т̓����x
	VECTOR cameraPosition;				//�J�����|�W�V����
	VECTOR cameraLookPosition;			//�J���������_
};