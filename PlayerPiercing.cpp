#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"Input.h"
#include"Camera.h"
#include"CollisionData.h"
#include"CollisionManager.h"
#include"PlayerPiercing.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="prevtargetLookDirection">前のキャラクターの向き</param>
PlayerPiercing::PlayerPiercing(int modelHandle, VECTOR prevtargetLookDirection) :PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::Squat);
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

	//private変数初期化
	newLookDirection = prevtargetLookDirection;
	LowestAttackPower = jsonData["PiercingLowestAttackPower"];
	AttackCapsuleRadius = jsonData["PiercingAttackCapsuleRadius"];
	MaxSwingUp = jsonData["PiercingMaxSwingUp"];
	SwingUpSpeed = jsonData["PiercingSwingUpSpeed"];
	SwingDownSpeed = jsonData["PiercingSwingDownSpeed"];
	ChargeMagnification = jsonData["PiercingChargeMagnification"];
	changeState = false;
	attackPower = 0;
	attackCapsuleStart = VGet(0.0f, -1000.0f, 0.0f);
	attackCapsuleEnd = VGet(0.0f, -1000.0f, 0.0f);
	attackChargeFlame = 0;
	attackEnd = false;
	rotate = VGet(0, 0, 0);
	attack = false;
	canInputX = false;
	attackHit = false;

	//当たり判定情報
	collisionManager = collisionManager->GetInstance();
	attackCollisionData.HitProcess = std::bind(&PlayerPiercing::OnHitObject, this, std::placeholders::_1);
	collisionManager->AddCollisionData(&attackCollisionData);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerPiercing::~PlayerPiercing()
{
	//当たり判定情報取り除き
	collisionManager->RemoveCollisionData(&attackCollisionData);
	//フレーム初期化
	MV1ResetFrameUserLocalMatrix(modelHandle, RightArmFrameNumber);
	//アニメーション終了
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerData">プレイヤーデータ</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCollision">衝突したオブジェクト</param>
/// <returns>終了</returns>
bool PlayerPiercing::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	moveVec = VGet(0, 0, 0);

	//連続入力防止
	if (!canInputX && (Input::InputNumber::XButton & playerData.inputState) != Input::InputNumber::XButton)
	{
		canInputX = true;
	}

	PiercingAttack(playerData.inputState);
	UpdateCollisionData();

	//離すとステート変更
	if ((Input::InputNumber::R1 & playerData.inputState) != Input::InputNumber::R1 || attackEnd)
	{
		changeState = true;
	}

	return changeState;
}

void PlayerPiercing::Draw()
{
	DrawCapsule3D(attackCapsuleStart, attackCapsuleEnd, AttackCapsuleRadius, 8, GetColor(255, 105, 180), GetColor(255, 105, 180), false);
}

/// <summary>
/// 突き刺し攻撃
/// </summary>
void PlayerPiercing::PiercingAttack(int inputstate)
{
	if (!attack)
	{
		//腕を上げる
		if (rotate.z < MaxSwingUp)
		{
			//フレーム加算
			attackChargeFlame++;
			//カメラズーム
			cameraZoom -= CameraZoomSpeed;
			//腕回転
			rotate.z += SwingUpSpeed;
		}
	}

	//Xボタンが押されると攻撃
	if (canInputX && (Input::InputNumber::XButton & inputstate) == Input::InputNumber::XButton)
	{
		attackPower = LowestAttackPower + (attackChargeFlame * ChargeMagnification);

		attack = true;
	}

	//攻撃カプセル設定
	if (attack)
	{
		VECTOR capsuleCenter = MV1GetFramePosition(modelHandle, RightHandFrameNumber);
		attackCapsuleStart = VAdd(capsuleCenter, VGet(0.0f, -50.0f, 0.0f));
		attackCapsuleEnd = VAdd(capsuleCenter, VGet(0.0f, 0.0f, 0.0f));
	}

	//腕を振り下ろす
	if (attack)
	{
		rotate.z -= SwingDownSpeed;

		if (rotate.z < 0.0f)
		{
			attackEnd = true;
		}
	}

	//攻撃が当たっていたら
	if (attackHit)
	{
		attack = false;
	}

	armRotateZ = rotate.z;
	//腕フレーム回転
	MV1SetFrameUserLocalMatrix(modelHandle, RightArmFrameNumber, MGetRotZ(rotate.z));
}

/// <summary>
/// 衝突後処理
/// </summary>
/// <param name="objectData">衝突したオブジェクト</param>
void PlayerPiercing::OnHitObject(CollisionData* objectData)
{
	if (objectData->tag == ObjectTag::Enemy || objectData->tag == ObjectTag::WeakPoint)
	{
		attackHit = true;
	}
}

/// <summary>
/// 当たり判定情報更新
/// </summary>
void PlayerPiercing::UpdateCollisionData()
{
	attackCollisionData.tag = ObjectTag::Attack_P;
	attackCollisionData.startPosition = attackCapsuleStart;
	attackCollisionData.endPosition = attackCapsuleEnd;
	attackCollisionData.radius = AttackCapsuleRadius;
	attackCollisionData.attackPower = attackPower;
	attackCollisionData.isCollisionActive = attack;
}