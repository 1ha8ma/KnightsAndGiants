#include<fstream>
#include<math.h>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"Camera.h"
#include"PlayerJump.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="prevmoveVec">前のmoveVec</param>
/// <param name="prevLookDir">前の向いている方向</param>
PlayerJump::PlayerJump(int modelHandle,VECTOR prevmoveVec,VECTOR prevLookDir) :PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Jump);
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
	moveVecV = prevmoveVec;
	moveVecV.y = 0.0f;
	newLookDirection = prevLookDir;
	FirstJumpPower = jsonData["JumpPower"];
	MoveSpeedHorizon = jsonData["FallingHorizonMoveSpeed"];
	jumpPower = FirstJumpPower;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerJump::~PlayerJump()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerJump::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerData">プレイヤーデータ</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCollision">衝突したオブジェクト</param>
/// <returns>終了</returns>
bool PlayerJump::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	//移動
	Move(playerData, camera);
	//アニメーション再生
	PlayAnimationOnce(PlayAnimationSpeed);

	return false;
}

/// <summary>
/// 移動処理
/// </summary>
void PlayerJump::Move(UsePlayerData playerData, Camera camera)
{
	//初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	//水平方向を追加
	moveVec = VAdd(moveVec, moveVecV);

	//高さ加算
	moveVec.y += jumpPower;
}