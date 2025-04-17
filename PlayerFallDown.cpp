#include"DxLib.h"
#include"PlayerFallDown.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
PlayerFallDown::PlayerFallDown(int modelHandle):PlayerStateProcessBase(modelHandle)
{
	this->modelHandle = modelHandle;

	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::FallDown);
	//アニメーションの総再生時間を取る
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//変数初期化
	animationEnd = false;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerFallDown::~PlayerFallDown()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// ゲームオーバーシーン更新
/// </summary>
void PlayerFallDown::UpdateGameOver()
{
	if (!animationEnd)
	{
		animationEnd = PlayAnimationOnce(PlayAnimationSpeed);
	}
}
