#pragma once
#include"PlayerStateProcessBase.h"

struct CollisionData;
class CollisionManager;
class SoundEffect;

class PlayerNormalAttack :public PlayerStateProcessBase
{
public:
	PlayerNormalAttack(int modelHandle, VECTOR prevtargetLookDirection);
	~PlayerNormalAttack()override;

	//初期化
	void Initialize()override;
	//更新
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)override;

	void Draw()override;

private:
	const float PlayAnimationSpeed = 0.7f;		//再生速度
	const float ReversePlaySpeed = 0.4f;		//逆再生速度
	const float ReversePoint = 0.6f;

	//当たり判定更新
	void UpdateCollisionData();

	//衝突後処理
	void OnHitObject(CollisionData* objectData);

	//他クラス
	CollisionManager* collisionManager;
	CollisionData attackCollisionData;
	SoundEffect* se;

	int AttackPower;				//攻撃力
	float AttackCapsuleRadius;		//攻撃当たり判定半径
	bool stateChangeflg;			//ステート変更フラグ
	bool animEndflg;				//アニメーション終了
	bool attack;					//攻撃中
	bool attackHit;					//攻撃が当たった
	VECTOR attackCapsuleStart;		//攻撃カプセル始点
	VECTOR attackCapsuleEnd;		//攻撃カプセル終点
};