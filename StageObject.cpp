#include"DxLib.h"
#include"CollisionData.h"
#include"CollisionManager.h"
#include"StageObject.h"

/// <summary>
/// コンストラクタ
/// </summary>
StageObject::StageObject(int modelHandle, VECTOR addCapsuleStart, VECTOR addCapsuleEnd, float capsuleRadius, VECTOR drawPosition, VECTOR rotate, float scale)
{
	this->modelHandle = modelHandle;

	this->drawPosition = drawPosition;
	this->capsuleStart = VAdd(drawPosition, addCapsuleStart);
	this->capsuleEnd = VAdd(drawPosition, addCapsuleEnd);
	this->capsuleRadius = capsuleRadius;

	//ポジション等反映
	MV1SetScale(modelHandle, VGet(scale, scale, scale));
	MV1SetRotationXYZ(modelHandle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetPosition(modelHandle, drawPosition);

	objecthit = false;
	InitializePolygonData();

	//当たり判定情報
	collisionManager = collisionManager->GetInstance();
	RegisterCollisionData();
	collisionData.HitProcess = std::bind(&StageObject::OnHitObject, this);
	collisionManager->AddCollisionData(&collisionData);
}

/// <summary>
/// デストラクタ
/// </summary>
StageObject::~StageObject()
{
	collisionManager->RemoveCollisionData(&collisionData);
}

/// <summary>
/// 描画
/// </summary>
void StageObject::Draw()
{
	MV1DrawModel(modelHandle);

	//確認用
	/*if (objecthit)
	{
		DrawPolygon();
	}
	DrawCapsule3D(capsuleStart, capsuleEnd, capsuleRadius, 8, GetColor(0, 255, 255), GetColor(0, 255, 255), FALSE);
	objecthit = false;*/
}

/// <summary>
/// オブジェクト衝突時処理
/// </summary>
void StageObject::OnHitObject()
{
	objecthit = true;
}

/// <summary>
/// 当たり判定情報セット
/// </summary>
void StageObject::RegisterCollisionData()
{
	//カプセル
	collisionData.tag = ObjectTag::StageObject;
	collisionData.startPosition = capsuleStart;
	collisionData.endPosition =	capsuleEnd;
	collisionData.position = VAdd(capsuleStart, capsuleEnd);
	collisionData.position = VScale(collisionData.position, 0.5f);	//カプセルの中点をポジションにしている
	collisionData.radius = capsuleRadius;
	collisionData.isCollisionActive = true;

	//メッシュ情報
	collisionData.meshData.polygonList = polygonList;
	collisionData.meshData.meshTag = ObjectTag::StageObject;
}

/// <summary>
/// ポリゴン情報初期化
/// </summary>
void StageObject::InitializePolygonData()
{
	//モデル全体の参照用メッシュを構築
	MV1SetupReferenceMesh(modelHandle, -1, TRUE);
	//参照用メッシュ情報の取得
	polygonList = MV1GetReferenceMesh(modelHandle, -1, TRUE);
}

/// <summary>
/// ポリゴン描画
/// </summary>
void StageObject::DrawPolygon()
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