#include<fstream>
#include<math.h>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"CollisionData.h"
#include"Camera.h"
#include"Calculation.h"
#include"Input.h"
#include"PlayerClimb.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle">���f���n���h��</param>
PlayerClimb::PlayerClimb(int modelHandle, VECTOR lookDir) :PlayerStateProcessBase(modelHandle)
{
	//�A�j���[�V�����A�^�b�`
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber:: Climb);
	//�A�j���[�V�����̑��Đ����Ԃ����
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);
	animTotalTime *= 2;

	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//�C���X�^���X��
	calculation = new Calculation();

	//�ϐ�������
	calculation = new Calculation();
	stopAnimation = false;
	newLookDirection = lookDir;
	Speed = jsonData["ClimbMoveSpeed"];
	prevClimbPolygonNumber = -1;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerClimb::~PlayerClimb()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// ������
/// </summary>
void PlayerClimb::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="inputstate">���͏��</param>
/// <param name="stickstate">�X�e�B�b�N���͏��</param>
/// <param name="camera">�J����</param>
/// <param name="objectCapsuleStart">�Փ˂��Ă���J�v�Z���n�_</param>
/// <param name="objectCapsuleEnd">�J�v�Z���I�_</param>
/// <returns>��Ԃ�ύX���邩</returns>
bool PlayerClimb::Update(UsePlayerData playerData, const Camera& camera,CollisionData objectCollision)
{
	bool stateChange = false;//��ԕύX�t���O

	//����
	Move(playerData, camera,objectCollision);
	//�A�j���[�V�����Đ�
	PlayAnimation(0.5f, stopAnimation);

	//R1�𗣂��ƃX�e�[�g�ύX
	if ((Input::InputNumber::R1 & playerData.inputState) != Input::InputNumber::R1)
	{
		stateChange = true;
	}

	return stateChange;
}

/// <summary>
///�o�蓮��
/// </summary>
/// <param name="playerData">�v���C���[���</param>
/// <param name="camera">�J����</param>
/// <param name="objectData">�I�u�W�F�N�g���</param>
void PlayerClimb::Move(UsePlayerData playerData, Camera camera, CollisionData objectData)
{
	//������
	moveVec = VGet(0, 0, 0);				
	stopAnimation = true;
	playerPosition = playerData.position;

	//�߂��|���S���̔ԍ������
	int nearPolygonNumber;				//�ł��߂��|���S���ԍ�
	VECTOR vertex0, vertex1, vertex2;	//�ł��߂��|���S���̒��_
	nearPolygonNumber = NearPolygon(objectData);
	vertex0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[nearPolygonNumber].VIndex[0]].Position;
	vertex1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[nearPolygonNumber].VIndex[1]].Position;
	vertex2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[nearPolygonNumber].VIndex[2]].Position;

	//�I�u�W�F�N�g�̈ړ��ɒ����Ă���
	nearPolygonCenter = calculation->TriangleCenter(vertex0, vertex1, vertex2);
	VECTOR objectMoveVec = FollowObjectMove(nearPolygonNumber, objectData);

	//�@���x�N�g��
	VECTOR normVec = calculation->Normalize(vertex0, vertex1, vertex2);

	//���͂����moveVec
	MoveVecForInput(playerData, normVec);

	//�����Ă�������X�V
	newLookDirection = VScale(normVec, -1);

	//��]�s��X�V
	UpdateRotateMatrix(newLookDirection, playerData.lookDirection, normVec);

	//�X�s�[�h���Z
	moveVec = VScale(moveVec, Speed);

	//�I�u�W�F�N�g�̓����������Z
	moveVec = VAdd(moveVec, objectMoveVec);
}

/// <summary>
/// �m�F�p
/// </summary>
void PlayerClimb::Draw()
{
	//�|�W�V�����Ɠo���Ă���|���S�����_
	DrawLine3D(nearPolygonCenter, playerPosition, GetColor(127, 255, 212));
}

/// <summary>
/// ���͂����moveVec
/// </summary>
/// <param name="playerData">�v���C���[���</param>
/// <param name="normVec">�@��</param>
void PlayerClimb::MoveVecForInput(UsePlayerData playerData, VECTOR normVec)
{
	//���͂���̃x�N�g���쐬
	VECTOR inputDir = VGet(0.0f, 0.0f, 0.0f);

	//��
	if (playerData.stickState.Y < 0.0f)
	{
		inputDir = VAdd(inputDir, VSub(playerData.capsuleStart, playerData.capsuleEnd));
	}
	//��
	if (playerData.stickState.Y > 0.0f)
	{
		inputDir = VAdd(inputDir, VSub(playerData.capsuleEnd, playerData.capsuleStart));
	}
	//�E
	if (playerData.stickState.X > 0.0f)
	{
		VECTOR cross = VCross(VSub(playerData.capsuleStart, playerData.capsuleEnd), playerData.lookDirection);
		inputDir = VAdd(inputDir, cross);
	}
	//��
	if (playerData.stickState.X < 0.0f)
	{
		VECTOR cross = VCross(playerData.lookDirection, VSub(playerData.capsuleStart, playerData.capsuleEnd));
		inputDir = VAdd(inputDir, cross);
	}

	//���͂��������ꍇ
	if (VSize(inputDir) != 0)
	{
		//�A�j���[�V�������~�߂Ȃ�
		stopAnimation = false;

		//���ʂ��ړ�����悤�ɕ␳
		inputDir = VNorm(inputDir);
		moveVec = ProjectOnPlane(inputDir, normVec);
		moveVec = VNorm(moveVec);
	}
}

