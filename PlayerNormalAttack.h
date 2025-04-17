#pragma once
#include"PlayerStateProcessBase.h"

struct CollisionData;
class CollisionManager;
class SoundEffect;

class PlayerNormalAttack :public PlayerStateProcessBase
{
public:
	PlayerNormalAttack(int modelHandle, VECTOR prevtargetLookDirection);
	~PlayerNormalAttack()override;

	//������
	void Initialize()override;
	//�X�V
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)override;

	void Draw()override;

private:
	const float PlayAnimationSpeed = 0.7f;		//�Đ����x
	const float ReversePlaySpeed = 0.4f;		//�t�Đ����x
	const float ReversePoint = 0.6f;

	//�����蔻��X�V
	void UpdateCollisionData();

	//�Փˌ㏈��
	void OnHitObject(CollisionData* objectData);

	//���N���X
	CollisionManager* collisionManager;
	CollisionData attackCollisionData;
	SoundEffect* se;

	int AttackPower;				//�U����
	float AttackCapsuleRadius;		//�U�������蔻�蔼�a
	bool stateChangeflg;			//�X�e�[�g�ύX�t���O
	bool animEndflg;				//�A�j���[�V�����I��
	bool attack;					//�U����
	bool attackHit;					//�U������������
	VECTOR attackCapsuleStart;		//�U���J�v�Z���n�_
	VECTOR attackCapsuleEnd;		//�U���J�v�Z���I�_
};