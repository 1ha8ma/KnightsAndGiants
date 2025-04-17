#include"DxLib.h"
#include"SoundEffect.h"
#include"Camera.h"
#include"ArmEnemySwing.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
/// <param name="modelScale">���f���T�C�Y</param>
ArmEnemySwing::ArmEnemySwing(int modelHandle,VECTOR prevRotate) :ArmEnemyMoveBase(modelHandle)
{
	//�C���X�^���X��
	se = new SoundEffect();
	
	//private�ϐ�������
	rotate = prevRotate;
	moveEnd = false;
	moveState = 0;

	//se�Đ�
	se->PlaySE(SoundEffect::SEKind::Swing);

	//�U��
	StartJoypadVibration(DX_INPUT_PAD1, JoyPadVibPower, JoyPadVibflame, -1);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArmEnemySwing::~ArmEnemySwing()
{
	//�U�����~�߂�
	StopJoypadVibration(DX_INPUT_PAD1, -1);
}

/// <summary>
/// �X�V
/// </summary>
/// <returns>��ԕύX</returns>
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