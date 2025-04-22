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
	const int TitleFontSize = 150;					//�^�C�g���t�H���g�T�C�Y]
	const int TitlePosX = 200;						//�^�C�g�������|�W�V����X
	const int TitlePosY = 300;						//�^�C�g�������|�W�V����Y
	const int SelectSceneDrawPosX = 100;			//�V�[���I���̃V�[���̕����̃|�W�V����X
	const int SelectSceneDrawPosYTop = 200;			//�V�[���I���̃V�[���̕����̈�ԏ�̃|�W�V����Y
	const int SelectSceneDrawPosYDistance = 100;	//2�̃V�[���̕����̕�
	const int TrianglePosX = 20;					//�O�p�`�J�[�\���̃|�W�V����X
	const int TrianglePosXTop = 70;					//�O�p�`�J�[�\���̐�[�|�W�V����X
	const int TriangleAddLengthY = 20;				//�O�p�`�J�[�\����Y���W�ɐL�΂�����

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