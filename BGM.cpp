#include"DxLib.h"
#include"Loader.h"
#include"BGM.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="kind">�ŏ��Ɏg�p����BGM</param>
BGM::BGM(BGMKind kind)
{
	Loader* loader = loader->GetInstance();

	//�n���h���ǂݍ��݁E���ʒ���
	handle[BGMKind::Title] = loader->GetHandle(Loader::Kind::TitleBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::Title / 100, handle[BGMKind::Title]);
	handle[BGMKind::GameOver] = loader->GetHandle(Loader::Kind::GameOverBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::GameOver / 100, handle[BGMKind::GameOver]);
	handle[BGMKind::ArmEnemyStage] = loader->GetHandle(Loader::Kind::ArmEnemyBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::ArmEnemyStage / 100, handle[BGMKind::ArmEnemyStage]);
	handle[BGMKind::GameClear] = loader->GetHandle(Loader::Kind::GameClearBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::GameClear / 100, handle[BGMKind::GameClear]);
	handle[BGMKind::Ranking] = loader->GetHandle(Loader::Kind::RankingBGM);
	ChangeVolumeSoundMem(MaxVolume * (int)Ratio::Ranking / 100, handle[BGMKind::Ranking]);

	//������
	playingBGM = handle[kind];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BGM::~BGM()
{

}

/// <summary>
/// BGM�Đ�
/// </summary>
/// <param name="kind">�Đ�������</param>
void BGM::PlayBGM(BGMKind kind)
{
	//�Đ����łȂ���΍Đ�
	if (!CheckSoundMem(playingBGM))
	{
		playingBGM = handle[kind];
		PlaySoundMem(playingBGM, DX_PLAYTYPE_LOOP, true);
	}
}

/// <summary>
/// BGM���~�߂�
/// </summary>
void BGM::StopBGM()
{
	if (CheckSoundMem(playingBGM))
	{
		StopSoundMem(playingBGM);
	}
}