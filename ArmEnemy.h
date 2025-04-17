#pragma once
#include<vector>
#include"EnemyBase.h"

class WeakPoint;
class ArmEnemyMoveBase;
class Calculation;

class ArmEnemy:public EnemyBase
{
public:
	ArmEnemy();
	~ArmEnemy()override;

	//初期化
	void Initialize()override;
	//更新
	bool Update(VECTOR playerPos, Camera* camera)override;

	//倒された後の初期化
	void InitializeFallDown()override;
	//倒された後の更新
	bool UpdateFallDown(Camera* camera)override;

	//描画
	void Draw()override;

private:
	//パーツの名前
	enum class PartsName :int
	{
		Upperarm,	//上腕
		Forearm,	//前腕
		Hand,		//手
		WeakPoint,	//弱点
	};

	//動きの種類
	enum class MoveKind :int
	{
		Idle,		//待機
		DropRock,	//岩落とし
		Swing,		//振り回し
		FallDown,	//倒れる
		HandUp,		//手を上げる
	};

	const float ModelScale = 40.0f;							//モデルの描画サイズ
	const float WeakPointRadius = 320.0f;					//弱点球半径
	const float CapsuleRadius = 1100.0f;					//あたり判定開始カプセルサイズ
	const VECTOR ModelRotate = VGet(0, DX_PI_F / 2, 0);		//モデル回転
	const int PartsNum = 3;									//パーツの数

	//プレイヤーが腕のどの部位に近いか判定
	void CheckPlayerNearParts(VECTOR playerPos);

	//動作変更
	void ChangeMove(VECTOR playerPos);

	//オブジェクトに衝突した時の処理
	void OnHitObject(CollisionData* hitObjectData)override;

	//他クラス
	ArmEnemyMoveBase* move;
	Calculation* calclation;
	WeakPoint* weakPoint;

	//ステータス
	int MaxHP;								//最大HP

	//攻撃関係
	int PlayerRideMoveStartFlame;			//プレイヤーが乗っている時の動きを始めるフレーム
	int AttackCoolTime;						//攻撃クールタイム
	float DropRockStartPlayerHeight;		//岩落とし攻撃をするプレイヤーの高さ
	int playerRidePlace;					//プレイヤーが乗っている場所
	bool moveChangeflg;						//状態からの動き変更指示フラグ
	MoveKind nowMoveKind;					//現在の動きの種類
	int playerRideFlame;					//プレイヤーが乗っている時間
	bool isPlayerRide;						//プレイヤーが乗っているフラグ
	bool playerRideMoveStartflg;			//プレイヤーが乗っている時の動きの開始フラグ
	int attackCoolTimeFlame;				//攻撃クールタイムフレーム
	bool isAttackCoolTime;					//攻撃クールタイムフラグ
};