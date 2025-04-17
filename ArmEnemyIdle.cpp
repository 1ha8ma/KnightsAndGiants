#include"DxLib.h"
#include"Camera.h"
#include"ArmEnemyIdle.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="modelScale"></param>
ArmEnemyIdle::ArmEnemyIdle(int modelHandle,VECTOR prevRotate) :ArmEnemyMoveBase(modelHandle)
{
	rotate = prevRotate;
	shakingflg = false;
	shakingflame = 0;

	moveState = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
ArmEnemyIdle::~ArmEnemyIdle()
{

}

/// <summary>
/// 更新
/// </summary>
/// <returns>状態変更フラグ</returns>
bool ArmEnemyIdle::Update(Camera* camera,VECTOR playerPosition)
{
	switch (moveState)
	{
	case 0:
	{
		//高さを合わせる
		if (rotate.x > BasicRotate.x)
		{
			rotate.x -= State0RotateXSpeed;
			if (rotate.x < BasicRotate.x)
			{
				rotate.x = BasicRotate.x;
			}
		}
		if (rotate.x < BasicRotate.x)
		{
			rotate.x += State0RotateXSpeed;

			if (rotate.x > BasicRotate.x)
			{
				rotate.x = BasicRotate.x;
			}
		}

		if (rotate.x == BasicRotate.x)
		{
			moveState++;
		}
	}
	break;

	case 1:
	{
		//上下揺れ
		if (!shakingflg)
		{
			rotate.x += State1RotateXSpeed;
		}
		else
		{
			rotate.x -= State1RotateXSpeed;
		}

		if (shakingflame != 0 && shakingflame % 30 == 0)
		{
			shakingflg = !shakingflg;
			shakingflame = 0;
		}
	}
	break;

	}

	MV1SetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyFrameIndex::Upperarm, MGetRotX(rotate.x));
	shakingflame++;

	return false;
}