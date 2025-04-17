#pragma once

class GameUI
{
public:
	GameUI(int enemyHP,int playerHP,int playerGrip,float maxPiercingRotateZ);
	~GameUI();

	void Update(int enemyHP, int playerHP, int playerGrip,bool onPiercingGauge,float piercingRotateZ);
	void Draw();

private:
	const float PlayerHPLength = 300.0f;	//�`��v���C���[HP�Q�[�W�̒���
	const float EnemyHPLength = 500.0f;		//�`��GHP�Q�[�W�̒���
	const float PlayerGripSize = 50.0f;		//�`��v���C���[���͂̑傫��
	const int TakeDamageChangeColorflame = 40;
	const int ColorChangeLowSpeed = 4;		//�F�̕ύX���x
	const int ColorChangeHighSpeed = 5;		

	float startEnemyHP;			//�J�n���GHP
	float startPlayerHP;		//�J�n���v���C���[HP
	float startPlayerGrip;		//�J�n���v���C���[����
	float maxPiercingRotateZ;	//�ő�˂��h������

	float prevEnemyHP;			//1�t���[���O�̓G��HP
	float prevPlayerHP;			//1�t���[���O�̃v���C���[��HP

	float enemyHP;				//���݂̓GHP
	float playerHP;				//���݂̃v���C���[HP
	float playerGrip;			//���݂̃v���C���[����
	float piercingRotateZ;		//�˂��h���r�̉�]�x

	float enemyHPPer;			//�GHP����
	float playerHPPer;			//�v���C���[HP����
	float playerGripPer;		//�v���C���[���͊���
	float piercingPer;

	bool enemyTakeDamage;		//�G���_���[�W���󂯂�
	bool playerTakeDamage;		//�v���C���[���_���[�W���󂯂�
	int enemyTakeDamageFlame;	//�G���_���[�W���󂯂����HP�o�[���o�̃t���[��
	int playerTakeDamageFlame;	//�v���C���[���_���[�W���󂯂����HP�o�[���o�̃t���[��
	bool onPiercingGauge;		//�˂��h���Q�[�W��\��

	int enemyColorR, enemyColorG, enemyColorB;
	int playerColorR, playerColorG, playerColorB;
};