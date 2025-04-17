#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"SoundEffect.h"
#include"Camera.h"
#include"ArmEnemyAttackRock.h"
#include"ArmEnemyDropRock.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
ArmEnemyDropRock::ArmEnemyDropRock(int modelHandle,VECTOR prevRotate) :ArmEnemyMoveBase(modelHandle)
{
	//インスタンス化
	se = new SoundEffect();

	//ファイル読み込み
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/ArmEnemyData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//変数初期化
	moveState = 0;
	moveEnd = false;
	rotate.x = InitRotateX;
	rotate.z = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
ArmEnemyDropRock::~ArmEnemyDropRock()
{
	delete se;
	delete rock;
}

/// <summary>
/// 更新
/// </summary>
bool ArmEnemyDropRock::Update(Camera* camera,VECTOR playerPosition)
{
	switch (moveState)
	{
	case 0:
	{
		rotate.x += State0RotateXSpeed;
		rotate.z += State0RotateZSpeed;

		if (rotate.x > State0changeNextStateRotX)
		{
			moveState++;
		}
	}
	break;
	case 1:
	{
		rotate.x -= State1RotateXSpeed;
		rotate.z -= State1RotateZSpeed;

		if (rotate.x < State1ChangeNextStateRotX)
		{
			//上下揺れ
			camera->PlayShakingVertical(CameraShakingPower, CameraShakingChangeDirflame, CameraShakinPlayflame);
			//振動
			StartJoypadVibration(DX_INPUT_PAD1, JoyPadVibPower, JoyPadVibflame, -1);
			//se再生
			se->PlaySE(SoundEffect::SEKind::MowinDownAttack);
			//落下する岩生成
			rock = new ArmEnemyAttackRock(playerPosition, camera);
			moveState++;
		}
	}
	break;
	case 2:
	{
		rock->Update();

		if (rotate.x < State2ChangeNextStateRotX)
		{
			rotate.x += State2RotateXSpeed;
		}
		else
		{
			moveState++;
		}
	}
	break;
	case 3:
	{
		rock->Update();

		if (rock->GetEnd())
		{
			moveEnd = true;
		}
	}
	break;
	}

	//回転適応
	MATRIX Mrotate = MMult(MGetRotX(rotate.x), MGetRotZ(rotate.z));
	MV1SetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyFrameIndex::Upperarm, Mrotate);

	return moveEnd;
}

/// <summary>
/// 描画
/// </summary>
void ArmEnemyDropRock::Draw()
{
	if (moveState >= 2)
	{
		rock->Draw();
	}

	//確認用
	/*if (onAttackCollision)
	{
		DrawCapsule3D(attackCapsuleStart, attackCapsuleEnd, AttackCapsuleRadius, 8, GetColor(173, 255, 47), GetColor(173, 255, 47), false);
	}*/
}