#pragma once

class CollisionManager;
struct CollisionData;

class StageObject
{
public:
	StageObject(int modelHandle, VECTOR addCapsuleStart, VECTOR addCapsuleEnd, float capsuleRadius, VECTOR drawPosition, VECTOR rotate, float scale);
	~StageObject();

	//�`��
	void Draw();

private:
	//�����������̏���
	void OnHitObject();
	//�����蔻����Z�b�g
	void RegisterCollisionData();
	//�|���S����񏉊���
	void InitializePolygonData();
	//�|���S���`��(�m�F�p)
	void DrawPolygon();

	//���N���X
	CollisionManager* collisionManager;
	CollisionData collisionData;

	int modelHandle;					//���f���n���h��
	float capsuleRadius;				//����J�n�J�v�Z�����a
	VECTOR capsuleStart;				//�J�v�Z���n�_
	VECTOR capsuleEnd;					//�J�v�Z���I�_
	VECTOR drawPosition;				//�`��|�W�V����
	MV1_REF_POLYGONLIST polygonList;	//�|���S����񂪓��������X�g

	//�m�F�p
	bool objecthit;						//�I�u�W�F�N�g�ɓ�������
};