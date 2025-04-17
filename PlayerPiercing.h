#pragma once
#include"PlayerStateProcessBase.h"

class CollisionManager;
struct CollisionData;

/// <summary>
/// 突き刺し攻撃
/// </summary>
class PlayerPiercing :public PlayerStateProcessBase
{
public:
	PlayerPiercing(int modelHandle, VECTOR prevtargetLookDirection);
	~PlayerPiercing();

	void Initialize()override {};
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision) override;
	void Draw()override;

private:
	const float CameraZoomSpeed = 1.4f;		//カメラズーム速度

	//当たり判定情報更新
	void UpdateCollisionData();
	//突き刺し攻撃
	void PiercingAttack(int inputstate);
	//衝突後処理
	void OnHitObject(CollisionData* objectData);

	//他クラス
	CollisionManager* collisionManager;
	CollisionData attackCollisionData;	//攻撃当たり判定情報

	int LowestAttackPower;			//最低攻撃力
	float AttackCapsuleRadius;		//攻撃カプセル半径
	float MaxSwingUp;				//最大振り上げ地点
	float SwingUpSpeed;				//振り上げ速度
	float SwingDownSpeed;			//振り下ろし速度
	float ChargeMagnification;		//チャージ倍率
	bool canInputX;					//Xボタン連続入力防止フラグ
	bool changeState;				//ステート変更
	VECTOR attackCapsuleStart;		//攻撃カプセル始点
	VECTOR attackCapsuleEnd;		//攻撃カプセル終点
	int attackPower;				//攻撃力
	int attackChargeFlame;			//攻撃貯め時間
	bool attack;					//攻撃中
	bool attackEnd;					//攻撃終了
	bool attackHit;					//攻撃が当たった
	VECTOR rotate;					//腕の回転
};