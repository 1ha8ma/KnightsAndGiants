#include<fstream>
#include<math.h>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerJump.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
/// <param name="prevmoveVec">�O��moveVec</param>
/// <param name="prevLookDir">�O�̌����Ă������</param>
PlayerJump::PlayerJump(int modelHandle,VECTOR prevmoveVec,VECTOR prevLookDir) :PlayerStateProcessBase(modelHandle)
{
	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Jump);
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
	moveVecV = prevmoveVec;
	moveVecV.y = 0.0f;
	newLookDirection = prevLookDir;
	FirstJumpPower = jsonData["JumpPower"];
	MoveSpeedHorizon = jsonData["FallingHorizonMoveSpeed"];
	jumpPower = FirstJumpPower;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerJump::~PlayerJump()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// ������
/// </summary>
void PlayerJump::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="playerData">�v���C���[�f�[�^</param>
/// <param name="camera">�J����</param>
/// <param name="objectCollision">�Փ˂����I�u�W�F�N�g</param>
/// <returns>�I��</returns>
bool PlayerJump::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//�ړ�
	Move(playerData, camera);
	//�A�j���[�V�����Đ�
	PlayAnimationOnce(PlayAnimationSpeed);

	return false;
}

/// <summary>
/// �ړ�����
/// </summary>
void PlayerJump::Move(UsePlayerData playerData, Camera camera)
{
	//������
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	//����������ǉ�
	moveVec = VAdd(moveVec, moveVecV);

	//�������Z
	moveVec.y += jumpPower;
}