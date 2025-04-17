#pragma once
#include<vector>
#include<map>

class Time
{
public:
	static Time* GetInstance();		//�C���X�^���X�Q�b�g

	//�X�e�[�W�̎��
	enum class StageTag :int
	{
		ArmEnemy,	//�r�̓G��
	};

	//�^�C�}�[�J�n
	void StartTimer();
	//�^�C�}�[�X�g�b�v
	bool StopTimer(bool clear);

	//Get,Set
	void SetTag(StageTag tag) { this->tag = tag; }
	int GetTime() const{ return endTime; }
	std::vector<int> GetRanking(StageTag tag);

private:
	Time();
	~Time();

	static Time* timeInstance;	//�C���X�^���X

	const int DisplayRankingNum = 5;	//�\�����郉���L���O�̐�

	//�����L���O���בւ�
	bool SortRanking(std::vector<int>& ranking,int thisTime);

	//�����L���O
	std::map<StageTag, std::vector<int>> ranking;
	//std::vector<int> armEnemyRanking;

	StageTag tag;			//���̉�̃^�O
	int startTime;		//�X�^�[�g����
	int endTime;		//�Q�[���I������
};