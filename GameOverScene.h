#pragma once
#include"SceneBase.h"

class Input;
class BGM;
class StageBase;
class EnemyBase;
class Camera;
class Player;

class GameOverScene :public SceneBase
{
public:
	GameOverScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player);
	~GameOverScene()override;

	//�V�[�����Ƃ̏�����
	void Initialize() override;
	//�V�[�����Ƃ̍X�V
	SceneBase* Update() override;
	//�V�[�����Ƃ̕`��
	void Draw() override;

private:
	const int AlphaIncrease = 2;					//�Z�����鑬��
	const int DrawGameOverStringflame = 200;		//�Q�[���I�[�o�[�̕�����\�����n�߂�t���[��

	//���N���X
	Input* input;
	BGM* bgm;
	StageBase* stage;
	EnemyBase* enemy;
	Camera* camera;
	Player* player;

	int flame;			//�Q�[���I�[�o�[�ɓ����Ă���̎���
	bool drawGameOver;	//�Q�[���I�[�o�[�\���t���O
	int backImage;		//�w�i�摜
	bool canInputB;		//B�{�^�����͉\
	int backImageAlpha;	//�w�i���ߓx
};