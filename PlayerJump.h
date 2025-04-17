#pragma once
#include"PlayerStateProcessBase.h"

class PlayerJump :public PlayerStateProcessBase
{
public:
	PlayerJump(int modelHandle,VECTOR prevmoveVec,VECTOR prevLookDir);
	~PlayerJump()override;

	//初期化
	void Initialize()override;
	//更新
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)override;

private:
	const float PlayAnimationSpeed = 1.3f;	//アニメーション再生速度

	//移動処理
	void Move(UsePlayerData playerData, Camera camera);

	VECTOR moveVecV;				//水平方向ベクトル
	float FirstJumpPower;			//最初のジャンプ力
	float MoveSpeedHorizon;			//平行移動速度
	float jumpPower;				//ジャンプ力
};