#pragma once

class SceneBase;

class GameManager
{
public:
	GameManager();
	//�X�V
	void Update();
	//�`��
	void Draw();

private:
	SceneBase* nowscene;
	SceneBase* nextscene;

	//�V�[���؂�ւ�
	void ChangeScene();
};