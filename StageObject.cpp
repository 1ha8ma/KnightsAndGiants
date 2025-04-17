#include"DxLib.h"
#include"CollisionData.h"
#include"CollisionManager.h"
#include"StageObject.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
StageObject::StageObject(int modelHandle, VECTOR addCapsuleStart, VECTOR addCapsuleEnd, float capsuleRadius, VECTOR drawPosition, VECTOR rotate, float scale)
{
	this->modelHandle = modelHandle;

	this->drawPosition = drawPosition;
	this->capsuleStart = VAdd(drawPosition, addCapsuleStart);
	this->capsuleEnd = VAdd(drawPosition, addCapsuleEnd);
	this->capsuleRadius = capsuleRadius;

	//�|�W�V���������f
	MV1SetScale(modelHandle, VGet(scale, scale, scale));
	MV1SetRotationXYZ(modelHandle, VGet(rotate.x, rotate.y, rotate.z));
	MV1SetPosition(modelHandle, drawPosition);

	objecthit = false;
	InitializePolygonData();

	//�����蔻����
	collisionManager = collisionManager->GetInstance();
	RegisterCollisionData();
	collisionData.HitProcess = std::bind(&StageObject::OnHitObject, this);
	collisionManager->AddCollisionData(&collisionData);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StageObject::~StageObject()
{
	collisionManager->RemoveCollisionData(&collisionData);
}

/// <summary>
/// �`��
/// </summary>
void StageObject::Draw()
{
	MV1DrawModel(modelHandle);

	//�m�F�p
	/*if (objecthit)
	{
		DrawPolygon();
	}
	DrawCapsule3D(capsuleStart, capsuleEnd, capsuleRadius, 8, GetColor(0, 255, 255), GetColor(0, 255, 255), FALSE);
	objecthit = false;*/
}

/// <summary>
/// �I�u�W�F�N�g�Փˎ�����
/// </summary>
void StageObject::OnHitObject()
{
	objecthit = true;
}

/// <summary>
/// �����蔻����Z�b�g
/// </summary>
void StageObject::RegisterCollisionData()
{
	//�J�v�Z��
	collisionData.tag = ObjectTag::StageObject;
	collisionData.startPosition = capsuleStart;
	collisionData.endPosition =	capsuleEnd;
	collisionData.position = VAdd(capsuleStart, capsuleEnd);
	collisionData.position = VScale(collisionData.position, 0.5f);	//�J�v�Z���̒��_���|�W�V�����ɂ��Ă���
	collisionData.radius = capsuleRadius;
	collisionData.isCollisionActive = true;

	//���b�V�����
	collisionData.meshData.polygonList = polygonList;
	collisionData.meshData.meshTag = ObjectTag::StageObject;
}

/// <summary>
/// �|���S����񏉊���
/// </summary>
void StageObject::InitializePolygonData()
{
	//���f���S�̂̎Q�Ɨp���b�V�����\�z
	MV1SetupReferenceMesh(modelHandle, -1, TRUE);
	//�Q�Ɨp���b�V�����̎擾
	polygonList = MV1GetReferenceMesh(modelHandle, -1, TRUE);
}

/// <summary>
/// �|���S���`��
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