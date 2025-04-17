#pragma once
#include"PlayerStateProcessBase.h"

class CollisionManager;
struct CollisionData;

/// <summary>
/// �˂��h���U��
/// </summary>
class PlayerPiercing :public PlayerStateProcessBase
{
public:
	PlayerPiercing(int modelHandle, VECTOR prevtargetLookDirection);
	~PlayerPiercing();

	void Initialize()override {};
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision) override;
	void Draw()override;

private:
	const float CameraZoomSpeed = 1.4f;		//�J�����Y�[�����x

	//�����蔻����X�V
	void UpdateCollisionData();
	//�˂��h���U��
	void PiercingAttack(int inputstate);
	//�Փˌ㏈��
	void OnHitObject(CollisionData* objectData);

	//���N���X
	CollisionManager* collisionManager;
	CollisionData attackCollisionData;	//�U�������蔻����

	int LowestAttackPower;			//�Œ�U����
	float AttackCapsuleRadius;		//�U���J�v�Z�����a
	float MaxSwingUp;				//�ő�U��グ�n�_
	float SwingUpSpeed;				//�U��グ���x
	float SwingDownSpeed;			//�U�艺�낵���x
	float ChargeMagnification;		//�`���[�W�{��
	bool canInputX;					//X�{�^���A�����͖h�~�t���O
	bool changeState;				//�X�e�[�g�ύX
	VECTOR attackCapsuleStart;		//�U���J�v�Z���n�_
	VECTOR attackCapsuleEnd;		//�U���J�v�Z���I�_
	int attackPower;				//�U����
	int attackChargeFlame;			//�U�����ߎ���
	bool attack;					//�U����
	bool attackEnd;					//�U���I��
	bool attackHit;					//�U������������
	VECTOR rotate;					//�r�̉�]
};