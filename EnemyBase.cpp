#include"DxLib.h"
#include"CollisionManager.h"
#include"EnemyBase.h"

/// <summary>
/// ポリゴン情報初期化
/// </summary>
void EnemyBase::InitializePolygonData()
{
	//モデル全体の参照用メッシュを構築
	MV1SetupReferenceMesh(modelHandle, -1, TRUE);
	//参照用メッシュ情報の取得
	polygonList = MV1GetReferenceMesh(modelHandle, -1, TRUE);
}

/// <summary>
/// ポリゴン描画
/// </summary>
void EnemyBase::DrawPolygon()
{
	for (int i = 0; i < polygonList.PolygonNum; i++)
	{
		DrawLine3D(
			polygonList.Vertexs[polygonList.Polygons[i].VIndex[0]].Position,
			polygonList.Vertexs[polygonList.Polygons[i].VIndex[1]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			polygonList.Vertexs[polygonList.Polygons[i].VIndex[1]].Position,
			polygonList.Vertexs[polygonList.Polygons[i].VIndex[2]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			polygonList.Vertexs[polygonList.Polygons[i].VIndex[2]].Position,
			polygonList.Vertexs[polygonList.Polygons[i].VIndex[0]].Position,
			GetColor(255, 255, 0));
	}
}

/// <summary>
/// あたり判定情報初期化
/// </summary>
void EnemyBase::InitializeCollisionData()
{
	//当たり判定情報追加
	collisionManager = collisionManager->GetInstance();
	collisionData.HitProcess = std::bind(&EnemyBase::OnHitObject, this, std::placeholders::_1);
	UpdateCollisionData();
	collisionManager->AddCollisionData(&collisionData);
}

/// <summary>
/// あたり判定情報更新
/// </summary>
void EnemyBase::UpdateCollisionData()
{
	collisionData.tag = ObjectTag::Enemy;
	collisionData.startPosition = capsuleStart;
	collisionData.endPosition = capsuleEnd;
	collisionData.radius = capsuleRadius;
	collisionData.attackPower = 0;
	collisionData.isCollisionActive = true;

	//ポリゴン情報
	collisionData.meshData.polygonList = polygonList;
	collisionData.meshData.meshTag = ObjectTag::Enemy;
}

//確認用
void EnemyBase::DrawCapsule()
{
	//確認用
	DrawCapsule3D(capsuleStart, capsuleEnd, capsuleRadius, 8, GetColor(102, 0, 255), GetColor(102, 0, 255), false);
}

/// <summary>
/// あたり判定情報破棄
/// </summary>
void EnemyBase::DeleteCollisionData()
{
	collisionManager->RemoveCollisionData(&collisionData);
}