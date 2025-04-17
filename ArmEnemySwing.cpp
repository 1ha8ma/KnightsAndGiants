#include"DxLib.h"
#include"SoundEffect.h"
#include"Camera.h"
#include"ArmEnemySwing.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="modelScale">モデルサイズ</param>
ArmEnemySwing::ArmEnemySwing(int modelHandle,VECTOR prevRotate) :ArmEnemyMoveBase(modelHandle)
{
	//インスタンス化
	se = new SoundEffect();
	
	//private変数初期化
	rotate = prevRotate;
	moveEnd = false;
	moveState = 0;

	//se再生
	se->PlaySE(SoundEffect::SEKind::Swing);

	//振動
	StartJoypadVibration(DX_INPUT_PAD1, JoyPadVibPower, JoyPadVibflame, -1);
}

/// <summary>
/// デストラクタ
/// </summary>
ArmEnemySwing::~ArmEnemySwing()
{
	//振動を止める
	StopJoypadVibration(DX_INPUT_PAD1, -1);
}

/// <summary>
/// 更新
/// </summary>
/// <returns>状態変更</returns>
bool ArmEnemySwing::Update(Camera* camera,VECTOR plaeyrPosition)
{
	switch (moveState)
	{
	case 0:
	{
		rotate.z += RotateZSpeed;

		if (rotate.z > State0ChangeNextStateRotZ)
		{
			moveState++;
		}
	}
	break;
	case 1:
	{
		rotate.z -= RotateZSpeed;
		if (rotate.z < State1ChangeNextStateRotZ)
		{
			moveState++;
		}
	}
	break;
	case 2:
	{
		rotate.z += RotateZSpeed;
		if (rotate.z >= State2ChangeNextStateRotZ)
		{
			rotate.z = State2ChangeNextStateRotZ;
			moveEnd = true;
		}
	}
	break;
	}

	
	MATRIX Mrotate = MGetRotZ(rotate.z);
	Mrotate = MMult(Mrotate, MGetRotX(rotate.x));

	MV1SetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyFrameIndex::Upperarm, Mrotate);

	return moveEnd;
}