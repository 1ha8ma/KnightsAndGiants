#include"DxLib.h"
#include"Loader.h"
#include"CollisionManager.h"
#include"BGM.h"
#include"StageObject.h"
#include"Wall.h"
#include"ArmEnemyStage.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ArmEnemyStage::ArmEnemyStage()
{
	//�C���X�^���X��
	Loader* loader = loader->GetInstance();
	wall = new Wall();
	wood = new StageObject(loader->GetHandle(Loader::Kind::WoodModel), VGet(-2000, 1000, 0), VGet(1000, -500, 0), 800.0f, VGet(3700.0f, 1000.0f, 3500.0f), VGet(-DX_PI_F / 2, 0.0f, -0.7f), 40.0f);
	rock1 = new StageObject(loader->GetHandle(Loader::Kind::RockModel1), VGet(-500, 0, 50), VGet(400, 0, 0), 1300.0f, VGet(500, 2300, 4400), VGet(0, 0, DX_PI_F / 2), 5.0f);
	rock2 = new StageObject(loader->GetHandle(Loader::Kind::RockModel2), VGet(200, 400, 0), VGet(-600, 400, 0), 1000.0f, VGet(3500, 3000, 4000), VGet(0, 0, 0), 10.0f);
	rock3 = new StageObject(loader->GetHandle(Loader::Kind::RockModel3), VGet(-800, 1000, -500), VGet(800, -1000, -500), 1400.0f, VGet(2300, 5800, 5000), VGet(0, 0, DX_PI_F / 4), 6.0f);
	bgm = new BGM(BGM::BGMKind::ArmEnemyStage);

	//�ϐ�������
	modelHandle = loader->GetHandle(Loader::Kind::ArmEnemyStage);
	skyDomeHandle = loader->GetHandle(Loader::Kind::SkyDome);
	stagePosition = StagePosition;

	//�X�e�[�W�ݒ�
	MV1SetScale(modelHandle, VGet(StageScale, StageScale, StageScale));
	MV1SetPosition(modelHandle, stagePosition);

	//�X�J�C�h�[���ݒ�
	MV1SetScale(skyDomeHandle, VGet(SkyDomeScale, SkyDomeScale, SkyDomeScale));
	MV1SetPosition(skyDomeHandle, SkyDomePosition);

	//�����蔻��
	collisionManager = collisionManager->GetInstance();
	collisionData.tag = ObjectTag::Stage;
	collisionData.stageLeft = StageLeft;
	collisionData.stageRight = StageRight;
	collisionData.stageFront = StageFront;
	collisionData.stageBack = StageBack;

	collisionManager->AddCollisionData(&collisionData);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArmEnemyStage::~ArmEnemyStage()
{
	bgm->StopBGM();
	collisionManager->RemoveCollisionData(&collisionData);
	delete wood;
	delete wall;
	delete rock1;
	delete rock2;
	delete rock3;
	delete bgm;
}

/// <summary>
/// �Q�[���I�[�o�[�V�[��������
/// </summary>
void ArmEnemyStage::InitializeGameOver()
{
	bgm->StopBGM();
}

/// <summary>
/// �Q�[���N���A������
/// </summary>
void ArmEnemyStage::InitializeGameClear()
{
	bgm->StopBGM();
}

/// <summary>
/// �X�V
/// </summary>
void ArmEnemyStage::Update()
{
	bgm->PlayBGM(BGM::BGMKind::ArmEnemyStage);
}

/// <summary>
/// �`��
/// </summary>
void ArmEnemyStage::Draw()
{
	MV1DrawModel(skyDomeHandle);
	MV1DrawModel(modelHandle);
	wall->Draw();
	wood->Draw();
	rock1->Draw();
	rock2->Draw();
	rock3->Draw();
}