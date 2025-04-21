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
	moveVecH = prevmoveVec;
	prevMoveVecH = prevmoveVec;
	moveVecH.y = 0.0f;
	newLookDirection = prevLookDir;
	FirstJumpPower = jsonData["JumpPower"];
	MoveSpeed = jsonData["RunSpeed"];
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
/// <param name="playerData">プレイヤー情報</param>
/// <param name="camera">カメラ</param>
void PlayerJump::Move(UsePlayerData playerData, Camera camera)
{
	//初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	newLookDirection = playerData.lookDirection;

	//左スティックの角度を取る
	float stickX = playerData.stickState.X;
	float stickY = -playerData.stickState.Y;

	//入力があれば
	if ((stickX != 0.0f || stickY != 0.0f))
	{
		float stickAngle = atan2(stickY, stickX);

		moveVecH.x = cos(stickAngle + -camera.GetangleH());
		moveVecH.z = sin(stickAngle + -camera.GetangleH());

		newLookDirection = moveVecH;
	}
	else
	{
		//前のフレームの方向に移動
		moveVecH = prevMoveVecH;
	}

	//正規化
	if (VSize(moveVecH) > 0)
	{
		//水平方向正規化
		moveVecH = VNorm(moveVecH);
		//水平方向を追加
		moveVec = VNorm(VAdd(moveVec, moveVecH));
		//速度追加
		moveVec = VScale(moveVec, MoveSpeed);
	}

	//高さ加算
	moveVec.y += jumpPower;

	//前の水平移動として保存
	prevMoveVecH = moveVecH;
}