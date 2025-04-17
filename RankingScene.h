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

	//‰Šú‰»
	void Initialize()override;
	//XV
	SceneBase* Update()override;
	//•`‰æ
	void Draw()override;

private:
	enum class CheckEnemy :int
	{
		ArmEnemy,	//˜r‚Ì“G
	};

	//‘¼ƒNƒ‰ƒX
	Time* time;
	Input* input;
	BGM* bgm;
	SoundEffect* se;

	//•Ï”
	int armEnemyBack;		//˜r‚Ì“G”wŒi
	bool canInputStick;		//ƒXƒeƒBƒbƒN˜A‘±“ü—Í–h~
	bool canInputA;			//Aƒ{ƒ^ƒ“˜A‘±“ü—Í–h~
	int checkRanking;		//Šm”F‚·‚é“G
};