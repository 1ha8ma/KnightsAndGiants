#include"DxLib.h"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerIdle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
/// <param name="prevtargetLookDirection">�O�����Ă�������</param>
PlayerIdle::PlayerIdle(int modelHandle,VECTOR prevtargetLookDirection):PlayerStateProcessBase(modelHandle)
{
	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Idle);
	//�A�j���[�V�����̑��Đ����Ԃ����
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//�ϐ�������
	newLookDirection = prevtargetLookDirection;
	moveVec = VGet(0, 0, 0);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerIdle::~PlayerIdle()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// ������
/// </summary>
void PlayerIdle::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="playerData">�v���C���[�f�[�^</param>
/// <param name="camera">�J����</param>
/// <param name="objectCollision">�Փ˂����I�u�W�F�N�g</param>
/// <returns>�I��</returns>
bool PlayerIdle::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//�ړ��x�N�g��
	moveVec = VGet(0, 0, 0);
	//�A�j���[�V�����Đ�
	PlayAnimation(PlayAnimationSpeed, false);

	return false;
}