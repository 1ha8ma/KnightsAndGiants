#pragma once
#include"SceneBase.h"

class Input;
class BGM;
class StageBase;
class EnemyBase;
class Camera;
class Player;

class GameClearScene :public SceneBase
{
public:
	GameClearScene(StageBase* stage, EnemyBase* enemy, Camera* camera, Player* player, bool rankIn,int time);
	~GameClearScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	const int MaxTextBackAlpha = 200;						//�e�L�X�g�w�i�̍ő�Z��
	const int MaxBlackBandAlpha = 255;						//���т̍ő�Z��
	const int AlphaIncrease = 3;							//�Z������X�s�[�h
	const VECTOR InitCameraPos = VGet(2000, 1000, -2000);	//�����J�����|�W�V����
		
	//���N���X
	Input* input;
	BGM* bgm;
	StageBase* stage;
	EnemyBase* enemy;
	Camera* camera;
	Player* player;

	bool canInputB;			//B�A�����͖h�~
	int blackBandAlpha;		//���т̓����x
	bool enemyMoveEnd;		//�G�̓����I��
	bool rankIn;			//�����N�C��
	int clearTextBackImage;	//�e�L�X�g�w�i
	int textBackAlpha;		//�e�L�X�g�w�i�����x
	int time;				//���̉�̃^�C��
};