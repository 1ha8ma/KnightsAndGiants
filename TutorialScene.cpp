#include"DxLib.h"
#include"Utility.h"
#include"Loader.h"
#include"Input.h"
#include"SoundEffect.h"
#include"TitleScene.h"
#include"TutorialScene.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TutorialScene::TutorialScene()
{
	//�C���X�^���X��
	input = new Input();
	se = new SoundEffect();
	Loader* loader = loader->GetInstance();

	//�ϐ�������
	backImage = loader->GetHandle(Loader::Kind::TutorialBackImage);
	canInputA = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TutorialScene::~TutorialScene()
{
	delete input;
}

/// <summary>
/// ������
/// </summary>
void TutorialScene::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <returns>���̃V�[��</returns>
SceneBase* TutorialScene::Update()
{
	//B�{�^�����͉\
	if (!canInputA && (Input::InputNumber::AButton & input->GetInputState()) != Input::InputNumber::AButton)
	{
		canInputA = true;
	}

	//B�{�^������
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
void TutorialScene::Draw()
{
	//�w�i
	DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, backImage, TRUE);

	//����
	SetFontSize(40);
	DrawString(200, 150, "���X�e�B�b�N�E�E�E�ړ�", GetColor(0, 0, 0));
	DrawString(200, 230, "�E�X�e�B�b�N�E�E�E���_�ړ�", GetColor(0, 0, 0));
	DrawString(200, 310, "A�E�E�E�W�����v", GetColor(0, 0, 0));
	DrawString(200, 390, "X�E�E�E�U��", GetColor(0, 0, 0));
	DrawString(200, 470, "LB�E�E�E�^�[�Q�b�g�J����", GetColor(0, 0, 0));
	DrawString(200, 550, "RB�E�E�E�͂܂�A���Ⴊ��", GetColor(0, 0, 0));
	DrawString(200, 650, "RB + X (���Ⴊ�� + �U��)�E�E�E���˂��h���U��", GetColor(0, 0, 0));

	//���͎w��
	SetFontSize(50);
	DrawString(100, 800, "�^�C�g���ɖ߂� [A]", GetColor(0, 0, 0));
}