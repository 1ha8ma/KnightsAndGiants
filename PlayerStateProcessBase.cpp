#include<math.h>
#include"DxLib.h"
#include"PlayerStateProcessBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
PlayerStateProcessBase::PlayerStateProcessBase(int modelHandle)
{
	this->modelHandle = modelHandle;

	nowAnimPlayTime = 0.0f;
	cameraZoom = 0.0f;
	armRotateZ = 0.0f;
	rotateMatrix = MGetIdent();
}

/// <summary>
/// アニメーション再生
/// </summary>
/// <param name="playSpeed">再生速度</param>
/// <param name="stop">一時停止</param>
void PlayerStateProcessBase::PlayAnimation(float playSpeed,bool stop)
{
	if (!stop)
	{
		nowAnimPlayTime += playSpeed;
	}

	//アニメーション時間が総再生時間に達していた場合
	if (nowAnimPlayTime >= animTotalTime)
	{
		nowAnimPlayTime = 0.0f;
	}

	//アニメーション反映
	MV1SetAttachAnimTime(modelHandle, nowPlayAnim, nowAnimPlayTime);
}

/// <summary>
/// アニメーション再生(1度のみ)
/// </summary>
/// <param name="playSpeed">再生速度</param>
/// <returns>アニメーションが終了したか</returns>
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

	//アニメーション反映
	MV1SetAttachAnimTime(modelHandle, nowPlayAnim, nowAnimPlayTime);

	return animEndflg;
}

/// <summary>
/// アニメーション逆再生
/// </summary>
/// <param name="playSpeed">再生速度</param>
/// <param name="backPoint">逆再生で戻す割合  0～1　1で最初まで</param>
/// <returns>終了したか</returns>
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

	//アニメーション反映
	MV1SetAttachAnimTime(modelHandle, nowPlayAnim, nowAnimPlayTime);

	return endflg;
}

/// <summary>
/// アニメーションの終了
/// </summary>
void PlayerStateProcessBase::DetachAnimation(int attachIndex)
{
	MV1DetachAnim(modelHandle, attachIndex);
}