#pragma once
#include"SceneBase.h"
#include"Time.h"

class Input;
class BGM;
class SoundEffect;

class RankingScene :public SceneBase
{
public:
	RankingScene();
	~RankingScene()override;

	//������
	void Initialize()override;
	//�X�V
	SceneBase* Update()override;
	//�`��
	void Draw()override;

private:
	enum class CheckEnemy :int
	{
		ArmEnemy,	//�r�̓G
	};

	//���N���X
	Time* time;
	Input* input;
	BGM* bgm;
	SoundEffect* se;

	//�ϐ�
	int armEnemyBack;		//�r�̓G�w�i
	bool canInputStick;		//�X�e�B�b�N�A�����͖h�~
	bool canInputA;			//A�{�^���A�����͖h�~
	int checkRanking;		//�m�F����G
};