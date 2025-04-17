#pragma once
#include<vector>
#include<map>

class Effect
{
public:
	//�G�t�F�N�g�̎��
	enum class EffectKind :int
	{
		WeakPoint,		//��_
		HitWeakPoint,	//��_�q�b�g
		Warning,		//����
		RockHit,		//��Փ�
	};

	Effect();
	~Effect();

	//������
	void Initialize();
	//�G�t�F�N�g�Đ�
	void PlayEffect(EffectKind kind, VECTOR playPosition, VECTOR initScale, VECTOR rotate, float playSpeed);
	//�X�V
	void Update(EffectKind kind, VECTOR playPosition, VECTOR rotate);
	//�`��
	void Draw();

	//�w��̃G�t�F�N�g���Đ������m�F
	bool IsPlayEffect(EffectKind kind);

	//�Đ����̃G�t�F�N�g���폜(�������[�v�̃G�t�F�N�g�p)
	void RemoveEffect(EffectKind kind);

private:
	std::vector<EffectKind> playingEffectKind;		//�Đ����̃G�t�F�N�g�̎��
	std::vector<int> playingEffectHandle;			//�Đ����̃G�t�F�N�g�n���h��
	std::vector<VECTOR> playingEffectScale;			//�G�t�F�N�g�T�C�Y
	std::map<EffectKind, int> effectHandle;			//�G�t�F�N�g�n���h��
};