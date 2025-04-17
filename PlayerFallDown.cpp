#include"DxLib.h"
#include"PlayerFallDown.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
PlayerFallDown::PlayerFallDown(int modelHandle):PlayerStateProcessBase(modelHandle)
{
	this->modelHandle = modelHandle;

	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::FallDown);
	//�A�j���[�V�����̑��Đ����Ԃ����
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//�ϐ�������
	animationEnd = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerFallDown::~PlayerFallDown()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// �Q�[���I�[�o�[�V�[���X�V
/// </summary>
void PlayerFallDown::UpdateGameOver()
{
	if (!animationEnd)
	{
		animationEnd = PlayAnimationOnce(PlayAnimationSpeed);
	}
}
