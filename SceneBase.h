#pragma once

class SceneBase
{
public:
	SceneBase() {};
	virtual ~SceneBase() {};
	//�ŏ��̏�����
	static SceneBase* InitializeBase();
	//�V�[�����Ƃ̏�����
	virtual void Initialize() abstract;
	//�V�[�����Ƃ̍X�V
	virtual SceneBase* Update() abstract;
	//�V�[�����Ƃ̕`��
	virtual void Draw() abstract;
};