#include"DxLib.h"
#include"CollisionManager.h"
#include"EnemyBase.h"

/// <summary>
/// �|���S����񏉊���
/// </summary>
void EnemyBase::InitializePolygonData()
{
	//���f���S�̂̎Q�Ɨp���b�V�����\�z
	MV1SetupReferenceMesh(modelHandle, -1, TRUE);
	//�Q�Ɨp���b�V�����̎擾
	polygonList = MV1GetReferenceMesh(modelHandle, -1, TRUE);
}

/// <summary>
/// �|���S���`��
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
/// �����蔻���񏉊���
/// </summary>
void EnemyBase::InitializeCollisionData()
{
	//�����蔻����ǉ�
	collisionManager = collisionManager->GetInstance();
	collisionData.HitProcess = std::bind(&EnemyBase::OnHitObject, this, std::placeholders::_1);
	UpdateCollisionData();
	collisionManager->AddCollisionData(&collisionData);
}

/// <summary>
/// �����蔻����X�V
/// </summary>
void EnemyBase::UpdateCollisionData()
{
	collisionData.tag = ObjectTag::Enemy;
	collisionData.startPosition = capsuleStart;
	collisionData.endPosition = capsuleEnd;
	collisionData.radius = capsuleRadius;
	collisionData.attackPower = 0;
	collisionData.isCollisionActive = true;

	//�|���S�����
	collisionData.meshData.polygonList = polygonList;
	collisionData.meshData.meshTag = ObjectTag::Enemy;
}

//�m�F�p
void EnemyBase::DrawCapsule()
{
	//�m�F�p
	DrawCapsule3D(capsuleStart, capsuleEnd, capsuleRadius, 8, GetColor(102, 0, 255), GetColor(102, 0, 255), false);
}

/// <summary>
/// �����蔻����j��
/// </summary>
void EnemyBase::DeleteCollisionData()
{
	collisionManager->RemoveCollisionData(&collisionData);
}