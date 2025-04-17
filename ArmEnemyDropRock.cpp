#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"SoundEffect.h"
#include"Camera.h"
#include"ArmEnemyAttackRock.h"
#include"ArmEnemyDropRock.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
ArmEnemyDropRock::ArmEnemyDropRock(int modelHandle,VECTOR prevRotate) :ArmEnemyMoveBase(modelHandle)
{
	//�C���X�^���X��
	se = new SoundEffect();

	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/ArmEnemyData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//�ϐ�������
	moveState = 0;
	moveEnd = false;
	rotate.x = InitRotateX;
	rotate.z = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArmEnemyDropRock::~ArmEnemyDropRock()
{
	delete se;
	delete rock;
}

/// <summary>
/// �X�V
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
			//�㉺�h��
			camera->PlayShakingVertical(CameraShakingPower, CameraShakingChangeDirflame, CameraShakinPlayflame);
			//�U��
			StartJoypadVibration(DX_INPUT_PAD1, JoyPadVibPower, JoyPadVibflame, -1);
			//se�Đ�
			se->PlaySE(SoundEffect::SEKind::MowinDownAttack);
			//��������␶��
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

	//��]�K��
	MATRIX Mrotate = MMult(MGetRotX(rotate.x), MGetRotZ(rotate.z));
	MV1SetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyFrameIndex::Upperarm, Mrotate);

	return moveEnd;
}

/// <summary>
/// �`��
/// </summary>
void ArmEnemyDropRock::Draw()
{
	if (moveState >= 2)
	{
		rock->Draw();
	}

	//�m�F�p
	/*if (onAttackCollision)
	{
		DrawCapsule3D(attackCapsuleStart, attackCapsuleEnd, AttackCapsuleRadius, 8, GetColor(173, 255, 47), GetColor(173, 255, 47), false);
	}*/
}