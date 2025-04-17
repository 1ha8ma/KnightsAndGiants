#include"DxLib.h"
#include"CollisionData.h"
#include"Input.h"
#include"Camera.h"
#include"PlayerSquat.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
PlayerSquat::PlayerSquat(int modelHandle,VECTOR prevtargetLookDirection) :PlayerStateProcessBase(modelHandle)
{
	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Squat);
	//�A�j���[�V�����̑��Đ����Ԃ����
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//�ϐ�������
	newLookDirection = prevtargetLookDirection;
	changeState = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerSquat::~PlayerSquat()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="playerData">�v���C���[�f�[�^</param>
/// <param name="camera">�J����</param>
/// <param name="objectCollision">�Փ˂����I�u�W�F�N�g</param>
/// <returns>�I��</returns>
bool PlayerSquat::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	moveVec = VGet(0, 0, 0);

	PlayAnimation(PlayAnimationSpeed, false);

	//R1�𗣂��ƃX�e�[�g�ύX
	if ((Input::InputNumber::R1 & playerData.inputState) != Input::InputNumber::R1)
	{
		changeState = true;
	}

	return changeState;
}