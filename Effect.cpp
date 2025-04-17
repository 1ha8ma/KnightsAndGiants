#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"
#include"Effect.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Effect::Effect()
{
	Loader* loader = loader->GetInstance();

	effectHandle[EffectKind::WeakPoint] = loader->GetHandle(Loader::Kind::WeakPointEffect);
	effectHandle[EffectKind::HitWeakPoint] = loader->GetHandle(Loader::Kind::HitWeakPointEffect);
	effectHandle[EffectKind::Warning] = loader->GetHandle(Loader::Kind::WarningEffect);
	effectHandle[EffectKind::RockHit] = loader->GetHandle(Loader::Kind::RockHitEffect);

	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Effect::~Effect()
{
	//�G�t�F�N�g�I��
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		//Effekseer�ł͍Đ����~�߂鎖�͂ł��Ă��`����������Ƃ��o���Ȃ��������߉��ɔ�΂��Ă���
		SetPosPlayingEffekseer3DEffect(playingEffectHandle[i], 0.0f, -10000.0f, 0.0f);
		StopEffekseer3DEffect(playingEffectHandle[i]);
	}
	UpdateEffekseer3D();

	//���폜
	playingEffectHandle.clear();
	playingEffectKind.clear();
	playingEffectScale.clear();
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		playingEffectHandle.erase(playingEffectHandle.begin() + i);
		playingEffectKind.erase(playingEffectKind.begin() + i);
		playingEffectScale.erase(playingEffectScale.begin() + i);
	}

	DrawEffekseer3D_End();
}

/// <summary>
/// ������
/// </summary>
void Effect::Initialize()
{
	playingEffectHandle.clear();
	playingEffectKind.clear();
	playingEffectScale.clear();
}

/// <summary>
/// �G�t�F�N�g�Đ��w��
/// </summary>
/// <param name="kind">�G�t�F�N�g�̎��</param>
/// <param name="playPosition">�����Đ��ʒu</param>
/// <param name="initSize">�����T�C�Y</param>
/// <param name="rotate">��]</param>
/// <param name="playSpeed">�Đ����x</param>
void Effect::PlayEffect(EffectKind kind, VECTOR playPosition, VECTOR initScale, VECTOR rotate, float playSpeed)
{
	//�Đ����Ă���G�t�F�N�g��ǉ�
	playingEffectKind.push_back(kind);
	//�n���h���ǉ�
	playingEffectHandle.push_back(PlayEffekseer3DEffect(effectHandle[kind]));

	//�X�P�[���ݒ�
	playingEffectScale.push_back(initScale);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle.back(), playingEffectScale.back().x, playingEffectScale.back().y, playingEffectScale.back().z);

	//�G�t�F�N�g�Đ����x�ݒ�
	SetSpeedPlayingEffekseer3DEffect(playingEffectHandle.back(), playSpeed);
	//�\���p�x�ݒ�
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle.back(), rotate.x, rotate.y, rotate.z);
	//�F�ύX
	if (kind == EffectKind::WeakPoint || kind == EffectKind::HitWeakPoint)
	{
		SetColorPlayingEffekseer3DEffect(playingEffectHandle.back(), 0, 255, 255, 255);
	}
	//�|�W�V�������f
	SetPosPlayingEffekseer3DEffect(playingEffectHandle.back(), playPosition.x, playPosition.y, playPosition.z);
}

/// <summary>
/// �G�t�F�N�g���̍X�V
/// </summary>
/// <param name="kind">�|�W�V���������X�V�����������</param>
/// <param name="playPosition">�Đ��|�W�V����</param>
/// <param name="rotate">�Đ��p�x</param>
void Effect::Update(EffectKind kind,VECTOR playPosition, VECTOR rotate)
{
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		//�w��Ɠ������̂��X�V
		if (playingEffectKind[i] == kind)
		{
			//�\���p�x�ݒ�
			SetRotationPlayingEffekseer3DEffect(playingEffectHandle[i], rotate.x, rotate.y, rotate.z);
			//�|�W�V�������f
			SetPosPlayingEffekseer3DEffect(playingEffectHandle[i], playPosition.x, playPosition.y, playPosition.z);
			//�Đ��I�����Ă�����Đ�������폜
			if (IsEffekseer3DEffectPlaying(playingEffectHandle[i]) == -1)
			{
				playingEffectHandle.erase(playingEffectHandle.begin() + i);
				playingEffectKind.erase(playingEffectKind.begin() + i);
				playingEffectScale.erase(playingEffectScale.begin() + i);
			}

			break;
		}
	}
}

/// <summary>
/// �`��
/// </summary>
void Effect::Draw()
{
	DrawEffekseer3D();
}

/// <summary>
/// �w��̃G�t�F�N�g���Đ�����
/// </summary>
/// <param name="kind">�G�t�F�N�g�̎��</param>
/// <returns>�Đ����Ftrue ���Đ��Ffalse</returns>
bool Effect::IsPlayEffect(EffectKind kind)
{
	bool playing = false;

	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		if (playingEffectKind[i] == kind)
		{
			playing = true;
			break;
		}
	}

	return playing;
}

/// <summary>
/// �G�t�F�N�g�폜(�������[�v�G�t�F�N�g�p)
/// </summary>
/// <param name="kind">�폜����G�t�F�N�g</param>
void Effect::RemoveEffect(EffectKind kind)
{
	//�G�t�F�N�g�I��
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		//Effekseer�ł͍Đ����~�߂鎖�͂ł��Ă��`����������Ƃ��o���Ȃ��������߉��ɔ�΂��Ă���
		if (playingEffectKind[i] == kind)
		{
			SetPosPlayingEffekseer3DEffect(playingEffectHandle[i], 0.0f, -10000.0f, 0.0f);
			StopEffekseer3DEffect(playingEffectHandle[i]);

			break;
		}
	}
	UpdateEffekseer3D();

	//���폜
	playingEffectHandle.clear();
	playingEffectKind.clear();
	playingEffectScale.clear();
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		playingEffectHandle.erase(playingEffectHandle.begin() + i);
		playingEffectKind.erase(playingEffectKind.begin() + i);
		playingEffectScale.erase(playingEffectScale.begin() + i);
	}
}