#pragma once

class Wall
{
public:
	Wall();
	~Wall();

	void Draw();

private:
	const VECTOR Scale = VGet(100.0f, 100.0f, 5.0f);	//�X�P�[��
	const VECTOR Position = VGet(0.0f, 0.0f, 4800.0f);	//�|�W�V����

	int modelHandle;
};