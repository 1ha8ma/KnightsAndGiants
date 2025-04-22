#pragma once
#include"PlayerStateProcessBase.h"
#include"Calculation.h"

class Calculation;

class PlayerClimb :public PlayerStateProcessBase
{
public:
	PlayerClimb(int modelHandle, VECTOR lookDir);
	~PlayerClimb()override;

	//������
	void Initialize()override;
	//�X�V
	bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)override;

	void Draw()override;

private:
	//����
	void Move(UsePlayerData playerData, Camera camera,CollisionData objectData);

	//���͂����moveVec�쐬
	void MoveVecForInput(UsePlayerData playerData, VECTOR normVec);

	//��]�s��X�V
	void UpdateRotateMatrix(VECTOR newLookDirection, VECTOR prevLookDirection,VECTOR normVec);

	//�߂��|���S���������A�������
	int NearPolygon(const CollisionData objectData);

	//���͂��ꂽ�����|���S���̕��ʂɉf��
	VECTOR ProjectOnPlane(VECTOR inputDir, VECTOR polygonNormal);

	//�I�u�W�F�N�g�̓����ɒǏ]
	VECTOR FollowObjectMove(int nearPolygonNumber, CollisionData objectData);

	//���N���X
	Calculation* calculation;

	//�ϐ�
	float Speed;					//�ړ����x
	bool stopAnimation;				//�A�j���[�V������~�t���O
	VECTOR playerPosition;			//�v���C���[�|�W�V����
	VECTOR nearPolygonCenter;		//��ԋ߂��|���S���̒��_
	int prevClimbPolygonNumber;		//�O�̃t���[���̓o���Ă����|���S���̔ԍ�
	VECTOR prevClimbPolygonCenter;	//�O�̃t���[���̓o���Ă����|���S���̒��_
};