#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"Camera.h"
#include"ArmEnemyHandUp.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
ArmEnemyHandUp::ArmEnemyHandUp(int modelHandle,VECTOR prevRotate) :ArmEnemyMoveBase(modelHandle)
{
	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/ArmEnemyData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//�ϐ�������
	coolTime = jsonData["HandUpCoolTime"];
	//�ϐ�������
	rotate = prevRotate;
	moveState = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArmEnemyHandUp::~ArmEnemyHandUp()
{
	
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="camera">�J�������</param>
/// <returns>�����̏I��</returns>
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

	//��]�s��K��
	MV1SetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyFrameIndex::Upperarm, MGetRotX(rotate.x));

	return moveEnd;
}