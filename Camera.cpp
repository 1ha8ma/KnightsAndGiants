#include<fstream>
#include<math.h>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"Input.h"
#include"Calculation.h"
#include"Camera.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera()
{
	input = new Input();
	calculation = new Calculation();
}

/// <summary>
/// �X�^�[�g�V�[��������
/// </summary>
/// <param name="position">�J�����|�W�V����</param>
/// <param name="lookPosition">�J���������_</param>
void Camera::StartSceneInitialize(VECTOR position,VECTOR lookPosition)
{
	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/CameraData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	Near = jsonData["Near"];
	Far = jsonData["Far"];
	this->position = position;
	this->lookPosition = lookPosition;

	//�����ݒ�
	SetCameraNearFar(Near, Far);
	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(this->position, this->lookPosition);
}

/// <summary>
/// �X�^�[�g�V�[���X�V
/// </summary>
/// <param name="position">�J�����|�W�V����</param>
/// <param name="lookPosition">�J���������_</param>
void Camera::UpdateStartScene(VECTOR position,VECTOR lookPosition)
{
	this->position = position;
	this->lookPosition = lookPosition;

	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(position, lookPosition);
}

/// <summary>
/// ������
/// </summary>
void Camera::GameInitialize(VECTOR playerPosition)
{
	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/CameraData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//private�ϐ�
	Near = jsonData["Near"];
	Far = jsonData["Far"];
	AngleSpeed = jsonData["AngleSpeed"];
	CameraPlayerTargetHeight = jsonData["CameraPlayerTargetHeight"];
	PlayerDistance = jsonData["PlayerDistance"];
	lookPosition = playerPosition;
	lookPosition.y += CameraPlayerTargetHeight;
	angleH = 0.0f;
	angleV = 0.0f;
	productionflg = false;
	shakingDirection = true;
	t = 0;
	lerpflg = false;
	LerpSpeed = jsonData["TargetCameraLerpSpeed"];

	//�����ݒ�
	SetCameraNearFar(Near, Far);
	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(position, lookPosition);
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="playerPosition">�v���C���[�|�W�V����</param>
/// <param name="targetCameraPosition">�^�[�Q�b�g�J�����|�W�V����</param>
/// <param name="addPlayerDistance">�J�������v���C���[�ɋ߂Â��鋗��</param>
void Camera::UpdateGame(VECTOR playerPosition, VECTOR targetCameraPosition, float addPlayerDistance)
{
	//�����_�ړ�
	bool inputflg = false;
	DINPUT_JOYSTATE stickinput = input->GetStickInput();//�X�e�B�b�N���͏��

	//�E
	if (stickinput.Rx > 0.0f)
	{
		angleH += AngleSpeed;
		//180�x�𒴂�����߂�
		if (angleH > DX_PI_F)
		{
			angleH -= DX_TWO_PI_F;
		}
		inputflg = true;
	}
	//��
	if (stickinput.Rx < 0.0f)
	{
		angleH -= AngleSpeed;
		//-180�x�𒴂�����߂�
		if (angleH < -DX_PI_F)
		{
			angleH += DX_TWO_PI_F;
		}
		inputflg = true;
	}
	//��
	if (stickinput.Ry < 0.0f)
	{
		angleV -= AngleSpeed;
		//���
		if (angleV < CameraUpLimit)
		{
			angleV = CameraUpLimit;
		}
		inputflg = true;
	}
	//��
	if (stickinput.Ry > 0.0f)
	{
		angleV += AngleSpeed;
		//���
		if (angleV > CameraDownLimit)
		{
			angleV = CameraDownLimit;
		}
		inputflg = true;
	}

	//�^�[�Q�b�g�J����
	if ((Input::InputNumber::L1 & input->GetInputState()) == Input::InputNumber::L1)
	{
		if (!lerpflg)
		{
			t = 0;
			lerpflg = true;
		}

		lookPosition = calculation->Lerp(targetCameraPosition, lookPosition, LerpSpeed, t);
	}
	else
	{
		lerpflg = false;
	}

	//�E�X�e�B�b�N���_�ړ����������ꍇ
	if (!lerpflg && inputflg)
	{
		lookPosition = playerPosition;
		lookPosition.x += cos(angleH);
		lookPosition.y = playerPosition.y + CameraPlayerTargetHeight;
		lookPosition.z += sin(angleH);
	}
	else if (productionflg)
	{

	}
	//���_�ړ��Ȃ��^�[�Q�b�g�J�����Ȃ��̒ʏ펞
	else if (!lerpflg && !productionflg)
	{
		lookPosition = playerPosition;
		lookPosition.y += CameraPlayerTargetHeight;
	}

	//0����v���C���[�Ɨ�������������X�������ɂ��炷��Z����]�ō������遨Y����]�ŏꏊ�ړ����v���C���[��Y���W�������������̂𑫂�
	//NOTE:�Ō�v���C���[��Y���W�������������̂𑫂��悤�ɂ����̂�lookPosition�𑫂��ƃ^�[�Q�b�g�J�����ɂ����Ƃ��Ƀ^�[�Q�b�g�̏ꏊ�Ɉړ����Ă��܂�����
	MATRIX rotY = MGetRotY(angleH + DX_PI_F / 2);
	MATRIX rotZ = MGetRotZ(angleV);

	position = VAdd(VTransform(VTransform(VGet(PlayerDistance + addPlayerDistance, 0.0f, 0.0f), rotZ), rotY), VAdd(playerPosition, VGet(0.0f, CameraPlayerTargetHeight, 0.0f)));
	//���o�X�V
	UpdateProduction(playerPosition);

	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(position, lookPosition);
}

/// <summary>
/// �Q�[���I�[�o�[�V�[��������
/// </summary>
/// <param name="playerPosition">�v���C���[�|�W�V����</param>
void Camera::InitializeGameOver(VECTOR playerPosition)
{
	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/CameraData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//������
	Near = jsonData["GameOverNear"];
	Far = jsonData["GameOverFar"];
	LeaveSpeed = jsonData["GameOverLeaveSpeed"];
	position = playerPosition;
	position.x += 10;
	position.z += 10;
	position.y += 100;

	//�����_
	lookPosition = playerPosition;

	//�����ݒ�
	SetCameraNearFar(Near, Far);
	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(position, lookPosition);
}

/// <summary>
/// �Q�[���I�[�o�[�V�[���X�V
/// </summary>
void Camera::UpdateGameOver()
{
	position.y += LeaveSpeed;

	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(position, lookPosition);
}

/// <summary>
/// �Q�[���N���A�V�[��������
/// </summary>
/// <param name="cameraPosition">�ݒu����J�����|�W�V����</param>
/// <param name="enemyPosition">�����_�ɂȂ�G�̃|�W�V����</param>
void Camera::InitializeGameClear(VECTOR cameraPosition, VECTOR enemyPosition)
{
	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/CameraData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	Near = jsonData["Near"];
	Far = jsonData["Far"];
	position = cameraPosition;
	lookPosition = enemyPosition;

	//�����ݒ�
	SetCameraNearFar(Near, Far);
	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(position, lookPosition);
}

/// <summary>
/// �Q�[���N���A�V�[���X�V
/// </summary>
/// <param name="enemyPosition">�����_�ɂȂ�G�̃|�W�V����</param>
void Camera::UpdateGameClear(VECTOR enemyPosition)
{
	lookPosition = enemyPosition;

	//���o�X�V
	UpdateProduction(enemyPosition);

	//�|�W�V�����E�����_���f
	SetCameraPositionAndTarget_UpVecY(position, lookPosition);
}

/// <summary>
/// ���o�X�V
/// </summary>
void Camera::UpdateProduction(VECTOR playerPosition)
{
	ShakingVertical(playerPosition);
}

/// <summary>
/// �c�h��
/// </summary>
/// <param name="shakingPower">�h��̋���</param>
/// <param name="shakingDirectionChangeflame">�h��������ύX����t���[��</param>
/// <param name="flame">�Đ��t���[��</param>
void Camera::PlayShakingVertical(float shakingPower, int shakingDirectionChangeflame, int flame)
{
	//���o���łȂ���ΊJ�n
	if (!productionflg)
	{
		totalflame = 0;
		changeflame = 0;
		this->shakingPower = shakingPower;
		this->shakingDirectionChangeflame = shakingDirectionChangeflame;
		this->playflame = flame;
		shakingDirection = true;

		productionflg = true;
		shakingVerticalflg = true;
	}
}

/// <summary>
/// �c�h��
/// </summary>
/// <param name="shakingPower">�h��̋���</param>
/// <param name="shakingDirectionChangeflame">�h��������ύX����t���[��</param>
/// <param name="flame">�Đ�����</param>
void Camera::ShakingVertical(VECTOR playerPosition)
{
	if (shakingVerticalflg)
	{
		lookPosition = playerPosition;
		lookPosition.y += CameraPlayerTargetHeight;
		//�h��
		if (shakingDirection)
		{
			lookPosition.y += shakingPower * changeflame;
		}
		else
		{
			lookPosition.y -= shakingPower * changeflame;
		}

		//�h�������ύX
		if (changeflame != 0 && changeflame % shakingDirectionChangeflame == 0)
		{
			shakingDirection = !shakingDirection;
			changeflame = 0;
		}

		//�t���[�����Z
		changeflame++;
		totalflame++;

		//�I��
		if (totalflame >= playflame)
		{
			productionflg = false;
			shakingVerticalflg = false;
		}
	}
}