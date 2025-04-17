#pragma once
#include"SceneBase.h"

class Input;
class BGM;
class SoundEffect;

class TitleScene :public SceneBase
{
public:
	TitleScene();
	~TitleScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	//�^�C�g�����
	enum class State :int
	{
		Title,		//�^�C�g�����Ɠ��͎w��
		Select,		//�V�[���Z���N�g
	};

	//�J�[�\���̂����Ă���ꏊ
	enum class Cursor :int
	{
		ArmEnemyStage,	//�r�̓G�X�e�[�W
		CheckRanking,	//�����L���O�m�F
		Tutorial,		//�`���[�g���A��
	};
	
	const float InitTrianglePosY = 220;				//�J�[�\���O�p�̏����ʒu
	const int StartStringAlphaMin = 100;			//�X�^�[�g�����̍ŏ��Z��
	const int AlphaIncrease = 2;					//�Z�����鑬��
	const int ChangeStageStringAlphaIncrease = 5;	//�^�C�g�������ʑI���ɍs�����̕����̂����鑬��
	const float CursorSpeed = 100;					//�J�[�\���ړ����x

	//���N���X
	Input* input;
	BGM* bgm;
	SoundEffect* se;

	State state;				//�^�C�g���̏��
	int cursor;					//�J�[�\���̂����Ă���ꏊ
	int titleBackImage;			//�w�i�摜
	int canInputB;				//B�{�^�����͉\
	int inputOrderAlpha;		//�{�^���w�������x
	bool inputOrderflg;			//���͎w���̓����x
	float trianglePosY;			//�O�p�`�J�[�\���̃|�W�V����
	bool canInputStick;			//�X�e�B�b�N���͉\
	bool stateChange;			//�^�C�g�����̃X�e�[�g�ύX
	int stringAlpha;			//�����̓����x
};