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
	moveVecH = prevmoveVec;
	prevMoveVecH = prevmoveVec;
	moveVecH.y = 0.0f;
	newLookDirection = prevLookDir;
	FirstJumpPower = jsonData["JumpPower"];
	MoveSpeed = jsonData["RunSpeed"];
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
/// <param name="playerData">�v���C���[���</param>
/// <param name="camera">�J����</param>
void PlayerJump::Move(UsePlayerData playerData, Camera camera)
{
	//������
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	newLookDirection = playerData.lookDirection;

	//���X�e�B�b�N�̊p�x�����
	float stickX = playerData.stickState.X;
	float stickY = -playerData.stickState.Y;

	//���͂������
	if ((stickX != 0.0f || stickY != 0.0f))
	{
		float stickAngle = atan2(stickY, stickX);

		moveVecH.x = cos(stickAngle + -camera.GetangleH());
		moveVecH.z = sin(stickAngle + -camera.GetangleH());

		newLookDirection = moveVecH;
	}
	else
	{
		//�O�̃t���[���̕����Ɉړ�
		moveVecH = prevMoveVecH;
	}

	//���K��
	if (VSize(moveVecH) > 0)
	{
		//�����������K��
		moveVecH = VNorm(moveVecH);
		//����������ǉ�
		moveVec = VNorm(VAdd(moveVec, moveVecH));
		//���x�ǉ�
		moveVec = VScale(moveVec, MoveSpeed);
	}

	//�������Z
	moveVec.y += jumpPower;

	//�O�̐����ړ��Ƃ��ĕۑ�
	prevMoveVecH = moveVecH;
}