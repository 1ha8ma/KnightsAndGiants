#include"DxLib.h"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerIdle.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="prevtargetLookDirection">前向いていた方向</param>
PlayerIdle::PlayerIdle(int modelHandle,VECTOR prevtargetLookDirection):PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Idle);
	//アニメーションの総再生時間を取る
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//変数初期化
	newLookDirection = prevtargetLookDirection;
	moveVec = VGet(0, 0, 0);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerIdle::~PlayerIdle()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerIdle::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerData">プレイヤーデータ</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCollision">衝突したオブジェクト</param>
/// <returns>終了</returns>
bool PlayerIdle::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//移動ベクトル
	moveVec = VGet(0, 0, 0);
	//アニメーション再生
	PlayAnimation(PlayAnimationSpeed, false);

	return false;
}