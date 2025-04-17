#pragma once
#include"SceneBase.h"

class SoundEffect;

class TutorialScene :public SceneBase
{
public:
	TutorialScene();
	~TutorialScene()override;

	void Initialize()override;
	SceneBase* Update()override;
	void Draw()override;

private:
	int backImage;	//�w�i�摜

	//���N���X
	Input* input;
	SoundEffect* se;

	bool canInputA;	//A�A�����͖h�~
};