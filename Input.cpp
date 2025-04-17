#include"DxLib.h"
#include"Input.h"

/// <summary>
/// 入力ゲット
/// </summary>
/// <returns>入力状態</returns>
int Input::GetInputState()
{
	int inputstate = 0;

	if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1)
	{
		inputstate |= InputNumber::AButton;
	}
	if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_2)
	{
		inputstate |= InputNumber::BButton;
	}
	if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_3)
	{
		inputstate |= InputNumber::XButton;
	}
	if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_5)
	{
		inputstate |= InputNumber::L1;
	}
	if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_6)
	{
		inputstate |= InputNumber::R1;
	}
	if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_8)
	{
		inputstate |= InputNumber::Start;
	}

	return inputstate;
}