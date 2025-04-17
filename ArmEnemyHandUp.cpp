#include"DxLib.h"
#include"Camera.h"
#include"ArmEnemyHandUp.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
ArmEnemyHandUp::ArmEnemyHandUp(int modelHandle,VECTOR prevRotate) :ArmEnemyMoveBase(modelHandle)
{
	//変数初期化
	rotate = prevRotate;
	moveState = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
ArmEnemyHandUp::~ArmEnemyHandUp()
{
	
}

/// <summary>
/// 更新
/// </summary>
/// <param name="camera">カメラ情報</param>
/// <returns>動きの終了</returns>
bool ArmEnemyHandUp::Update(Camera* camera,VECTOR playerPosition)
{
	bool moveEnd = false;

	switch (moveState)
	{
	case 0:
	{
		rotate.x += RotateXSpeed;
		if (rotate.x > State0ChangeNextStateRotX)
		{
			moveState++;
		}
	}
	break;
	case 1:
	{
		rotate.x -= RotateXSpeed;
		if (rotate.x < State1ChangeNextStateRotX)
		{
			moveState++;
		}
	}
	break;
	case 2:
	{
		moveEnd = true;
	}
	break;
	}

	//回転行列適応
	MV1SetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyFrameIndex::Upperarm, MGetRotX(rotate.x));

	return moveEnd;
}