#pragma once

class StageBase
{
public:
	StageBase() {};
	virtual ~StageBase() {};

	virtual void InitializeGameOver()abstract;
	virtual void InitializeGameClear()abstract;
	//�X�V
	virtual void Update()abstract;
	//�`��
	virtual void Draw()abstract;

protected:
	const VECTOR centerPosition = VGet(0.0f, 0.0f, 0.0f);	//���S�_
	
	int modelHandle;		//���f���n���h��
	VECTOR stagePosition;	//�X�e�[�W�̕`��|�W�V����
};