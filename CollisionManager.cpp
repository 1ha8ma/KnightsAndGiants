#include"DxLib.h"
#include"CollisionManager.h"

//������
CollisionManager* CollisionManager::collisionManager = nullptr;
std::vector<CollisionData*> CollisionManager::collisionDataList;

/// <summary>
/// �R���X�g���N�^
/// </summary>
CollisionManager::CollisionManager()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CollisionManager::~CollisionManager()
{
	collisionDataList.clear();
}

/// <summary>
/// ������
/// </summary>
void CollisionManager::Initialize()
{

}

/// <summary>
/// �C���X�^���X�Q�b�g
/// </summary>
/// <returns>�C���X�^���X</returns>
CollisionManager* CollisionManager::GetInstance()
{
	if (collisionManager == nullptr)
	{
		collisionManager = new CollisionManager();
	}

	return collisionManager;
}

/// <summary>
/// �����蔻����ǉ�
/// </summary>
/// <param name="data">�f�[�^</param>
void CollisionManager::AddCollisionData(CollisionData* data)
{
	collisionDataList.push_back(data);
}

/// <summary>
/// �����蔻����폜
/// </summary>
/// <param name="data">�f�[�^</param>
void CollisionManager::RemoveCollisionData(CollisionData* data)
{
	int num = 0;
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		if (collisionDataList[i] == data)
		{
			break;
		}
		else
		{
			num++;
		}
	}

	//�폜
	collisionDataList.erase(collisionDataList.begin() + num);
}

/// <summary>
/// �S�Ă̓����蔻�菈��
/// </summary>
void CollisionManager::Update()
{
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		for (int j = 0; j < collisionDataList.size(); j++)
		{
			//�������̂�������X�L�b�v
			if (i == j)
			{
				continue;
			}

			//�ǂ̃^�C�v�̓����蔻����s�������ׂ�
			CollisionData* data1 = collisionDataList[i];
			CollisionData* data2 = collisionDataList[j];

			//�����蔻����s���ė~�������m�F
			if (data1->isCollisionActive && data2->isCollisionActive)
			{
				//�J�v�Z���ǂ���
				if (
					/*�v���C���[�ƃX�e�[�W�̃I�u�W�F�N�g*/
					data1->tag == ObjectTag::PlayerWholeBody && data2->tag == ObjectTag::StageObject ||
					data1->tag == ObjectTag::PlayerFoot && data2->tag == ObjectTag::StageObject ||
					/*�v���C���[�ƓG*/
					data1->tag == ObjectTag::PlayerWholeBody && data2->tag == ObjectTag::Enemy ||
					data1->tag == ObjectTag::PlayerFoot && data2->tag == ObjectTag::Enemy ||
					/*�G�̍U���ƃv���C���[*/
					data1->tag == ObjectTag::EnemyAttack && data2->tag == ObjectTag::PlayerWholeBody ||
					data1->tag == ObjectTag::EnemyAttack && data2->tag == ObjectTag::PlayerFoot ||
					/*�v���C���[�̍U���ƓG*/
					data1->tag == ObjectTag::Attack_P && data2->tag == ObjectTag::Enemy ||
					data1->tag == ObjectTag::Attack_P && data2->tag == ObjectTag::WeakPoint
					)
				{
					bool hit = CapsuleWithCapsule(data1->startPosition, data1->endPosition, data1->radius, data2->startPosition, data2->endPosition, data2->radius);

					if (hit)
					{
						data1->HitProcess(data2);
						data2->HitProcess(data1);
					}
				}

				//�J�v�Z���ƃ��b�V��
				if (
					/*�G�̍U���ƃX�e�[�W�I�u�W�F�N�g*/
					data1->tag == ObjectTag::EnemyAttack && data2->tag == ObjectTag::StageObject
					)
				{
					//�J�v�Z���ł̓����蔻������Ă��烁�b�V���̔�������邩���f
					bool isHitJudge = CapsuleWithCapsule(data1->startPosition, data1->endPosition, data1->radius, data2->startPosition, data2->endPosition, data2->radius);
					if (isHitJudge)
					{
						bool hit = CapsuleWithMesh(data1->startPosition, data1->endPosition, data1->radius, data2->meshData.polygonList);

						if (hit)
						{
							data1->HitProcess(data2);
							data2->HitProcess(data1);
						}
					}
				}
			}

		}
	}

	//�v���C���[�ƕǂƂ̓����蔻��
	CollisionData wallData;		//�ǂ̏��
	bool wallHit = false;		//�ǂɓ�������
	//�ǂ̏���T���Ď��
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		ObjectTag tag = collisionDataList[i]->tag;
		if (tag == ObjectTag::Stage)
		{
			wallData = *collisionDataList[i];
			break;
		}
	}
	//�v���C���[�̏���T���Ď��A�����߂��x�N�g�����v�Z
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		CollisionData data = *collisionDataList[i];

		if (data.tag == ObjectTag::PlayerWholeBody)
		{
			VECTOR correction = VGet(0, 0, 0);
			if (data.position.x > wallData.stageRight)
			{
				float dif = data.position.x - wallData.stageRight;
				correction = VAdd(correction, VGet(-dif, 0, 0));
				wallHit = true;
			}
			if (data.position.x < wallData.stageLeft)
			{
				float dif = wallData.stageLeft - data.position.x;
				correction = VAdd(correction, VGet(dif, 0, 0));
				wallHit = true;
			}
			if (data.position.z > wallData.stageFront)
			{
				float dif = data.position.z - wallData.stageFront;
				correction = VAdd(correction, VGet(0, 0, -dif));
				wallHit = true;
			}
			if (data.position.z < wallData.stageBack)
			{
				float dif = wallData.stageBack - data.position.z;
				correction = VAdd(correction, VGet(0, 0, dif));
				wallHit = true;
			}

			//�������Ă����ꍇ
			if (wallHit)
			{
				data.WallHitProcess(correction);
			}
		}
	}
}

