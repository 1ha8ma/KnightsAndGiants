#pragma once

class StageBase
{
public:
	StageBase() {};
	virtual ~StageBase() {};

	virtual void InitializeGameOver()abstract;
	virtual void InitializeGameClear()abstract;
	//更新
	virtual void Update()abstract;
	//描画
	virtual void Draw()abstract;

protected:
	const VECTOR centerPosition = VGet(0.0f, 0.0f, 0.0f);	//中心点
	
	int modelHandle;		//モデルハンドル
	VECTOR stagePosition;	//ステージの描画ポジション
};