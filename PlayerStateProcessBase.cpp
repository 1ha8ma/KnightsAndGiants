#include<math.h>
#include"DxLib.h"
#include"PlayerStateProcessBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
PlayerStateProcessBase::PlayerStateProcessBase(int modelHandle)
{
	this->modelHandle = modelHandle;

	nowAnimPlayTime = 0.0f;
	cameraZoom = 0.0f;
	armRotateZ = 0.0f;
	rotateMatrix = MGetIdent();
}

/// <summary>
/// �A�j���[�V�����Đ�
/// </summary>
/// <param name="playSpeed">�Đ����x</param>
/// <param name="stop">�ꎞ��~</param>
void PlayerStateProcessBase::PlayAnimation(float playSpeed,bool stop)
{
	if (!stop)
	{
		nowAnimPlayTime += playSpeed;
	}

	//�A�j���[�V�������Ԃ����Đ����ԂɒB���Ă����ꍇ
	if (nowAnimPlayTime >= animTotalTime)
	{
		nowAnimPlayTime = 0.0f;
	}

	//�A�j���[�V�������f
	MV1SetAttachAnimTime(modelHandle, nowPlayAnim, nowAnimPlayTime);
}

/// <summary>
/// �A�j���[�V�����Đ�(1�x�̂�)
/// </summary>
/// <param name="playSpeed">�Đ����x</param>
/// <returns>�A�j���[�V�������I��������</returns>
bool PlayerStateProcessBase::PlayAnimationOnce(float playSpeed)
{
	bool animEndflg = false;

	if (nowAnimPlayTime >= animTotalTime)
	{
		animEndflg = true;
	}

	if (!animEndflg)
	{
		nowAnimPlayTime += playSpeed;
	}

	//�A�j���[�V�������f
	MV1SetAttachAnimTime(modelHandle, nowPlayAnim, nowAnimPlayTime);

	return animEndflg;
}

/// <summary>
/// �A�j���[�V�����t�Đ�
/// </summary>
/// <param name="playSpeed">�Đ����x</param>
/// <param name="backPoint">�t�Đ��Ŗ߂�����  0�`1�@1�ōŏ��܂�</param>
/// <returns>�I��������</returns>
bool PlayerStateProcessBase::ReversePlaybackAnimation(float playSpeed, float backPoint)
{
	bool endflg = false;

	if (nowAnimPlayTime <= animTotalTime * (1 - backPoint))
	{
		endflg = true;
	}

	if (!endflg)
	{
		nowAnimPlayTime -= playSpeed;
	}

	//�A�j���[�V�������f
	MV1SetAttachAnimTime(modelHandle, nowPlayAnim, nowAnimPlayTime);

	return endflg;
}

/// <summary>
/// �A�j���[�V�����̏I��
/// </summary>
void PlayerStateProcessBase::DetachAnimation(int attachIndex)
{
	MV1DetachAnim(modelHandle, attachIndex);
}