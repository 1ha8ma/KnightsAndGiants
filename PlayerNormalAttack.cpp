#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"CollisionManager.h"
#include"SoundEffect.h"
#include"Camera.h"
#include"PlayerNormalAttack.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">通常攻撃</param>
PlayerNormalAttack::PlayerNormalAttack(int modelHandle, VECTOR prevtargetLookDirection) :PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::NormalAttack);
	//アニメーションの総再生時間を取る
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//移動ベクトル初期化
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	//目標の方向を前の方向にする
	newLookDirection = prevtargetLookDirection;

	//インスタンス化
	se = new SoundEffect();

	//ファイル読み込み
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//private変数初期化
	AttackPower = jsonData["NormalAttackPower"];
	AttackCapsuleRadius = jsonData["NormalAttackCapsuleRadius"];
	stateChangeflg = false;
	animEndflg = false;
	attack = true;
	attackHit = false;

	//当たり判定情報
	collisionManager = collisionManager->GetInstance();
	attackCollisionData.HitProcess = std::bind(&PlayerNormalAttack::OnHitObject, this, std::placeholders::_1);
	collisionManager->AddCollisionData(&attackCollisionData);

	//SE再生
	se->PlaySE(SoundEffect::SEKind::NormalAttack);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerNormalAttack::~PlayerNormalAttack()
{
	//攻撃当たり判定取り除き
	collisionManager->RemoveCollisionData(&attackCollisionData);
	//アニメーションデタッチ
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerNormalAttack::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerData">プレイヤーデータ</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCollision">衝突したオブジェクト</param>
/// <returns>終了</returns>
bool PlayerNormalAttack::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	if (!animEndflg)//再生
	{
		animEndflg = PlayAnimationOnce(PlayAnimationSpeed);
	}
	else//逆再生
	{
		attack = false;
		stateChangeflg = ReversePlaybackAnimation(ReversePlaySpeed, ReversePoint);
	}

	if (attackHit)
	{
		attack = false;
	}

	//移動はしない
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	//攻撃当たり判定更新
	VECTOR capsuleCenter = MV1GetFramePosition(modelHandle, RightHandFrameNumber);
	attackCapsuleStart = VAdd(capsuleCenter, VGet(-sin(playerData.angle + DX_PI_F) * 70, -20.0f, -cos(playerData.angle + DX_PI_F) * 70));
	attackCapsuleEnd = VAdd(capsuleCenter, VGet(sin(playerData.angle + DX_PI_F), 0.0f, cos(playerData.angle + DX_PI_F)));

	UpdateCollisionData();

	return stateChangeflg;
}

/// <summary>
/// 描画(確認用)
/// </summary>
void PlayerNormalAttack::Draw()
{
	DrawCapsule3D(attackCapsuleStart, attackCapsuleEnd, AttackCapsuleRadius, 8, GetColor(255, 105, 180), GetColor(255, 105, 180), false);
	DrawSphere3D(attackCapsuleStart, AttackCapsuleRadius, 8, GetColor(145, 154, 12), GetColor(145, 154, 12), false);
}

/// <summary>
/// 衝突後処理
/// </summary>
/// <param name="objectData">衝突したオブジェクト</param>
void PlayerNormalAttack::OnHitObject(CollisionData* objectData)
{
	if (objectData->tag == ObjectTag::Enemy || objectData->tag == ObjectTag::WeakPoint)
	{
		attackHit = true;
	}
}

/// <summary>
/// 当たり判定情報更新
/// </summary>
void PlayerNormalAttack::UpdateCollisionData()
{
	attackCollisionData.tag = ObjectTag::Attack_P;
	attackCollisionData.startPosition = attackCapsuleStart;
	attackCollisionData.endPosition = attackCapsuleEnd;
	attackCollisionData.radius = AttackCapsuleRadius;
	attackCollisionData.attackPower = AttackPower;
	attackCollisionData.isCollisionActive = attack;
}