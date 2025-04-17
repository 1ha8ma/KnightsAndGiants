#pragma once
#include<vector>
#include"EnemyBase.h"

class WeakPoint;
class ArmEnemyMoveBase;
class Calculation;

class ArmEnemy:public EnemyBase
{
public:
	ArmEnemy();
	~ArmEnemy()override;

	//������
	void Initialize()override;
	//�X�V
	bool Update(VECTOR playerPos, Camera* camera)override;

	//�|���ꂽ��̏�����
	void InitializeFallDown()override;
	//�|���ꂽ��̍X�V
	bool UpdateFallDown(Camera* camera)override;

	//�`��
	void Draw()override;

private:
	//�p�[�c�̖��O
	enum class PartsName :int
	{
		Upperarm,	//��r
		Forearm,	//�O�r
		Hand,		//��
		WeakPoint,	//��_
	};

	//�����̎��
	enum class MoveKind :int
	{
		Idle,		//�ҋ@
		DropRock,	//�◎�Ƃ�
		Swing,		//�U���
		FallDown,	//�|���
		HandUp,		//����グ��
	};

	const float ModelScale = 40.0f;							//���f���̕`��T�C�Y
	const float WeakPointRadius = 320.0f;					//��_�����a
	const float CapsuleRadius = 1100.0f;					//�����蔻��J�n�J�v�Z���T�C�Y
	const VECTOR ModelRotate = VGet(0, DX_PI_F / 2, 0);		//���f����]
	const int PartsNum = 3;									//�p�[�c�̐�

	//�v���C���[���r�̂ǂ̕��ʂɋ߂�������
	void CheckPlayerNearParts(VECTOR playerPos);

	//����ύX
	void ChangeMove(VECTOR playerPos);

	//�I�u�W�F�N�g�ɏՓ˂������̏���
	void OnHitObject(CollisionData* hitObjectData)override;

	//���N���X
	ArmEnemyMoveBase* move;
	Calculation* calclation;
	WeakPoint* weakPoint;

	//�X�e�[�^�X
	int MaxHP;								//�ő�HP

	//�U���֌W
	int PlayerRideMoveStartFlame;			//�v���C���[������Ă��鎞�̓������n�߂�t���[��
	int AttackCoolTime;						//�U���N�[���^�C��
	float DropRockStartPlayerHeight;		//�◎�Ƃ��U��������v���C���[�̍���
	int playerRidePlace;					//�v���C���[������Ă���ꏊ
	bool moveChangeflg;						//��Ԃ���̓����ύX�w���t���O
	MoveKind nowMoveKind;					//���݂̓����̎��
	int playerRideFlame;					//�v���C���[������Ă��鎞��
	bool isPlayerRide;						//�v���C���[������Ă���t���O
	bool playerRideMoveStartflg;			//�v���C���[������Ă��鎞�̓����̊J�n�t���O
	int attackCoolTimeFlame;				//�U���N�[���^�C���t���[��
	bool isAttackCoolTime;					//�U���N�[���^�C���t���O
};