#pragma once

class SceneBase;

class GameManager
{
public:
	GameManager();
	//更新
	void Update();
	//描画
	void Draw();

private:
	SceneBase* nowscene;
	SceneBase* nextscene;

	//シーン切り替え
	void ChangeScene();
};