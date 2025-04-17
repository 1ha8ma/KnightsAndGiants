#pragma once
#include"Camera.h"
#include"CollisionData.h"

class CollisionManager;

class EnemyBase
{
public:
	EnemyBase() {};
	virtual ~EnemyBase() {};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()abstract;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤーポジション</param>
	/// <param name="camera">カメラ</param>
	/// <returns>ゲームオーバーか</returns>
	virtual bool Update(VECTOR playerPos, Camera* camera)abstract;

	/// <summary>
	/// 倒された後の初期化
	/// </summary>
	virtual void InitializeFallDown()abstract;

	/// <summary>
	/// 倒された後の更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns>動きの終了</returns>
	virtual bool UpdateFallDown(Camera* camera)abstract;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()abstract;

	//GetSet
	int GetHP() { return HP; }
	VECTOR GetTargetCameraPosition() { return targetCameraPosition; }

protected:
	//ポリゴン情報初期化
	void InitializePolygonData();
	//ポリゴン表示
	void DrawPolygon();
	//オブジェクトに衝突した時の処理
	virtual void OnHitObject(CollisionData* hitObjectData)abstract;
	//あたり判定情報初期化
	void InitializeCollisionData();
	//あたり判定更新
	void UpdateCollisionData();
	//あたり判定カプセル表示
	void DrawCapsule();
	//あたり判定破棄
	void DeleteCollisionData();

	//他クラス
	CollisionManager* collisionManager;
	CollisionData collisionData;

	//ステータス
	int HP;

	int modelHandle;						//モデルハンドル
	VECTOR position;						//描画ポジション
	VECTOR targetCameraPosition;			//ターゲットカメラ用ポジション
	MV1_REF_POLYGONLIST polygonList;		//ポリゴンの情報が入ったリスト
	VECTOR capsuleStart;				//カプセル始点
	VECTOR capsuleEnd;					//カプセル終点
	float capsuleRadius;				//カプセル半径
};