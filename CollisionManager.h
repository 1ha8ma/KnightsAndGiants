#pragma once
#include"CollisionData.h"
#include<vector>

class CollisionManager final
{
public:
	virtual ~CollisionManager();

	//������
	void Initialize();
	//�X�V
	void Update();

	//�C���X�^���X�Q�b�g
	static CollisionManager* GetInstance();

	//�����蔻����ǉ�
	static void AddCollisionData(CollisionData* data);

	//�����蔻����̎�菜��
	static void RemoveCollisionData(CollisionData* data);

	//��肽���I�u�W�F�N�g�̓����蔻��������
	static CollisionData GetCollisionData(CollisionData *data);

private:
	CollisionManager();

	//�J�v�Z���ǂ����̓����蔻��
	bool CapsuleWithCapsule(VECTOR capsuleStart1, VECTOR capsuleEnd1, float capsuleRadius1, VECTOR capsuleStart2, VECTOR capsuleEnd2, float capsuleRadius2);

	//�J�v�Z���ƃ��b�V���̂����蔻��
	bool CapsuleWithMesh(VECTOR capsuleStart, VECTOR capsuleEnd, float capsuleRadius, MV1_REF_POLYGONLIST meshData);

	static CollisionManager* collisionManager;				//�C���X�^���X
	static std::vector<CollisionData*> collisionDataList;	//�����蔻���񃊃X�g
};