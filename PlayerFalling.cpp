#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerFalling.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="prevtargetLookDirection">前向いていた方向</param>
PlayerFalling::PlayerFalling(int modelHandle, VECTOR prevtargetLookDirection) :PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Falling);
	//アニメーションの総再生時間を取る
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//ファイル読み込み
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//変数初期化
	MoveSpeed = jsonData["FallingHorizonMoveSpeed"];
	newLookDirection = prevtargetLookDirection;
	moveVec = VGet(0, 0, 0);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerFalling::~PlayerFalling()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerFalling::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerData">プレイヤーデータ</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCollision">衝突したオブジェクト</param>
/// <returns>終了</returns>
bool PlayerFalling::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//アニメーション再生
	PlayAnimation(PlayAnimationSpeed, false);

	Move(playerData, camera);

	return false;
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="playerData">プレイヤーデータ</param>
/// <param name="camera">カメラ</param>
void PlayerFalling::Move(UsePlayerData playerData, Camera camera)
{
	//初期化
	moveVec = VGet(0, 0, 0);
	newLookDirection = playerData.lookDirection;

	//左スティックの角度を取る
	float stickX = playerData.stickState.X;
	float stickY = -playerData.stickState.Y;

	//入力があれば
	if ((stickX != 0.0f || stickY != 0.0f))
	{
		float stickAngle = atan2(stickY, stickX);

		moveVec.x = cos(stickAngle + -camera.GetangleH());
		moveVec.z = sin(stickAngle + -camera.GetangleH());

		newLookDirection = moveVec;
	}

	//正規化
	if (VSize(moveVec) > 0)
	{
		moveVec = VNorm(moveVec);
	}

	moveVec = VScale(moveVec, MoveSpeed);
}