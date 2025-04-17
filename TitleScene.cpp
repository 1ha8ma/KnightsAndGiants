#include"DxLib.h"
#include"Loader.h"
#include"Input.h"
#include"Utility.h"
#include"CollisionData.h"
#include"BGM.h"
#include"SoundEffect.h"
#include"ArmEnemyStartScene.h"
#include"RankingScene.h"
#include"TutorialScene.h"
#include"TitleScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleScene::TitleScene()
{
	Loader* loader = loader->GetInstance();
	titleBackImage = loader->GetHandle(Loader::Kind::TitleBackImage);

	//�C���X�^���X��
	input = new Input();
	bgm = new BGM(BGM::BGMKind::Title);
	se = new SoundEffect();

	//�ϐ�������
	canInputB = false;
	canInputStick = false;
	inputOrderAlpha = MaxAlpha;
	inputOrderflg = true;
	trianglePosY = InitTrianglePosY;
	stringAlpha = MaxAlpha;
	stateChange = false;
	state = State::Title;
	cursor = (int)Cursor::ArmEnemyStage;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleScene::~TitleScene()
{
	bgm->StopBGM();
	delete input;
	delete bgm;
	delete se;
}

/// <summary>
/// ������
/// </summary>
void TitleScene::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <returns>���̃X�e�[�g</returns>
SceneBase* TitleScene::Update()
{
	//bgm�Đ�
	bgm->PlayBGM(BGM::BGMKind::Title);

	//B�{�^�����͉\
	if (!canInputB && (Input::InputNumber::BButton & input->GetInputState()) != Input::InputNumber::BButton)
	{
		canInputB = true;
	}
	//�X�e�B�b�N���͏��
	DINPUT_JOYSTATE stick = input->GetStickInput();
	if (stick.Y == 0)
	{
		canInputStick = true;
	}

	switch (state)
	{
	case State::Title:
	{
		//���͎w���_��
		if (inputOrderflg)
		{
			inputOrderAlpha -= AlphaIncrease;

			if (inputOrderAlpha <= StartStringAlphaMin)
			{
				inputOrderAlpha = StartStringAlphaMin;
				inputOrderflg = false;
			}
		}
		else
		{
			inputOrderAlpha += AlphaIncrease;

			if (inputOrderAlpha >= MaxAlpha)
			{
				inputOrderAlpha = MaxAlpha;
				inputOrderflg = true;
			}
		}

		//B�{�^������
		if (canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
		{
			se->PlaySE(SoundEffect::SEKind::Crick);
			canInputB = false;
			stateChange = true;
		}

		//�X�e�[�g�ύX
		if (stateChange)
		{
			stringAlpha -= ChangeStageStringAlphaIncrease;

			if (stringAlpha <= 0)
			{
				stringAlpha = 0;
				state = State::Select;
			}
		}
	}
	break;
	case State::Select:
	{
		if (stringAlpha < MaxAlpha)
		{
			stringAlpha += ChangeStageStringAlphaIncrease;
			if (stringAlpha >= MaxAlpha)
			{
				stringAlpha = MaxAlpha;
			}
		}

		//�����
		if (stringAlpha == MaxAlpha && cursor != (int)Cursor::ArmEnemyStage && canInputStick && stick.Y < 0)
		{
			se->PlaySE(SoundEffect::SEKind::CursorMove);
			canInputStick = false;
			trianglePosY -= CursorSpeed;
			cursor--;
		}
		//������
		if (stringAlpha == MaxAlpha && cursor != (int)Cursor::Tutorial && canInputStick && stick.Y > 0)
		{
			se->PlaySE(SoundEffect::SEKind::CursorMove);
			canInputStick = false;
			trianglePosY += CursorSpeed;
			cursor++;
		}

		//B�{�^������
		if (stringAlpha == MaxAlpha && canInputB && (Input::InputNumber::BButton & input->GetInputState()) == Input::InputNumber::BButton)
		{
			se->PlaySE(SoundEffect::SEKind::Crick);
			if (cursor == (int)Cursor::ArmEnemyStage)
			{
				return new ArmEnemyStartScene();
			}
			if (cursor == (int)Cursor::CheckRanking)
			{
				return new RankingScene();
			}
			if (cursor == (int)Cursor::Tutorial)
			{
				return new TutorialScene();
			}
		}
	}
	break;
	}

	return this;
}

/// <summary>
/// �`��
/// </summary>
void TitleScene::Draw()
{
	//�w�i
	DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, titleBackImage, TRUE);

	switch (state)
	{
	case State::Title:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, stringAlpha);
		//�^�C�g����
		SetFontSize(150);
		DrawString(200, 200, "�R�m�Ƌ��l", GetColor(0, 0, 0));

		//�{�^���w��
		//�����x�ύX
		if (!stateChange)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, inputOrderAlpha);
		}
		SetFontSize(50);
		DrawString(500, 500, "Press B Button", GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//�����x�����ɖ߂�
	}
	break;
	case State::Select:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, stringAlpha);
		SetFontSize(50);
		DrawString(100, 200, "Game Start", GetColor(0, 0, 0));
		DrawString(100, 300, "�����L���O", GetColor(0, 0, 0));
		DrawString(100, 400, "�������", GetColor(0, 0, 0));

		//�O�p�`
		DrawTriangle(20, trianglePosY - 20, 20, trianglePosY + 20, 70, trianglePosY, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MaxAlpha);
	}
	break;
	}
}