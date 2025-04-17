#include"DxLib.h"
#include"CollisionData.h"
#include"Input.h"
#include"Camera.h"
#include"PlayerSquat.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
PlayerSquat::PlayerSquat(int modelHandle,VECTOR prevtargetLookDirection) :PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Squat);
	//アニメーションの総再生時間を取る
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//変数初期化
	newLookDirection = prevtargetLookDirection;
	changeState = false;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerSquat::~PlayerSquat()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerData">プレイヤーデータ</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCollision">衝突したオブジェクト</param>
/// <returns>終了</returns>
bool PlayerSquat::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	moveVec = VGet(0, 0, 0);

	PlayAnimation(PlayAnimationSpeed, false);

	//R1を離すとステート変更
	if ((Input::InputNumber::R1 & playerData.inputState) != Input::InputNumber::R1)
	{
		changeState = true;
	}

	return changeState;
}