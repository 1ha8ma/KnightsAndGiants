#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerFalling.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
/// <param name="prevtargetLookDirection">�O�����Ă�������</param>
PlayerFalling::PlayerFalling(int modelHandle, VECTOR prevtargetLookDirection) :PlayerStateProcessBase(modelHandle)
{
	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Falling);
	//�A�j���[�V�����̑��Đ����Ԃ����
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//�ϐ�������
	MoveSpeed = jsonData["FallingHorizonMoveSpeed"];
	newLookDirection = prevtargetLookDirection;
	moveVec = VGet(0, 0, 0);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerFalling::~PlayerFalling()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// ������
/// </summary>
void PlayerFalling::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="playerData">�v���C���[�f�[�^</param>
/// <param name="camera">�J����</param>
/// <param name="objectCollision">�Փ˂����I�u�W�F�N�g</param>
/// <returns>�I��</returns>
bool PlayerFalling::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//�A�j���[�V�����Đ�
	PlayAnimation(PlayAnimationSpeed, false);

	Move(playerData, camera);

	return false;
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="playerData">�v���C���[�f�[�^</param>
/// <param name="camera">�J����</param>
void PlayerFalling::Move(UsePlayerData playerData, Camera camera)
{
	//������
	moveVec = VGet(0, 0, 0);
	newLookDirection = playerData.lookDirection;

	//���X�e�B�b�N�̊p�x�����
	float stickX = playerData.stickState.X;
	float stickY = -playerData.stickState.Y;

	//���͂������
	if ((stickX != 0.0f || stickY != 0.0f))
	{
		float stickAngle = atan2(stickY, stickX);

		moveVec.x = cos(stickAngle + -camera.GetangleH());
		moveVec.z = sin(stickAngle + -camera.GetangleH());

		newLookDirection = moveVec;
	}

	//���K��
	if (VSize(moveVec) > 0)
	{
		moveVec = VNorm(moveVec);
	}

	moveVec = VScale(moveVec, MoveSpeed);
}