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
	/// ������
	/// </summary>
	virtual void Initialize()abstract;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�|�W�V����</param>
	/// <param name="camera">�J����</param>
	/// <returns>�Q�[���I�[�o�[��</returns>
	virtual bool Update(VECTOR playerPos, Camera* camera)abstract;

	/// <summary>
	/// �|���ꂽ��̏�����
	/// </summary>
	virtual void InitializeFallDown()abstract;

	/// <summary>
	/// �|���ꂽ��̍X�V
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <returns>�����̏I��</returns>
	virtual bool UpdateFallDown(Camera* camera)abstract;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw()abstract;

	//GetSet
	int GetHP() { return HP; }
	VECTOR GetTargetCameraPosition() { return targetCameraPosition; }

protected:
	//�|���S����񏉊���
	void InitializePolygonData();
	//�|���S���\��
	void DrawPolygon();
	//�I�u�W�F�N�g�ɏՓ˂������̏���
	virtual void OnHitObject(CollisionData* hitObjectData)abstract;
	//�����蔻���񏉊���
	void InitializeCollisionData();
	//�����蔻��X�V
	void UpdateCollisionData();
	//�����蔻��J�v�Z���\��
	void DrawCapsule();
	//�����蔻��j��
	void DeleteCollisionData();

	//���N���X
	CollisionManager* collisionManager;
	CollisionData collisionData;

	//�X�e�[�^�X
	int HP;

	int modelHandle;						//���f���n���h��
	VECTOR position;						//�`��|�W�V����
	VECTOR targetCameraPosition;			//�^�[�Q�b�g�J�����p�|�W�V����
	MV1_REF_POLYGONLIST polygonList;		//�|���S���̏�񂪓��������X�g
	VECTOR capsuleStart;				//�J�v�Z���n�_
	VECTOR capsuleEnd;					//�J�v�Z���I�_
	float capsuleRadius;				//�J�v�Z�����a
};