/// <summary>
/// �J�v�Z���ǂ����̓����蔻��
/// </summary>
/// <param name="capsuleStart1">�J�v�Z��1�n�_</param>
/// <param name="capsuleEnd1">�J�v�Z��1�I�_</param>
/// <param name="capsuleRadius1">�J�v�Z��1���a</param>
/// <param name="capsuleStart2">�J�v�Z��2�n�_</param>
/// <param name="capsuleEnd2">�J�v�Z��2�I�_</param>
/// <param name="capsuleRadius2">�J�v�Z��2���a</param>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::CapsuleWithCapsule(VECTOR capsuleStart1, VECTOR capsuleEnd1, float capsuleRadius1, VECTOR capsuleStart2, VECTOR capsuleEnd2, float capsuleRadius2)
{
	bool hit = false;

	//�J�v�Z���Ԃ̍ŒZ����
	float length = Segment_Segment_MinLength(capsuleStart1, capsuleEnd1, capsuleStart2, capsuleEnd2);

	//����
	if (length < capsuleRadius1 + capsuleRadius2)
	{
		hit = true;
	}

	return hit;
}

/// <summary>
/// �J�v�Z���ƃ��b�V���̂����蔻��
/// </summary>
/// <param name="capsuleStart">�J�v�Z���n�_</param>
/// <param name="capsuleEnd">�J�v�Z���I�_</param>
/// <param name="capsuleRadius">�J�v�Z�����a</param>
/// <param name="meshData">���b�V���̏��</param>
/// <returns>����������</returns>
bool CollisionManager::CapsuleWithMesh(VECTOR capsuleStart, VECTOR capsuleEnd, float capsuleRadius, MV1_REF_POLYGONLIST meshData)
{
	bool result = false;

	for (int i = 0; i < meshData.PolygonNum; i++)
	{
		//�O�p�`���_
		VECTOR vertex0 = meshData.Vertexs[meshData.Polygons[i].VIndex[0]].Position;
		VECTOR vertex1 = meshData.Vertexs[meshData.Polygons[i].VIndex[1]].Position;
		VECTOR vertex2 = meshData.Vertexs[meshData.Polygons[i].VIndex[2]].Position;

		//�J�v�Z���ƎO�p�`�̓����蔻��
		result = HitCheck_Capsule_Triangle(capsuleStart, capsuleEnd, capsuleRadius, vertex0, vertex1, vertex2);

		if (result)
		{
			break;
		}
	}

	return result;
}

/// <summary>
/// �w��̃I�u�W�F�N�g�̓����蔻��������(���������������łȂ����̌���ŐV�̏�񂪗~�������Ɏg�p)
/// </summary>
/// <param name="data">������肽���I�u�W�F�N�g�̃|�C���^</param>
/// <returns>�f�[�^</returns>
CollisionData CollisionManager::GetCollisionData(CollisionData* data)
{
	int i;
	for (i = 0; i < collisionDataList.size(); i++)
	{
		if (collisionDataList[i] == data)
		{
			break;
		}
	}

	return *collisionDataList[i];
}