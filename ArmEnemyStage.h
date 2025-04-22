#pragma once
#include"StageBase.h"
#include"CollisionData.h"

class CollisionManager;
class StageObject;
class Wall;
class BGM;

class ArmEnemyStage :public StageBase
{
public:
	ArmEnemyStage();
	~ArmEnemyStage()override;

	void InitializeGameOver()override;
	void InitializeGameClear()override;
	void Update()override;
	void Draw()override;
private:
	//�Ǔ����蔻��
	const float StageLeft = -2450.0f;
	const float StageRight = 7150.0f;
	const float StageFront = 4530.0f;
	const float StageBack = -4770.0f;
	
	const VECTOR StagePosition = VGet(0, 0, 0);				//�X�e�[�W�|�W�V����
	const float StageScale = 25.0f;							//�X�e�[�W���f���X�P�[��
	const float SkyDomeScale = 130.0f;						//�X�J�C�h�[���X�P�[��
	const VECTOR SkyDomePosition = VGet(1000, 0, -1000);	//�X�J�C�h�[���|�W�V����
	
	//��
	const VECTOR WoodPosition = VGet(3700.0f, 1000.0f, 3500.0f);		//�|�W�V����
	const VECTOR WoodAddCapsuleStart = VGet(-2000.0f, 1000.0f, 0.0f);	//�J�v�Z���n�_�ɐL�΂���
	const VECTOR WoodAddCapsuleEnd = VGet(1000.0f, -500.0f, 0.0f);		//�J�v�Z���I�_�ɐL�΂���
	const float WoodCapsuleRadius = 800.0f;;							//�J�v�Z�����a
	const VECTOR WoodRotate = VGet(-DX_PI_F / 2, 0.0f, -0.7f);			//��]
	const float WoodScale = 40.0f;										//�`��X�P�[��

	//��1
	const VECTOR Rock1Position = VGet(500.0f, 2300.0f, 4400.0f);		//�|�W�V����
	const VECTOR Rock1AddCapsuleStart = VGet(-500.0f, 0.0f, 50.0f);		//�J�v�Z���n�_�ɐL�΂���
	const VECTOR Rock1AddCapsuleEnd = VGet(400.0f, 0.0f, 0.0f);			//�J�v�Z���I�_�ɐL�΂���
	const float Rock1CapsuleRadius = 1300.0f;							//�J�v�Z�����a
	const VECTOR Rock1Rotate = VGet(0.0f, 0.0f, DX_PI_F / 2);			//��]
	const float Rock1Scale = 5.0f;										//�`��X�P�[��

	//��2
	const VECTOR Rock2Position = VGet(3500.0f, 3000.0f, 4000.0f);		//�|�W�V����
	const VECTOR Rock2AddCapsuleStart = VGet(200.0f, 400.0f, 0.0f);		//�J�v�Z���n�_�ɐL�΂���
	const VECTOR Rock2AddCapsuleEnd = VGet(-600.0f, 400.0f, 0.0f);		//�J�v�Z���I�_�ɐL�΂���
	const float Rock2CapsuleRadius = 1000.0f;							//�J�v�Z�����a
	const VECTOR Rock2Rotate = VGet(0.0f, 0.0f, 0.0f);					//��]
	const float Rock2Scale = 10.0f;										//�`��X�P�[��

	//��3
	const VECTOR Rock3Position = VGet(2300.0f, 5800.0f, 5000.0f);			//�|�W�V����
	const VECTOR Rock3AddCapsuleStart = VGet(-800.0f, 1000.0f, -500.0f);	//�J�v�Z���n�_�ɐL�΂���
	const VECTOR Rock3AddCapsuleEnd = VGet(800.0f, -1000.0f, -500.0f);		//�J�v�Z���I�_�ɐL�΂���
	const float Rock3CapsuleRadius = 1400.0f;								//�J�v�Z�����a
	const VECTOR Rock3Rotate = VGet(0.0f, 0.0f, DX_PI_F / 4);				//��]
	const float Rock3Scale = 6.0f;											//�`��X�P�[��


	CollisionManager* collisionManager;
	CollisionData collisionData;
	StageObject* wood;
	StageObject* rock1;
	StageObject* rock2;
	StageObject* rock3;
	Wall* wall;
	BGM* bgm;

	int skyDomeHandle;	//�X�J�C�h�[���n���h��
};