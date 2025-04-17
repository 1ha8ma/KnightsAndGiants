#pragma once

class Input;
class Calculation;

class Camera
{
public:
	Camera();

	//�X�^�[�g�V�[��������
	void StartSceneInitialize(VECTOR position,VECTOR lookPosition);
	//�X�^�[�g�V�[���X�V
	void UpdateStartScene(VECTOR position, VECTOR lookPosition);

	//�Q�[���V�[��������
	void GameInitialize(VECTOR playerpos);
	//�X�V
	void UpdateGame(VECTOR playerPosition,VECTOR targetCameraPosition,float addPlayerDistance);

	//�Q�[���I�[�o�[�V�[��������
	void InitializeGameOver(VECTOR playerPosition);
	//�Q�[���I�[�o�[�V�[���X�V
	void UpdateGameOver();

	//�Q�[���N���A�V�[��������
	void InitializeGameClear(VECTOR cameraPosition, VECTOR enemyPosition);
	//�Q�[���N���A�V�[���X�V
	void UpdateGameClear(VECTOR enemyPosition);
	
	//���o
	void PlayShakingVertical(float shakingPower, int shakingDirectionChangeflame, int flame);

	//GetSet
	float GetangleH() { return angleH; }
	float GetangleV() { return angleV; }
	VECTOR GetLookPosition() { return lookPosition; }
	VECTOR GetPosition() { return position; }

private:
	const float CameraUpLimit = -1.2f;	//���������� -0.7
	const float CameraDownLimit = 1.0f;	//����������� 1.0

	//���o�X�V
	void UpdateProduction(VECTOR playerPosition);
	//�㉺�h��
	void ShakingVertical(VECTOR playerPosition);

	//���N���X
	Input* input;
	Calculation* calculation;

	float Near;							//�`��\�ɂȂ�߂�����
	float Far;							//�`��\�ȉ���
	float AngleSpeed;					//����p�x
	float CameraPlayerTargetHeight;		//�v���C���[�̂ǂꂾ�������Ƃ�������邩
	float PlayerDistance;				//�v���C���[�Ƃ̋���
	VECTOR position;					//�|�W�V����
	VECTOR lookPosition;				//�����_
	float angleH;						//���ʊp�x
	float angleV;						//�����p�x

	//���o
	bool productionflg;					//���o���t���O
	int totalflame;						//���o�t���[��
	float t;							//Lerp�p
	bool lerpflg;						//Lerp�J�n�t���O
	float LerpSpeed;					//Lerp���x

	//�c�h��
	int changeflame;					//�؂�ւ��܂ł̃t���[��
	bool shakingDirection;				//�h�������t���O
	bool shakingVerticalflg;			//�c�h��t���O
	float shakingPower;					//�h��̋���
	int shakingDirectionChangeflame;	//�h��������ύX����t���[��
	int playflame;						//�Đ�����

	//�Q�[���I�[�o�[
	float LeaveSpeed;					//�v���C���[���痣��鑬�x
};