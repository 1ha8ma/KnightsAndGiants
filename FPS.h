#pragma once

class FPS
{
public:
	FPS();
	void Update();
	void Wait();

private:
	const int N = 60;//平均を取るサンプル数
	const int AssumptionFPS = 60;//想定しているFPS

	int StartTime;//測定開始時間
	int flameCount;//フレームカウンタ
	float mFps;//fps
};