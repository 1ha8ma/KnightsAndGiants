#pragma once

class FPS
{
public:
	FPS();
	void Update();
	void Wait();

private:
	const int N = 60;//���ς����T���v����
	const int AssumptionFPS = 60;//�z�肵�Ă���FPS

	int StartTime;//����J�n����
	int flameCount;//�t���[���J�E���^
	float mFps;//fps
};