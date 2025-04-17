#pragma once
#include<map>

class SoundEffect
{
public:
	//SE�̎��
	enum class SEKind :int
	{
		NormalAttack,		//�ʏ�U��
		MowinDownAttack,	//�U�艺�낵�U��
		Swing,				//�U���
		HitWeakPoint,		//��_�ɓ���������
		Jump,				//�W�����v
		CursorMove,			//�J�[�\���ړ�
		Crick,				//����
		RockHit,			//��Փ�
	};

	SoundEffect();
	~SoundEffect();

	//SE�Đ�
	void PlaySE(SEKind kind);
	//�����SE���I��
	void StopSE(SEKind kind);

private:
	//���ʊ���
	enum class Ratio :int
	{
		NormalAttack = 80,
		MowingAttack = 100,
		Swing = 100,
		HitWeakPoint = 100,
		CursorMove = 70,
		Crick = 100,
		RockHit = 100,
	};

	const int MaxVolume = 255;		//�ő剹��

	std::map<SEKind, int> handle;	//�n���h��
};