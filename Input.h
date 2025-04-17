#pragma once

class Input
{
public:
	enum InputNumber
	{
		AButton = 1,		//Aボタン
		BButton = 2,		//Bボタン
		XButton = 4,		//Xボタン
		Start = 8,			//start
		L1 = 16,			//L1
		R1 = 32,			//R1
	};

	//ボタン入力
	int GetInputState();
	//スティック入力
	DINPUT_JOYSTATE GetStickInput() { GetJoypadDirectInputState(DX_INPUT_PAD1, &stickInput); return stickInput; }

private:
	DINPUT_JOYSTATE stickInput;//スティック入力情報
};