#include<fstream>
#include<math.h>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerRun.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
PlayerRun::PlayerRun(int modelHandle,VECTOR prevtargetLookDirection):PlayerStateProcessBase(modelHandle)
{
	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Run);
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
	Speed = jsonData["RunSpeed"];
	newLookDirection = prevtargetLookDirection;
	rotateMatrix = MGetIdent();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerRun::~PlayerRun()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="position">�|�W�V����</param>
/// <param name="angle">�p�x</param>
/// <param name="inputstate">���͏��</param>
/// <param name="stickstate">�X�e�B�b�N���͏��</param>
/// <param name="camera">�J����</param>
/// <param name="objectCollision">�Փ˃I�u�W�F�N�g���</param>
/// <returns>�����ł͎g��Ȃ�</returns>
bool PlayerRun::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//�ړ�����
	Move(playerData, camera);
	//�A�j���[�V�����Đ�
	PlayAnimation(PlayAnimationSpeed, !moveflg);

	return false;
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="inputstate">���͏��</param>
/// <param name="stickstate">�X�e�B�b�N���͏��</param>
/// <param name="camera">�J����</param>
void PlayerRun::Move(UsePlayerData playerData, Camera camera)
{
	//������
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	moveflg = false;
	VECTOR prevTargetLookDir = playerData.lookDirection;

	//���X�e�B�b�N�̊p�x�����
	float stickX = playerData.stickState.X;
	float stickY = -playerData.stickState.Y;

	//���͂������
	if ((stickX != 0.0f || stickY != 0.0f))
	{
		float stickAngle = atan2(stickY, stickX);

		moveVec.x = cos(stickAngle + -camera.GetangleH());
		moveVec.z = sin(stickAngle + -camera.GetangleH());

		moveflg = true;
		newLookDirection = moveVec;
	}

	//���K��
	if (VSize(moveVec) > 0)
	{
		moveVec = VNorm(moveVec);
	}

	rotateMatrix = MGetRotVec2(prevTargetLookDir, moveVec);
	//�X�s�[�h���Z
	moveVec = VScale(moveVec, Speed);
}