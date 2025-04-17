#pragma once
#include"CollisionData.h"

class CollisionManager;
class Effect;
class Camera;
class SoundEffect;

class ArmEnemyAttackRock
{
public:
	ArmEnemyAttackRock(VECTOR playerPosition,Camera* camera);
	~ArmEnemyAttackRock();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�I��������
	bool GetEnd() { return end; }

private:
	const int AttackPower = 20;									//�U����
	const float ModelScale = 100.0f;							//���f���X�P�[��
	const float FallSpeed = 35.0f;								//�����X�s�[�h
	const float CapsuleRadius = 300.0f;							//�J�v�Z�����a
	const float AddCollisionPosY = 170.0f;						//�|�W�V�������炠���蔻��̃|�W�V���������ۂ�Y���ǉ���
	const float ModelRotateSpeedY = 0.04f;						//���f����Y����]���x
	const VECTOR WarningEffectScale = VGet(80, 20, 80);			//���ӃG�t�F�N�g�X�P�[��
	const VECTOR WarningEffectRotate = VGet(0, 0, 0);			//���ӃG�t�F�N�g��]
	const float WarningEffectPlaySpeed = 1.0f;					//���ӃG�t�F�N�g�Đ����x
	const float AddHitEffectPositionY = 70.0f;					//�Փ˃G�t�F�N�g�|�W�V����Y���ǉ���
	const VECTOR HitEffectScale = VGet(50, 50, 50);				//�Փ˃G�t�F�N�g�X�P�[��
	const VECTOR HitEffectRotate = VGet(DX_PI_F / 2, 0, 0);		//�Փ˃G�t�F�N�g��]
	const float HitEffectPlaySpeed = 0.1f;						//�Փ˃G�t�F�N�g�Đ����x
	const float CameraShakingPower = 1.0f;						//�J�����U������
	const int CameraShakingChangeDirFrame = 5;					//�J�����U�������ύX�t���[����
	const int CameraShakingPlayFrame = 50;						//�J�����U���Đ��t���[��
	const int PadVibPower = 150;								//�p�b�h�U������
	const int PadVibPlayFrame = 1000;							//�p�b�h�U���Đ��t���[��

	//�I�u�W�F�N�g�ɏՓ˂����ۂ̏���
	void OnHitObject(CollisionData* objectData);
	//�����蔻����X�V
	void UpdateCollisionData();

	//���N���X
	CollisionManager* collisionManager;
	CollisionData collisionData;
	Effect* effect;
	Camera* camera;
	SoundEffect* se;

	int modelHandle;				//���f���n���h��
	VECTOR position;				//�|�W�V����
	float rotateY;					//���Y���W��]
	bool collisionActive;			//�����蔻���t���邩
	bool end;						//�G�t�F�N�g�܂ŏI��
	VECTOR collisionPosition;		//�����蔻��|�W�V����
	VECTOR hitEffectPosition;		//�Փ˃G�t�F�N�g�|�W�V����
	VECTOR warningEffectPosition;	//���ӃG�t�F�N�g�|�W�V����
};