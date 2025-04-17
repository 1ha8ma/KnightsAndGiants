#pragma once

class CollisionManager;
struct CollisionData;

class StageObject
{
public:
	StageObject(int modelHandle, VECTOR addCapsuleStart, VECTOR addCapsuleEnd, float capsuleRadius, VECTOR drawPosition, VECTOR rotate, float scale);
	~StageObject();

	//描画
	void Draw();

private:
	//当たった時の処理
	void OnHitObject();
	//当たり判定情報セット
	void RegisterCollisionData();
	//ポリゴン情報初期化
	void InitializePolygonData();
	//ポリゴン描画(確認用)
	void DrawPolygon();

	//他クラス
	CollisionManager* collisionManager;
	CollisionData collisionData;

	int modelHandle;					//モデルハンドル
	float capsuleRadius;				//判定開始カプセル半径
	VECTOR capsuleStart;				//カプセル始点
	VECTOR capsuleEnd;					//カプセル終点
	VECTOR drawPosition;				//描画ポジション
	MV1_REF_POLYGONLIST polygonList;	//ポリゴン情報が入ったリスト

	//確認用
	bool objecthit;						//オブジェクトに当たった
};