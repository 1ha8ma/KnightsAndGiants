#include"DxLib.h"
#include"CollisionManager.h"
#include"SoundEffect.h"
#include"Effect.h"
#include"WeakPoint.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="tag">�����蔻��^�O</param>
/// <param name="partsName">�p�[�c�̖��O</param>
/// <param name="modelHandle">���f���n���h��</param>
/// <param name="frameIndex1">�J�v�Z���p�{�[���ԍ�1</param>
/// <param name="frameIndex2">�J�v�Z���p�{�[���ԍ��Q</param>
/// <param name="capsuleRadius">�J�v�Z�����a</param>
WeakPoint::WeakPoint(int modelHandle, int frameIndex1, int frameIndex2, float capsuleRadius)
{
	//�n���h���R�s�[
	this->modelHandle = modelHandle;
	//�^�O�ݒ�
	this->tag = ObjectTag::WeakPoint;

	//�t���[���ԍ��擾
	this->frameIndex1 = frameIndex1;
	this->frameIndex2 = frameIndex2;

	//�J�v�Z���ݒ�
	this->capsuleRadius = capsuleRadius;
	capsuleStart = MV1GetFramePosition(modelHandle, frameIndex1);
	capsuleEnd = MV1GetFramePosition(modelHandle, frameIndex2);

	//�C���X�^���X��
	se = new SoundEffect();

	//private�ϐ�������
	hitWeakPointEffectflg = false;
	hitWeakPointEffectflame = 0;

	//�����蔻����ǉ�
	collisionManager = collisionManager->GetInstance();
	collisionData.HitProcess = std::bind(&WeakPoint::OnHitObject, this, std::placeholders::_1);
	UpdateCollisionData();
	collisionManager->AddCollisionData(&collisionData);

	//�G�t�F�N�g
	effect = new Effect();
	effect->PlayEffect(Effect::EffectKind::WeakPoint, capsuleStart, WeakPointEffectScale, WeakPointEffectRotate, WeakPointEffectPlaySpeed);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
WeakPoint::~WeakPoint()
{
	MV1ResetFrameUserLocalMatrix(modelHandle, frameIndex1);
	MV1ResetFrameUserLocalMatrix(modelHandle, frameIndex2);
	collisionManager->RemoveCollisionData(&collisionData);
	//se�폜
	delete se;
	//�G�t�F�N�g�폜
	effect->RemoveEffect(Effect::EffectKind::WeakPoint);
	delete effect;
}

/// <summary>
/// ������
/// </summary>
void WeakPoint::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void WeakPoint::Update()
{
	//�J�v�Z���ݒ�
	//�t���[���Ɛe�t���[���̊�
	capsuleStart = MV1GetFramePosition(modelHandle, frameIndex1);
	capsuleEnd = MV1GetFramePosition(modelHandle, frameIndex2);

	//�����蔻����X�V
	UpdateCollisionData();

	//��_�Ȃ�G�t�F�N�g��t����
	bool playing = effect->IsPlayEffect(Effect::EffectKind::WeakPoint);

	//�I�����Ă�����܂��Đ�
	if (!playing)
	{
		effect->PlayEffect(Effect::EffectKind::WeakPoint, capsuleStart, WeakPointEffectScale, WeakPointEffectRotate, WeakPointEffectPlaySpeed);
	}

	effect->Update(Effect::EffectKind::WeakPoint, capsuleStart, WeakPointEffectRotate);


	//��_�q�b�g�̃G�t�F�N�g�X�V
	if (hitWeakPointEffectflg)
	{
		effect->Update(Effect::EffectKind::HitWeakPoint, capsuleStart, HitEffectRotate);

		if (hitWeakPointEffectflame == PlayHitWeakPointEffectFlame)
		{
			effect->RemoveEffect(Effect::EffectKind::HitWeakPoint);
			hitWeakPointEffectflg = false;
		}

		//�t���[�����Z
		hitWeakPointEffectflame++;
	}
}

/// <summary>
/// �`��
/// </summary>
void WeakPoint::Draw()
{
	effect->Draw();

	//�m�F�p
	//DrawCapsule3D(capsuleStart, capsuleEnd, capsuleRadius, 8, GetColor(102, 0, 255), GetColor(102, 0, 255), false);
	//DrawSphere3D(movePrevPos, capsuleRadius, 8, GetColor(102, 0, 255), GetColor(102, 0, 255), false);	//�|�W�V�����̓_
}

/// <summary>
/// �_���[�W��n��
/// </summary>
/// <returns>�_���[�W</returns>
int WeakPoint::TakeDamage()
{
	//�_���[�W�R�s�[
	int takeDamage = damage;
	//�_���[�W������
	damage = 0;

	return takeDamage;
}

/// <summary>
/// �Փˌ�̏���
/// </summary>
/// <param name="hitObjectData">�Փ˂����I�u�W�F�N�g</param>
void WeakPoint::OnHitObject(CollisionData* hitObjectData)
{
	//�v���C���[�̍U���������ꍇ
	if (hitObjectData->tag == ObjectTag::Attack_P)
	{
		se->PlaySE(SoundEffect::SEKind::HitWeakPoint);
		damage = hitObjectData->attackPower * WeakPointDamageMultiplier;
		if (!hitWeakPointEffectflg)
		{
			effect->PlayEffect(Effect::EffectKind::HitWeakPoint, capsuleStart, HitEffectScale, HitEffectRotate, HitEffectPlaySpeed);
			hitWeakPointEffectflame = 0;
			hitWeakPointEffectflg = true;
		}
	}
}

/// <summary>
/// �����蔻��X�V
/// </summary>
void WeakPoint::UpdateCollisionData()
{
	collisionData.tag = tag;
	collisionData.startPosition = capsuleStart;
	collisionData.endPosition = capsuleEnd;
	collisionData.radius = capsuleRadius;
	collisionData.attackPower = 0;
	collisionData.isCollisionActive = true;
}