#include"DxLib.h"
#include"CollisionManager.h"
#include"Camera.h"
#include"Effect.h"
#include"SoundEffect.h"
#include"Loader.h"
#include"ArmEnemyAttackRock.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="playerPosition">�v���C���[�|�W�V����</param>
ArmEnemyAttackRock::ArmEnemyAttackRock(VECTOR playerPosition, Camera* camera)
{
	//�C���X�^���X��
	Loader* loader = loader->GetInstance();
	effect = new Effect();
	se = new SoundEffect();
	this->camera = camera;

	//�ϐ�������
	modelHandle = loader->GetHandle(Loader::Kind::RockModel4);
	position = playerPosition;
	warningEffectPosition = position;
	position.y = 8000.0f;
	collisionPosition = VAdd(position, VGet(0.0f, AddCollisionPosY, 0.0f));
	rotateY = 0.0f;
	collisionActive = true;
	end = false;

	//�����蔻����
	collisionManager = collisionManager->GetInstance();
	collisionData.HitProcess = std::bind(&ArmEnemyAttackRock::OnHitObject, this, std::placeholders::_1);
	UpdateCollisionData();
	collisionManager->AddCollisionData(&collisionData);

	//�|�W�V�����Z�b�g
	MV1SetScale(modelHandle, VGet(ModelScale, ModelScale, ModelScale));
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, 0.0f, rotateY));
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArmEnemyAttackRock::~ArmEnemyAttackRock()
{
	collisionManager->RemoveCollisionData(&collisionData);

	delete effect;
}

/// <summary>
/// �X�V
/// </summary>
void ArmEnemyAttackRock::Update()
{
	position.y -= FallSpeed;
	rotateY += ModelRotateSpeedY;
	collisionPosition = VAdd(position, VGet(0.0f, AddCollisionPosY, 0.0f));

	//�����蔻����X�V
	UpdateCollisionData();

	//���ӃG�t�F�N�g
	if (!effect->IsPlayEffect(Effect::EffectKind::Warning))
	{
		effect->PlayEffect(Effect::EffectKind::Warning, warningEffectPosition, WarningEffectScale, WarningEffectRotate, WarningEffectPlaySpeed);
	}
	effect->Update(Effect::EffectKind::Warning, warningEffectPosition, WarningEffectRotate);

	//�Փˌ�
	if (!collisionActive)
	{
		effect->Update(Effect::EffectKind::RockHit, hitEffectPosition, HitEffectRotate);
		if (!effect->IsPlayEffect(Effect::EffectKind::RockHit))
		{
			end = true;
		}
	}

	//�n�ʂƂ̏Փ�
	if (position.y <= 0.0f && collisionActive)
	{
		hitEffectPosition = VAdd(position, VGet(0.0f, AddHitEffectPositionY, 0.0f));
		effect->PlayEffect(Effect::EffectKind::RockHit, hitEffectPosition, HitEffectScale, HitEffectRotate, HitEffectPlaySpeed);
		//�㉺�h��
		camera->PlayShakingVertical(CameraShakingPower, CameraShakingChangeDirFrame, CameraShakingPlayFrame);
		//�U��
		StartJoypadVibration(DX_INPUT_PAD1, PadVibPower, PadVibPlayFrame, -1);
		//se
		se->PlaySE(SoundEffect::SEKind::RockHit);
		collisionActive = false;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, rotateY, 0));
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// �`��
/// </summary>
void ArmEnemyAttackRock::Draw()
{
	if (collisionActive)
	{
		MV1DrawModel(modelHandle);
	}
	effect->Draw();

	//�m�F�p
	//DrawCapsule3D(collisionPosition, collisionPosition, CapsuleRadius, 8, GetColor(124, 34, 232), GetColor(124, 34, 232), false);
}

/// <summary>
/// �����蔻����X�V
/// </summary>
void ArmEnemyAttackRock::UpdateCollisionData()
{
	collisionData.tag = ObjectTag::EnemyAttack;
	collisionData.attackPower = AttackPower;
	collisionData.startPosition = collisionPosition;
	collisionData.endPosition = collisionPosition;
	collisionData.radius = CapsuleRadius;
	collisionData.isCollisionActive = collisionActive;
}

/// <summary>
/// �I�u�W�F�N�g�Փˎ�
/// </summary>
/// <param name="collisionData">�Փ˂����I�u�W�F�N�g</param>
void ArmEnemyAttackRock::OnHitObject(CollisionData* collisionData)
{
	if (collisionActive && (collisionData->tag == ObjectTag::StageObject || collisionData->tag == ObjectTag::PlayerWholeBody))
	{
		hitEffectPosition = VAdd(position, VGet(0.0f, AddHitEffectPositionY, 0.0f));
		effect->PlayEffect(Effect::EffectKind::RockHit, hitEffectPosition, HitEffectScale, HitEffectRotate, HitEffectPlaySpeed);
		//�U��
		//�㉺�h��
		camera->PlayShakingVertical(CameraShakingPower, CameraShakingChangeDirFrame, CameraShakingPlayFrame);
		//�U��
		StartJoypadVibration(DX_INPUT_PAD1, PadVibPower, PadVibPlayFrame, -1);
		//se
		se->PlaySE(SoundEffect::SEKind::RockHit);
		collisionActive = false;
	}
}