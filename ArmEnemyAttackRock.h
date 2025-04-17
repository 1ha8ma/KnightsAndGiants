#pragma once
#include"CollisionData.h"

class CollisionManager;
class Effect;
class Camera;
class SoundEffect;

class ArmEnemyAttackRock
{
public:
	ArmEnemyAttackRock(VECTOR playerPosition,Camera* camera);
	~ArmEnemyAttackRock();
	//更新
	void Update();
	//描画
	void Draw();
	//終了したか
	bool GetEnd() { return end; }

private:
	const int AttackPower = 20;									//攻撃力
	const float ModelScale = 100.0f;							//モデルスケール
	const float FallSpeed = 35.0f;								//落下スピード
	const float CapsuleRadius = 300.0f;							//カプセル半径
	const float AddCollisionPosY = 170.0f;						//ポジションからあたり判定のポジションを作る際のY軸追加分
	const float ModelRotateSpeedY = 0.04f;						//モデルのY軸回転速度
	const VECTOR WarningEffectScale = VGet(80, 20, 80);			//注意エフェクトスケール
	const VECTOR WarningEffectRotate = VGet(0, 0, 0);			//注意エフェクト回転
	const float WarningEffectPlaySpeed = 1.0f;					//注意エフェクト再生速度
	const float AddHitEffectPositionY = 70.0f;					//衝突エフェクトポジションY軸追加分
	const VECTOR HitEffectScale = VGet(50, 50, 50);				//衝突エフェクトスケール
	const VECTOR HitEffectRotate = VGet(DX_PI_F / 2, 0, 0);		//衝突エフェクト回転
	const float HitEffectPlaySpeed = 0.1f;						//衝突エフェクト再生速度
	const float CameraShakingPower = 1.0f;						//カメラ振動強さ
	const int CameraShakingChangeDirFrame = 5;					//カメラ振動方向変更フレーム数
	const int CameraShakingPlayFrame = 50;						//カメラ振動再生フレーム
	const int PadVibPower = 150;								//パッド振動強さ
	const int PadVibPlayFrame = 1000;							//パッド振動再生フレーム

	//オブジェクトに衝突した際の処理
	void OnHitObject(CollisionData* objectData);
	//あたり判定情報更新
	void UpdateCollisionData();

	//他クラス
	CollisionManager* collisionManager;
	CollisionData collisionData;
	Effect* effect;
	Camera* camera;
	SoundEffect* se;

	int modelHandle;				//モデルハンドル
	VECTOR position;				//ポジション
	float rotateY;					//岩のY座標回転
	bool collisionActive;			//当たり判定を付けるか
	bool end;						//エフェクトまで終了
	VECTOR collisionPosition;		//当たり判定ポジション
	VECTOR hitEffectPosition;		//衝突エフェクトポジション
	VECTOR warningEffectPosition;	//注意エフェクトポジション
};