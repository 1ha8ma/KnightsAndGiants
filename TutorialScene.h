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
	int backImage;	//”wŒi‰æ‘œ

	//‘¼ƒNƒ‰ƒX
	Input* input;
	SoundEffect* se;

	bool canInputA;	//A˜A‘±“ü—Í–hŽ~
};