/// <summary>
/// ��]�s��X�V
/// </summary>
/// <param name="newLookDirection">�V���������Ă������</param>
/// <param name="prevLookDirection">�O�Ɍ����Ă�������</param>
void PlayerClimb::UpdateRotateMatrix(VECTOR newLookDirection, VECTOR prevLookDirection, VECTOR normVec)
{
	//��]��
	VECTOR rotateAxis = VCross(VGet(0, 1, 0), normVec);
	//��]�p�x
	float dot = VDot(VGet(0, 1, 0), normVec);
	float angle = acos(dot) - DX_PI_F / 2;
	rotateMatrix = MGetRotAxis(rotateAxis, angle);
}

/// <summary>
/// �ł��߂������̃|���S���̔ԍ������
/// </summary>
/// <param name="objectData">�I�u�W�F�N�g���</param>
/// <returns>��ԋ߂��|���S���̔ԍ�</returns>
int PlayerClimb::NearPolygon(const CollisionData objectData)
{
	float nearDistance;				//��ԋ߂�����
	int nearPolygonNumber = -1;		//��ԋ߂��|���S���̔ԍ�

	for (int i = 0; i < objectData.meshData.polygonList.PolygonNum; i++)
	{
		//���_
		VECTOR vertex0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[i].VIndex[0]].Position;
		VECTOR vertex1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[i].VIndex[1]].Position;
		VECTOR vertex2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[i].VIndex[2]].Position;

		//���������
		float distance = Triangle_Point_MinLength(vertex0, vertex1, vertex2, playerPosition);

		//���ڂ̏���
		if (i == 0)
		{
			nearDistance = distance;
			nearPolygonNumber = i;
		}

		//�ł��߂��|���S���Ƃ��̋���
		if (nearDistance > distance)
		{
			nearDistance = distance;
			nearPolygonNumber = i;
		}
	}

	return nearPolygonNumber;
}

/// <summary>
/// ���͂��ꂽ�����|���S���̕��ʂɉf��
/// </summary>
/// <param name="inputDir">���͂��ꂽ����</param>
/// <param name="polygonNorm">�|���S���@��</param>
/// <returns>���ʂ̃x�N�g��</returns>
VECTOR PlayerClimb::ProjectOnPlane(VECTOR inputDir, VECTOR polygonNorm)
{
	float dot = VDot(inputDir, polygonNorm);
	return VSub(inputDir, VScale(polygonNorm, VDot(inputDir, polygonNorm)));
}

/// <summary>
/// �I�u�W�F�N�g�̓����ɒ����Ă���
/// </summary>
/// <param name="nearPolygonNumber">��ԋ߂��|���S���̔ԍ�</param>
/// <param name="objectData">�I�u�W�F�N�g���</param>
/// <returns>�I�u�W�F�N�g�̓�������</returns>
VECTOR PlayerClimb::FollowObjectMove(int nearPolygonNumber,CollisionData objectData)
{
	//�ŏ��̏ꍇ
	if (prevClimbPolygonNumber == -1)
	{
		prevClimbPolygonNumber = nearPolygonNumber;
		prevClimbPolygonCenter = nearPolygonCenter;
	}
	//�O�o���Ă����|���S���̌��݂̃|�W�V����
	VECTOR prevVer0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevClimbPolygonNumber].VIndex[0]].Position;
	VECTOR prevVer1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevClimbPolygonNumber].VIndex[1]].Position;
	VECTOR prevVer2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevClimbPolygonNumber].VIndex[2]].Position;
	//��L�̒��S�|�W�V����
	VECTOR nowPrevNumberCenterPos = calculation->TriangleCenter(prevVer0, prevVer1, prevVer2);
	//�|���S���̓������ʂ��v�Z
	VECTOR resultVec = VSub(nowPrevNumberCenterPos, prevClimbPolygonCenter);
	//�O�̃t���[���̏��Ƃ��ĕۑ�
	prevClimbPolygonNumber = nearPolygonNumber;
	prevClimbPolygonCenter = nearPolygonCenter;

	return resultVec;
}