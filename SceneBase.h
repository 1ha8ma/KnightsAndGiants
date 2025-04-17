#pragma once

class SceneBase
{
public:
	SceneBase() {};
	virtual ~SceneBase() {};
	//最初の初期化
	static SceneBase* InitializeBase();
	//シーンごとの初期化
	virtual void Initialize() abstract;
	//シーンごとの更新
	virtual SceneBase* Update() abstract;
	//シーンごとの描画
	virtual void Draw() abstract;
};