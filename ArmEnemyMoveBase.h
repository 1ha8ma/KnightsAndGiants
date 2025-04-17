#pragma once
#include"Camera.h"

class ArmEnemyMoveBase
{
public:
	//�t���[���ԍ�
	enum class ArmEnemyFrameIndex :int
	{
		Upperarm = 16,				//��r
		Forearm = 17,				//�O�r
		Hand = 18,					//��
		Finger1 = 22,				//�e�w
		LeftHandMiddle = 30,		//���w���֐�
		Finger2 = 40,				//���w�̐�
	};

	ArmEnemyMoveBase(int modelHandle);
	virtual ~ArmEnemyMoveBase() {};
	
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <returns>�����̏I��</returns>
	virtual bool Update(Camera* camera,VECTOR playerPosition) { return false; }

	/// <summary>
	/// �|���ꂽ��p�X�V
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <returns>�����̏I��</returns>
	virtual bool UpdateFallDown(Camera* camera) { return false; }

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw()abstract;


	VECTOR GetRotate() { return rotate; }

protected:
	int modelHandle;	//���f���n���h��
	VECTOR rotate;		//��]�p
	int moveState;		//�������
};