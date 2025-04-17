#pragma once
#include<map>

class Loader
{
public:
	static Loader* GetInstance();//�C���X�^���X�Q�b�g

	enum Kind
	{
		//3D
		PlayerModel,					//�v���C���[
		ArmEnemyModel,					//�r�̓G
		WallModel,						//��
		BossEnemyModel,					//�{�X
		WoodModel,						//��
		ArmEnemyStage,					//�r�̓G��X�e�[�W
		BossEnemyStage,					//�{�X��X�e�[�W
		SkyDome,						//�X�J�C�h�[��
		Enemy2Model,					//2�Ԗڂ̓G���f��
		RockModel1,						//��1
		RockModel2,						//��2
		RockModel3,						//��3
		RockModel4,						//��4

		//2D
		GameOverBackImage,				//�Q�[���I�[�o�[�w�i
		TitleBackImage,					//�^�C�g���w�i
		RankingArmEnemyImage,			//�����L���O��ʂ̘r�̓G�w�i
		ClearTextBackImage,				//�N���A��ʂ̃e�L�X�g�w�i
		TutorialBackImage,				//��������w�i
		DemoMovie,						//�f���f��

		//BGM
		TitleBGM,						//�^�C�g��BGM
		GameOverBGM,					//�Q�[���I�[�o�[BGM
		ArmEnemyBGM,					//�r�̓G��
		GameClearBGM,					//�Q�[���N���ABGM
		RankingBGM,						//�����L���O���BGM

		//SE
		MowinDownSE,					//�U�艺�낵�U��SE
		NormalAttackSE,					//�ʏ�U��SE
		SwingSE,						//�U��܂��SE
		HitWeakPointSE,					//��_�q�b�gSE
		CrickSE,						//����SE
		CursorMoveSE,					//�J�[�\���ړ�SE
		RockHitSE,						//��Փ�SE

		//�G�t�F�N�g
		WeakPointEffect,				//��_
		HitWeakPointEffect,				//��_�q�b�g�G�t�F�N�g
		WarningEffect,					//�₪�����Ă���ꏊ�̒��ӃG�t�F�N�g
		RockHitEffect,					//�₪�������ꏊ�ɏo���G�t�F�N�g
	};

	//�n���h���Q�b�g
	int GetHandle(Kind kind) { return handle[kind]; }
	//���[�h
	void Load();

private:
	//�C���X�^���X
	static Loader* loader;

	//�n���h���}�b�v
	std::map<Kind, int> handle;
};