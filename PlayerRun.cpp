#include<fstream>
#include<math.h>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerRun.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
PlayerRun::PlayerRun(int modelHandle,VECTOR prevtargetLookDirection):PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Run);
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
	Speed = jsonData["RunSpeed"];
	newLookDirection = prevtargetLookDirection;
	rotateMatrix = MGetIdent();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerRun::~PlayerRun()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="position">ポジション</param>
/// <param name="angle">角度</param>
/// <param name="inputstate">入力状態</param>
/// <param name="stickstate">スティック入力情報</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCollision">衝突オブジェクト情報</param>
/// <returns>ここでは使わない</returns>
bool PlayerRun::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//移動処理
	Move(playerData, camera);
	//アニメーション再生
	PlayAnimation(PlayAnimationSpeed, !moveflg);

	return false;
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="inputstate">入力情報</param>
/// <param name="stickstate">スティック入力情報</param>
/// <param name="camera">カメラ</param>
void PlayerRun::Move(UsePlayerData playerData, Camera camera)
{
	//初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	moveflg = false;
	VECTOR prevTargetLookDir = playerData.lookDirection;

	//左スティックの角度を取る
	float stickX = playerData.stickState.X;
	float stickY = -playerData.stickState.Y;

	//入力があれば
	if ((stickX != 0.0f || stickY != 0.0f))
	{
		float stickAngle = atan2(stickY, stickX);

		moveVec.x = cos(stickAngle + -camera.GetangleH());
		moveVec.z = sin(stickAngle + -camera.GetangleH());

		moveflg = true;
		newLookDirection = moveVec;
	}

	//正規化
	if (VSize(moveVec) > 0)
	{
		moveVec = VNorm(moveVec);
	}

	rotateMatrix = MGetRotVec2(prevTargetLookDir, moveVec);
	//スピード加算
	moveVec = VScale(moveVec, Speed);
}