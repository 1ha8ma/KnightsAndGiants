#include"DxLib.h"
#include"Camera.h"
#include"ArmEnemyIdle.h"

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
ArmEnemyIdle::~ArmEnemyIdle()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <returns>��ԕύX�t���O</returns>
bool ArmEnemyIdle::Update(Camera* camera,VECTOR playerPosition)
{
	switch (moveState)
	{
	case 0:
	{
		//���������킹��
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
		//�㉺�h��
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