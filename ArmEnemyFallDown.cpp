#include"DxLib.h"
#include"Camera.h"
#include"ArmEnemyFallDown.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
/// <param name="modelScale">���f���X�P�[��</param>
ArmEnemyFallDown::ArmEnemyFallDown(int modelHandle) :ArmEnemyMoveBase(modelHandle)
{
	//�ϐ�������
	moveState = 0;
	rotate = VGet(0, 0, 0);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArmEnemyFallDown::~ArmEnemyFallDown()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="camera">�J����</param>
/// <returns>�����̏I��</returns>
bool ArmEnemyFallDown::UpdateFallDown(Camera* camera)
{
	bool moveEnd = false;
	switch (moveState)
	{
	case 0:
	{
		rotate.x += State0RotateXSpeed;

		if (rotate.x >= State0ChangeNextStateRotX)
		{
			moveState++;
		}
	}
	break;
	case 1:
	{
		rotate.x -= State1RotateXSpeed;

		if (rotate.x <= State1ChangeNextStateRotX)
		{
			//�J�����U��
			camera->PlayShakingVertical(CameraShakingPower, CameraShakingChangeDirflame, CameraShakinPlayflame);
			//�U��
			StartJoypadVibration(DX_INPUT_PAD1, JoyPadVibPower, JoyPadVibflame, -1);
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

	//��]�s��K��
	MATRIX Mrotate = MMult(MGetRotX(rotate.x), MGetRotZ(rotate.z));
	MV1SetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyFrameIndex::Upperarm, Mrotate);

	return moveEnd;
}

/// <summary>
/// �`��(�m�F�p)
/// </summary>
void ArmEnemyFallDown::Draw()
{

}