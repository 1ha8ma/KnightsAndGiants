#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"
#include"FPS.h"
#include"GameManager.h"
#include"Utility.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/////////////////////////
	//�@��{�ݒ�
	/////////////////////////
	SetGraphMode(SCREEN_W, SCREEN_H, 32);
	SetWaitVSyncFlag(FALSE);//��������
	ChangeWindowMode(TRUE);//FASLE ��@TRUE ��
	//���C�e�B���O�����g�p�ݒ�
	SetUseLighting(FALSE);

	//���C�u����������
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	DxLib_Init();
	Effekseer_Init(8000);
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	SetUseZBuffer3D(TRUE);

	//�E�B���h�E���[�h���O�ύX
	SetMainWindowText("KnightsAndGiants");
	//�_�u���o�b�t�@�����O��L��
	SetDrawScreen(DX_SCREEN_BACK);

	/////////////////////////
	// �Q�[��������
	/////////////////////////
	//�f�ރ��[�h
	Loader* loader = loader->GetInstance();
	loader->Load();

	//�t�H���g�ύX
	ChangeFont("���E����", DX_CHARSET_DEFAULT);

	//�C���X�^���X��
	FPS* fps = new FPS();
	GameManager* game = new GameManager();

	/////////////////////////
	// �Q�[�����[�v
	/////////////////////////
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		//��ʃN���A
		ClearDrawScreen();

		//fps�X�V
		fps->Update();

		//�X�V
		game->Update();
		//�`�揈��
		game->Draw();

		//�`��
		ScreenFlip();

		//fps�Œ�
		fps->Wait();
	}

	//�I��
	Effkseer_End();
	DxLib_End();
}
