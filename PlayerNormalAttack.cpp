#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"CollisionManager.h"
#include"SoundEffect.h"
#include"Camera.h"
#include"PlayerNormalAttack.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">�ʏ�U��</param>
PlayerNormalAttack::PlayerNormalAttack(int modelHandle, VECTOR prevtargetLookDirection) :PlayerStateProcessBase(modelHandle)
{
	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber::NormalAttack);
	//�A�j���[�V�����̑��Đ����Ԃ����
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);

	//�ړ��x�N�g��������
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	//�ڕW�̕�����O�̕����ɂ���
	newLookDirection = prevtargetLookDirection;

	//�C���X�^���X��
	se = new SoundEffect();

	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//private�ϐ�������
	AttackPower = jsonData["NormalAttackPower"];
	AttackCapsuleRadius = jsonData["NormalAttackCapsuleRadius"];
	stateChangeflg = false;
	animEndflg = false;
	attack = true;
	attackHit = false;

	//�����蔻����
	collisionManager = collisionManager->GetInstance();
	attackCollisionData.HitProcess = std::bind(&PlayerNormalAttack::OnHitObject, this, std::placeholders::_1);
	collisionManager->AddCollisionData(&attackCollisionData);

	//SE�Đ�
	se->PlaySE(SoundEffect::SEKind::NormalAttack);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerNormalAttack::~PlayerNormalAttack()
{
	//�U�������蔻���菜��
	collisionManager->RemoveCollisionData(&attackCollisionData);
	//�A�j���[�V�����f�^�b�`
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// ������
/// </summary>
void PlayerNormalAttack::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="playerData">�v���C���[�f�[�^</param>
/// <param name="camera">�J����</param>
/// <param name="objectCollision">�Փ˂����I�u�W�F�N�g</param>
/// <returns>�I��</returns>
bool PlayerNormalAttack::Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision)
{
	if (!animEndflg)//�Đ�
	{
		animEndflg = PlayAnimationOnce(PlayAnimationSpeed);
	}
	else//�t�Đ�
	{
		attack = false;
		stateChangeflg = ReversePlaybackAnimation(ReversePlaySpeed, ReversePoint);
	}

	if (attackHit)
	{
		attack = false;
	}

	//�ړ��͂��Ȃ�
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	//�U�������蔻��X�V
	VECTOR capsuleCenter = MV1GetFramePosition(modelHandle, RightHandFrameNumber);
	attackCapsuleStart = VAdd(capsuleCenter, VGet(-sin(playerData.angle + DX_PI_F) * 70, -20.0f, -cos(playerData.angle + DX_PI_F) * 70));
	attackCapsuleEnd = VAdd(capsuleCenter, VGet(sin(playerData.angle + DX_PI_F), 0.0f, cos(playerData.angle + DX_PI_F)));

	UpdateCollisionData();

	return stateChangeflg;
}

/// <summary>
/// �`��(�m�F�p)
/// </summary>
void PlayerNormalAttack::Draw()
{
	DrawCapsule3D(attackCapsuleStart, attackCapsuleEnd, AttackCapsuleRadius, 8, GetColor(255, 105, 180), GetColor(255, 105, 180), false);
	DrawSphere3D(attackCapsuleStart, AttackCapsuleRadius, 8, GetColor(145, 154, 12), GetColor(145, 154, 12), false);
}

/// <summary>
/// �Փˌ㏈��
/// </summary>
/// <param name="objectData">�Փ˂����I�u�W�F�N�g</param>
void PlayerNormalAttack::OnHitObject(CollisionData* objectData)
{
	if (objectData->tag == ObjectTag::Enemy || objectData->tag == ObjectTag::WeakPoint)
	{
		attackHit = true;
	}
}

/// <summary>
/// �����蔻����X�V
/// </summary>
void PlayerNormalAttack::UpdateCollisionData()
{
	attackCollisionData.tag = ObjectTag::Attack_P;
	attackCollisionData.startPosition = attackCapsuleStart;
	attackCollisionData.endPosition = attackCapsuleEnd;
	attackCollisionData.radius = AttackCapsuleRadius;
	attackCollisionData.attackPower = AttackPower;
	attackCollisionData.isCollisionActive = attack;
}