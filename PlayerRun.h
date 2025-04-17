#pragma once
#include"PlayerStateProcessBase.h"

class PlayerRun :public PlayerStateProcessBase
{
public:
	PlayerRun(int modelHandle,VECTOR prevtargetLookDirection);
	~PlayerRun()override;

	//������
	void Initialize()override {};
	//�X�V
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)override;

private:
	const float PlayAnimationSpeed = 0.4f;	//�A�j���[�V�����Đ����x

	//�ړ�����
	void Move(UsePlayerData playerData,Camera camera);

	float Speed;      //�X�s�[�h
	bool moveflg;	//�ړ����Ă��邩
};