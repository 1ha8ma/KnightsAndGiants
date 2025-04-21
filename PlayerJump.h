#pragma once
#include"PlayerStateProcessBase.h"

class PlayerJump :public PlayerStateProcessBase
{
public:
	PlayerJump(int modelHandle,VECTOR prevmoveVec,VECTOR prevLookDir);
	~PlayerJump()override;

	//������
	void Initialize()override;
	//�X�V
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)override;

private:
	const float PlayAnimationSpeed = 1.3f;	//�A�j���[�V�����Đ����x

	//�ړ�����
	void Move(UsePlayerData playerData, Camera camera);

	VECTOR moveVecH;				//���������x�N�g��
	VECTOR prevMoveVecH;			//�ȑO�̐���moveVec
	float FirstJumpPower;			//�ŏ��̃W�����v��
	float MoveSpeed;				//���s�ړ����x
	float jumpPower;				//�W�����v��
};