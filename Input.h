#pragma once

class Input
{
public:
	enum InputNumber
	{
		AButton = 1,		//A�{�^��
		BButton = 2,		//B�{�^��
		XButton = 4,		//X�{�^��
		Start = 8,			//start
		L1 = 16,			//L1
		R1 = 32,			//R1
	};

	//�{�^������
	int GetInputState();
	//�X�e�B�b�N����
	DINPUT_JOYSTATE GetStickInput() { GetJoypadDirectInputState(DX_INPUT_PAD1, &stickInput); return stickInput; }

private:
	DINPUT_JOYSTATE stickInput;//�X�e�B�b�N���͏��
};