#include"DxLib.h"
#include"Time.h"

//�C���X�^���X������
Time* Time::timeInstance = nullptr;

/// <summary>
/// �C���X�^���X�Q�b�g
/// </summary>
/// <returns></returns>
Time* Time::GetInstance()
{
	//�C���X�^���X��������ΐ���
	if (timeInstance == nullptr)
	{
		timeInstance = new Time();
	}

	return timeInstance;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Time::Time()
{
	//�ϐ�������
	startTime = 0;
	endTime = 0;
	for (int i = 0; i < DisplayRankingNum; i++)
	{
		ranking[StageTag::ArmEnemy].push_back(0);
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Time::~Time()
{
	ranking[StageTag::ArmEnemy].clear();
}

/// <summary>
/// �^�C�}�[�X�^�[�g
/// </summary>
/// <param name="tag"></param>
void Time::StartTimer()
{
	if (tag == StageTag::ArmEnemy)
	{
		//6�ڂ̃����L���O�����̉�̃^�C��
		ranking[tag].push_back(0);
	}

	startTime = GetNowCount();
}

/// <summary>
/// �^�C�}�[�X�g�b�v
/// </summary>
/// <param name="tag">�X�e�[�W�^�O</param>
/// <param name="clear">�N���A���Ă��邩</param>
bool Time::StopTimer(bool clear)
{
	endTime = (GetNowCount() - startTime) / 1000;	//�I������(�b)
	bool rankIn = false;

	if (clear)
	{
		ranking[tag].back() = endTime;
		rankIn = SortRanking(ranking[tag], endTime);
	}

	//�ŉ��ʂ�����
	ranking[tag].pop_back();

	return rankIn;
}

/// <summary>
/// �����L���O���בւ�
/// </summary>
/// <param name="ranking">���בւ������郉���L���O</param>
/// <param name="thisTime">���̉�̃^�C��</param>
/// <returns>�����L���O�ɓ�������</returns>
bool Time::SortRanking(std::vector<int>& ranking,int thisTime)
{
	bool rankIn = false;

	//�o�u���\�[�g(������)
	for (int i = 0; i < DisplayRankingNum; i++)
	{
		for (int j = 0; j < DisplayRankingNum - i; j++)
		{
			//0������
			if ((ranking[j] == 0 && ranking[j + 1] != 0) ||
				ranking[j] != 0 && ranking[j + 1] != 0 && ranking[j] > ranking[j + 1])
			{
				//����̃X�R�A���������ꍇ
				if (ranking[j + 1] == thisTime)
				{
					rankIn = true;
				}

				int temp = ranking[j];
				ranking[j] = ranking[j + 1];
				ranking[j + 1] = temp;
			}
		}
	}


	return rankIn;
}

/// <summary>
/// �����L���O�Q�b�g
/// </summary>
/// <param name="checkTag">�m�F����G�̎��</param>
/// <returns>�m�F�������G�̃����L���O</returns>
std::vector<int> Time::GetRanking(StageTag checkTag)
{
	return ranking[tag];
}