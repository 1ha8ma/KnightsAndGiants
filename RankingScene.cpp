#include"DxLib.h"
#include"Utility.h"
#include"Loader.h"
#include"Input.h"
#include"BGM.h"
#include"SoundEffect.h"
#include"TitleScene.h"
#include"RankingScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
RankingScene::RankingScene()
{
	//�C���X�^���X��
	time = time->GetInstance();
	input = new Input();
	bgm = new BGM(BGM::BGMKind::Ranking);
	se = new SoundEffect();
	Loader* loader = loader->GetInstance();

	//�ϐ�������
	armEnemyBack = loader->GetHandle(Loader::Kind::RankingArmEnemyImage);
	canInputStick = false;
	canInputA = false;
	checkRanking = (int)CheckEnemy::ArmEnemy;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
RankingScene::~RankingScene()
{
	bgm->StopBGM();

	delete input;
	delete bgm;
	delete se;
}

/// <summary>
/// ������
/// </summary>
void RankingScene::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <returns>���̃V�[��</returns>
SceneBase* RankingScene::Update()
{
	bgm->PlayBGM(BGM::BGMKind::Ranking);

	//�A�����͊m�F
	DINPUT_JOYSTATE stick = input->GetStickInput();
	if (stick.X == 0)
	{
		canInputStick = true;
	}
	if (!canInputA && (Input::InputNumber::AButton & input->GetInputState()) != Input::InputNumber::AButton)
	{
		canInputA = true;
	}

	//���E���͂Ō���G��؂�ւ�

	//�I���{�^��(A)�Ń^�C�g���ɖ߂�
	if (canInputA && (Input::InputNumber::AButton & input->GetInputState()) == Input::InputNumber::AButton)
	{
		se->PlaySE(SoundEffect::SEKind::Crick);
		return new TitleScene();
	}
	return this;
}

/// <summary>
/// �`��
/// </summary>
void RankingScene::Draw()
{
	switch (checkRanking)
	{
	case ((int)CheckEnemy::ArmEnemy):
	{
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, armEnemyBack, TRUE);

		SetFontSize(70);
		DrawString(640, 30, "��̘r", GetColor(255, 255, 255));
		SetFontSize(50);
		DrawFormatString(600, 250, GetColor(255, 255, 255), "1st�E�E�E%d�@�b", time->GetRanking(Time::StageTag::ArmEnemy)[0]);
		DrawFormatString(600, 350, GetColor(255, 255, 255), "2nd�E�E�E%d�@�b", time->GetRanking(Time::StageTag::ArmEnemy)[1]);
		DrawFormatString(600, 450, GetColor(255, 255, 255), "3rd�E�E�E%d�@�b", time->GetRanking(Time::StageTag::ArmEnemy)[2]);
		DrawFormatString(600, 550, GetColor(255, 255, 255), "4th�E�E�E%d�@�b", time->GetRanking(Time::StageTag::ArmEnemy)[3]);
		DrawFormatString(600, 650, GetColor(255, 255, 255), "5th�E�E�E%d�@�b", time->GetRanking(Time::StageTag::ArmEnemy)[4]);
	}
	break;
	}

	DrawString(50, 840, "�^�C�g���ɖ߂� [A]", GetColor(255, 255, 255));
}