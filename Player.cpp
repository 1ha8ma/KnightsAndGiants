#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"Loader.h"
#include"Camera.h"
#include"Input.h"
#include"CollisionManager.h"
#include"Calculation.h"
#include"PlayerRun.h"
#include"PlayerJump.h"
#include"PlayerNormalAttack.h"
#include"PlayerClimb.h"
#include"PlayerSquat.h"
#include"PlayerPiercing.h"
#include"PlayerFallDown.h"
#include"PlayerIdle.h"
#include"PlayerFalling.h"
#include"Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
	Loader* loader = loader->GetInstance();

	modelHandle = loader->GetHandle(Loader::Kind::PlayerModel);

	InitializeStartScene();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	collisionManager->RemoveCollisionData(&bodyCollisionData);
	collisionManager->RemoveCollisionData(&footCollisionData);
	delete input;
	delete nowstate;
}

/// <summary>
/// �X�^�[�g�V�[��������
/// </summary>
void Player::InitializeStartScene()
{
	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	nowstate = new PlayerRun(modelHandle, VGet(0.0f, 0.0f, 0.0f));
	positionDistanceGround = jsonData["PositionDistanceGround"];
	position = VGet(jsonData["InitPositionX"], positionDistanceGround, jsonData["InitPositionZ"]);
	drawPosition = position;
	angle = 0.0f;
}

/// <summary>
/// �Q�[���V�[��������
/// </summary>
void Player::InitializeGame()
{
	//�C���X�^���X��
	nowstate = new PlayerIdle(modelHandle, VGet(0.0f, 0.0f, 0.0f));
	input = new Input();
	calculation = new Calculation();

	//�t�@�C���ǂݍ���
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//�ϐ�������
	//�X�e�[�^�X
	MaxHP = jsonData["PlayerMaxHP"];
	HP = MaxHP;
	MaxGripPoint = jsonData["MaxGripPoint"];
	gripPoint = MaxGripPoint;
	//����
	canInputX = false;
	canInputA = false;
	//���f���֌W
	drawPosition = position;
	lookDirection = VGet(0, 0, 1);
	//�����蔻��
	positionDistanceGround = jsonData["PositionDistanceGround"];
	position = VGet(jsonData["InitPositionX"], positionDistanceGround, jsonData["InitPositionZ"]);
	WholeBodyCapsuleRadius = jsonData["WholeBodyCapsuleRadius"];
	WholeBodyCapsuleHalfLength = jsonData["WholeBodyCapsuleHalfLength"];
	FootCapsuleRadius = jsonData["FootCapsuleRadius"];

	//�ړ��֌W
	angle = 0.0f;
	changeAngleSpeed = jsonData["ChangeAngleSpeed"];
	jumpAfterLeaveFoot = false;
	onGround = true;
	onFootObject = false;
	isCatch = false;
	//�����֌W
	nowstateKind = State::Run;
	isChangeState = false;
	//�����֌W
	fallSpeed = 0.0f;
	fallFrame = 0;
	fallDamage = 0;
	FallDamageIncrease = jsonData["FallDamageIncrease"];
	Gravity = jsonData["Gravity"];
	//�˂��h���U��
	cameraZoom = 0.0f;
	piercingArmRotateZ = 0.0f;
	onPiercingGauge = false;
	//�o��
	MinusGripPoint = jsonData["MinusGripPoint"];
	//���̑�
	isGameOver = false;

	//�����蔻��
	collisionManager = collisionManager->GetInstance();
	//�Փˌ�̏�����n��
	bodyCollisionData.HitProcess = std::bind(&Player::BodyOnHitObject, this, std::placeholders::_1);
	bodyCollisionData.WallHitProcess = std::bind(&Player::WallHitProcess, this, std::placeholders::_1);
	footCollisionData.HitProcess = std::bind(&Player::FootOnHitObject, this, std::placeholders::_1);
	//�����蔻��ɕK�v�ȃf�[�^��n��
	collisionManager->AddCollisionData(&bodyCollisionData);
	collisionManager->AddCollisionData(&footCollisionData);

	//�|�W�V�������f
	MV1SetPosition(modelHandle, drawPosition);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="camera">�J����</param>
/// <returns>�Q�[���I�[�o�[</returns>
bool Player::UpdateGame(Camera* camera)
{
	//����
	inputstate = input->GetInputState();
	stickstate = input->GetStickInput();

	//���͂ɂ���ē����ύX
	ChangeState();

	//�����X�V�ɕK�v�ȏ���n��
	MoveUseDataSet();

	//�����X�V
	CollisionData latestData = hitObjectData;	
	if (nowstateKind == State::Climb)
	{
		//�ŐV�̃f�[�^���擾
		latestData = *hitObjectDataPointer;
	}
	isChangeState = nowstate->Update(moveUseData, *camera, latestData);
	rotateMatrix = nowstate->GetRotateMatrix();
	lookDirection = nowstate->GetLookDirection();
	lookDirection = VNorm(lookDirection);

	//�ړ�
	moveVec = nowstate->GetmoveVec();
	//�������x�ǉ�
	moveVec = VSub(moveVec, VGet(0.0f, fallSpeed, 0.0f));
	//�|�W�V�������f
	position = VAdd(position, moveVec);
	//�������Ă��邩�m�F
	CheckOnGround(camera);

	//�p�x�X�V
	UpdateAngle();

	//�������̏���
	MoveStateProcess();

	//�`��ʒu�ݒ�
	DrawPositionSet();

	//�A�����͖h�~
	PreventionContinuousInput();

	//�Q�[���I�[�o�[�m�F
	if (HP <= 0)
	{
		isGameOver = true;
	}

	//�J�v�Z�����X�V
	UpdateCapsule();
	UpdateCollisionData();

	//�`��|�W�V�����ݒ�
	MV1SetPosition(modelHandle, drawPosition);

	//�ϐ�������
	isCatch = false;
	onFootObject = false;
	
	return isGameOver;
}

/// <summary>
/// �Q�[���I�[�o�[�V�[��������
/// </summary>
void Player::InitializeGameOver()
{
	delete nowstate;
	nowstateKind = State::FallDown;
	nowstate = new PlayerFallDown(modelHandle);
}

/// <summary>
/// �Q�[���I�[�o�[�V�[���X�V
/// </summary>
void Player::UpdateGameOver()
{
	nowstate->UpdateGameOver();
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	MV1DrawModel(modelHandle);

	//�m�F�p
	//DrawCapsule3D(bodyCollisionData.startPosition, bodyCollisionData.endPosition, WholeBodyCapsuleRadius, 8, GetColor(220, 20, 60),GetColor(220,20,60), FALSE);
	//nowstate->Draw();	
	//DrawLine3D(position, VAdd(position, VScale(lookDirection, 200)), GetColor(127, 255, 0));							//lookDirection
	//DrawLine3D(position, wholebodyCapStart, GetColor(220, 20, 60));		//�^�񒆂��瓪
	//DrawCapsule3D(footCapStart, footCapEnd, FootCapsuleRadius, 8, GetColor(220, 20, 60), GetColor(220, 20, 60), FALSE);	//��
}

/// <summary>
/// �Փˎ��̏���
/// </summary>
/// <param name="hitObjectData">�Փ˂����I�u�W�F�N�g</param>
void Player::BodyOnHitObject(CollisionData* hitObjectData)
{
	//�Փ˂����I�u�W�F�N�g���Ƃɏ�����ύX
	//�G�̍U��
	if (hitObjectData->tag == ObjectTag::EnemyAttack)
	{
		//HP����
		HP -= hitObjectData->attackPower;

		if (HP < 0)
		{
			HP = 0;
		}
	}

	//�J�v�Z��
	if (hitObjectData->tag == ObjectTag::StageObject ||
		hitObjectData->tag == ObjectTag::Enemy)
	{
		//�����߂�
		if (nowstateKind != State::Climb)
		{
			CollisionPushBack(hitObjectData);
		}
	}
}

/// <summary>
/// �I�u�W�F�N�g�Փˎ��̏���(��)
/// </summary>
/// <param name="hitObjectData">�Փ˂����I�u�W�F�N�g</param>
void Player::FootOnHitObject(CollisionData* hitObjectData)
{
	if (hitObjectData->tag == ObjectTag::StageObject ||
		hitObjectData->tag == ObjectTag::Enemy)
	{
		//�����Ȃ�
	}
}

/// <summary>
/// �p�x�X�V(���������̊p�x�̂�)
/// </summary>
void Player::UpdateAngle()
{
	//�ړ������Ƀ��f���̕������߂Â���
	float targetAngle;		//�ڕW�̊p�x
	float difference;		//�ڕW�p�x�ƌ��݂̊p�x�̍�

	//�ڕW�̕����x�N�g������p�x�l���Z�o����
	targetAngle = static_cast<float>(atan2(lookDirection.x, lookDirection.z));

	//�ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
	//�ŏ��͈����Z
	difference = targetAngle - angle;

	//����������炠������̍���180�x�ȏ�ɂȂ邱�Ƃ͂Ȃ��̂ō���180�x�ȏ�ɂȂ��Ă�����C������
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	//�p�x�̍���0�ɋ߂Â���
	if (difference > 0.0f)//�����}�C�i�X�̏ꍇ
	{
		difference -= changeAngleSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else//�����v���X�̏ꍇ
	{
		difference += changeAngleSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	//���f���̊p�x���X�V
	angle = targetAngle - difference;
}

/// <summary>
/// �X�e�[�g�ύX
/// </summary>
void Player::ChangeState()
{
	//�ҋ@
	if (nowstateKind == State::Run && (stickstate.X == 0.0f && stickstate.Y == 0.0f) ||
		nowstateKind == State::Jump && onGround && jumpAfterLeaveFoot ||
		nowstateKind == State::NormalAttack && isChangeState ||
		nowstateKind == State::Climb && isChangeState || nowstateKind == State::Climb && gripPoint <= 0 ||
		nowstateKind == State::Squat && isChangeState ||
		nowstateKind == State::Piercing && isChangeState ||
		nowstateKind == State::Falling && onGround)
	{
		delete nowstate;
		nowstateKind = State::Idle;
		nowstate = new PlayerIdle(modelHandle, lookDirection);
	}

	//����
	if (nowstateKind != State::Run && nowstateKind != State::Climb && nowstateKind != State::NormalAttack && nowstateKind != State::Piercing && nowstateKind != State::Squat && nowstateKind != State::Jump && onGround && (stickstate.X != 0.0f || stickstate.Y != 0.0f))
	{
		delete nowstate;
		nowstateKind = State::Run;
		nowstate = new PlayerRun(modelHandle, lookDirection);
		prevHitPolygonNumber = -1;
	}

	//�W�����v
	if (nowstateKind != State::Jump && nowstateKind != State::NormalAttack && nowstateKind != State::Climb && canInputA && (Input::InputNumber::AButton & inputstate) == Input::InputNumber::AButton)
	{
		delete nowstate;
		onGround = false;
		jumpAfterLeaveFoot = false;
		nowstateKind = State::Jump;
		nowstate = new PlayerJump(modelHandle, moveVec, lookDirection);
	}

	//�˂��h���U��
	if (nowstateKind == State::Squat && canInputX && (Input::InputNumber::XButton & inputstate) == Input::InputNumber::XButton)
	{
		delete nowstate;
		nowstateKind = State::Piercing;
		nowstate = new PlayerPiercing(modelHandle, lookDirection);
	}

	//�ʏ�U���@(�˂��h���U���̌�ɔ��肵�Ȃ��ƈ�x�ʏ�U���ɓ����Ă��܂�)
	if (nowstateKind != State::NormalAttack && nowstateKind != State::Piercing && nowstateKind != State::Jump && nowstateKind != State::Squat && nowstateKind != State::Climb && canInputX && (Input::InputNumber::XButton & inputstate) == Input::InputNumber::XButton)
	{
		delete nowstate;
		nowstateKind = State::NormalAttack;
		nowstate = new PlayerNormalAttack(modelHandle, lookDirection);
	}

	//�͂܂�
	if (nowstateKind != State::Climb && isCatch && !onGround && (Input::InputNumber::R1 & inputstate) == Input::InputNumber::R1 && gripPoint > 20)
	{
		delete nowstate;
		nowstateKind = State::Climb;
		nowstate = new PlayerClimb(modelHandle, lookDirection);
	}

	//���Ⴊ��
	if (onGround && nowstateKind != State::Piercing && nowstateKind != State::Squat && nowstateKind != State::Climb && !isCatch && (Input::InputNumber::R1 & inputstate) == Input::InputNumber::R1)
	{
		delete nowstate;
		nowstateKind = State::Squat;
		nowstate = new PlayerSquat(modelHandle, lookDirection);
	}

	//����(���t���[���ȏ㗎���Ă���ꍇ�Ɉڍs)
	if (fallFrame > 25 && !onGround && nowstateKind != State::Falling && nowstateKind != State::Jump && nowstateKind != State::Climb)
	{
		delete nowstate;
		nowstateKind = State::Falling;
		nowstate = new PlayerFalling(modelHandle, lookDirection);
	}
}

/// <summary>
/// ���������Ă��邩�m�F
/// </summary>
/// <param name="camera">�J����</param>
void Player::CheckOnGround(Camera* camera)
{
	//���n�����Ƃ�
	if ((nowstateKind == State::Jump && jumpAfterLeaveFoot && (!onGround && position.y - positionDistanceGround < 0.0f || !onGround && onFootObject)) ||
		nowstateKind != State::Jump && (!onGround && position.y - positionDistanceGround < 0.0f || !onGround && onFootObject))
	{
		onGround = true;
		moveVec.y = 0.0f;

		//��莞�ԗ������Ă����痎���_���[�W
		if (fallFrame >= FallDamageStartFrame)
		{
			HP -= fallDamage;
			//�J�����U��
			camera->PlayShakingVertical(FallDamageCameraShakingPower, FallDamageCameraShakingDirChangeflame, FallDamageCameraShakingPlayflame);
			//�U��
			StartJoypadVibration(DX_INPUT_PAD1, FallDamageJoypadVibPower, FallDamageJoypadVibPlayflame, -1);
		}
	}

	//���������Ă��Ȃ����
	if (position.y - positionDistanceGround >= 0.0f && !onFootObject && nowstateKind != State::Climb)
	{
		onGround = false;						//���n���Ă��Ȃ��ɕύX
		fallFrame++;							//�����t���[�����Z
		fallSpeed += Gravity;					//�����X�s�[�h���Z
		fallDamage += FallDamageIncrease;		//�����_���[�W���Z
	}
	else
	{
		onGround = true;		//���n���Ă���ɕύX
		fallFrame = 0;			//�����t���[��������
		fallSpeed = 0.0f;		//�����X�s�[�h������
		fallDamage = 0;			//�����_���[�W������
	}

	//���܂ōs���Ȃ��悤��
	if (position.y - positionDistanceGround < 0.0f)
	{
		position.y = 0.0f + positionDistanceGround;
	}
}

/// <summary>
/// �J�v�Z���X�V
/// </summary>
void Player::UpdateCapsule()
{
	//�S�g
	VECTOR startVec = VTransform(VGet(0, 1, 0), rotateMatrix);
	wholebodyCapStart = VAdd(position, VScale(startVec, WholeBodyCapsuleHalfLength));
	wholebodyCapEnd = VSub(position, VScale(startVec, WholeBodyCapsuleHalfLength));
	
	//���S
	centerPosition = VAdd(wholebodyCapStart, wholebodyCapEnd);
	centerPosition = VScale(centerPosition, 0.5);

	//��
	footCapStart = VAdd(position, VGet(0.0f, -(WholeBodyCapsuleHalfLength + WholeBodyCapsuleRadius - FootCapsuleRadius + 1), 0.0f));
	footCapEnd = VAdd(position, VGet(0.0f, -(WholeBodyCapsuleHalfLength + WholeBodyCapsuleRadius - FootCapsuleRadius + 1), 0.0f));
}

/// <summary>
/// �����蔻����X�V
/// </summary>
void Player::UpdateCollisionData()
{
	//�S�g
	bodyCollisionData.tag = ObjectTag::PlayerWholeBody;
	bodyCollisionData.position = position;
	bodyCollisionData.startPosition = wholebodyCapStart;
	bodyCollisionData.endPosition = wholebodyCapEnd;
	bodyCollisionData.radius = WholeBodyCapsuleRadius;
	bodyCollisionData.isCollisionActive = true;

	//��
	footCollisionData.tag = ObjectTag::PlayerFoot;
	footCollisionData.position = position;
	footCollisionData.startPosition = footCapStart;
	footCollisionData.endPosition = footCapEnd;
	footCollisionData.radius = FootCapsuleRadius;
	footCollisionData.isCollisionActive = true;
}

/// <summary>
/// �`��ʒu�ݒ�(�ʒu�C���A��])
/// </summary>
void Player::DrawPositionSet()
{
	//��{
	drawPosition = position;
	drawPosition.y -= positionDistanceGround;
	MV1SetRotationMatrix(modelHandle, MGetIdent());		//��x���������ēK�����Ȃ���ΑO�̉�]�ɒǉ��ŉ�]����悤�ɂȂ�

	//�W�����v
	if (nowstateKind == State::Jump)
	{
		drawPosition.y += JumpDrowCorrectionY;
	}
	//�o��
	if (nowstateKind == State::Climb)
	{
		VECTOR axis = VNorm(VSub(wholebodyCapStart, wholebodyCapEnd));
		MATRIX modelRotate = MMult(rotateMatrix, MGetRotAxis(axis, angle));
		//��]�s�񃂃f���K��
		MV1SetRotationMatrix(modelHandle, modelRotate);
		//�`��ʒu�␳
		MV1SetPosition(modelHandle, drawPosition);		//��x�C���O�̃|�W�V�����ƃ{�[���̈ʒu�̍�����邽�߂ɔ��f
		VECTOR hipsFramePos = MV1GetFramePosition(modelHandle, PlayerStateProcessBase::PlayerFrameNumber::Hips);	//��������̃|�W�V����	
		VECTOR correctVec = VSub(position, hipsFramePos);	//�������聨�J�v�Z�����S�̃x�N�g�����A�j���[�V�����̂����␳
		drawPosition = VAdd(drawPosition, correctVec);		//�C�����f
	}

	//��]
	if (nowstateKind != State::Climb)
	{
		//-z�����ɐ��ʂ������悤�ɕ␳
		rotateMatrix = MGetRotY(angle + DX_PI_F);
		//��]�s�񃂃f���K��
		MV1SetRotationMatrix(modelHandle, rotateMatrix);
	}

}

/// <summary>
/// �Փˌ�̉����߂�
/// </summary>
/// <param name="hitObjectData">�I�u�W�F�N�g���</param>
void Player::CollisionPushBack(CollisionData *hitObjectData)
{
	for (int i = 0; i < hitObjectData->meshData.polygonList.PolygonNum; i++)
	{
		//�O�p�`���_
		VECTOR vertex0 = hitObjectData->meshData.polygonList.Vertexs[hitObjectData->meshData.polygonList.Polygons[i].VIndex[0]].Position;
		VECTOR vertex1 = hitObjectData->meshData.polygonList.Vertexs[hitObjectData->meshData.polygonList.Polygons[i].VIndex[1]].Position;
		VECTOR vertex2 = hitObjectData->meshData.polygonList.Vertexs[hitObjectData->meshData.polygonList.Polygons[i].VIndex[2]].Position;

		//�������_���łĂ����ꍇ��蒼��
		if (calculation->SameVector(vertex0, vertex1) || calculation->SameVector(vertex0, vertex2) || calculation->SameVector(vertex1, vertex2))
		{
			continue;
		}

		//�J�v�Z���ƎO�p�`�̓����蔻��
		bool bodyResult = HitCheck_Capsule_Triangle(wholebodyCapStart, wholebodyCapEnd, WholeBodyCapsuleRadius, vertex0, vertex1, vertex2);
		bool footResult = HitCheck_Capsule_Triangle(footCapStart, footCapEnd, FootCapsuleRadius, vertex0, vertex1, vertex2);

		//���������Ă���
		if (footResult)
		{
			onFootObject = true;
		}

		if (bodyResult)
		{
			//�f�[�^�R�s�[
			hitObjectDataPointer = hitObjectData;
			this->hitObjectData = *hitObjectData;

			//�@���x�N�g��
			VECTOR normVec = calculation->Normalize(vertex0, vertex1, vertex2);	

			//�͂߂�悤�ɂ���
			if (VDot(lookDirection, normVec) < 0)
			{
				isCatch = true;
			}
			
			//�����߂�
			VECTOR triangleClosest = VGet(0, 0, 0);
			VECTOR capsuleClosest = VGet(0, 0, 0);
			calculation->ClosestPointCapsuleAndTriangle(wholebodyCapStart, wholebodyCapEnd, vertex0, vertex1, vertex2, capsuleClosest, triangleClosest);

			//���������
			float distance = calculation->LengthTwoPoint3D(triangleClosest, capsuleClosest);
			
			//�����Ԃ��x�N�g��
			VECTOR pushBackVec = VSub(capsuleClosest, triangleClosest);
			pushBackVec = VNorm(pushBackVec);		
			pushBackVec = VScale(pushBackVec, WholeBodyCapsuleRadius - distance);

			//���������Ă����犊�藎���h�~
			if (onFootObject && pushBackVec.y < 0.0f)
			{
				pushBackVec.y = 0.0f;
			}
			
			//�|�W�V�������f
			if (nowstateKind == State::Run)
			{
				//moveVec��߂�
				position = VSub(position, moveVec);
				//�����Ԃ�
				position = VAdd(position, pushBackVec);
				//moveVec���߂荞�܂Ȃ��悤�Ƀ|���S���̕��ʂɉf��
				VECTOR slideVec;
				slideVec = VSub(moveVec, VScale(normVec, VDot(moveVec, normVec)));
				//������̃x�N�g��
				VECTOR upVec = FollowObjectUpMove(i, vertex0, vertex1, vertex2, *hitObjectData);
				//�|�W�V�������f
				position = VAdd(position, VAdd(slideVec, upVec));
			}
			else if (nowstateKind == State::Idle)
			{
				//������̃x�N�g��
				VECTOR upVec = FollowObjectUpMove(i, vertex0, vertex1, vertex2, *hitObjectData);
				//�|�W�V�������f
				position = VAdd(position, VAdd(pushBackVec, upVec));
			}
			else
			{
				//�|�W�V�������f
				position = VAdd(position, pushBackVec);
			}

			//�J�v�Z�����X�V
			rotateMatrix = MGetIdent();
			UpdateCapsule();
		}
	}
}

/// <summary>
/// �I�u�W�F�N�g�̏�����̓����ɒ����Ă���
/// </summary>
/// <param name="polygonNumber">�|���S���ԍ�</param>
/// <param name="vertex0">���_</param>
/// <param name="vertex1">���_</param>
/// <param name="vertex2">���_</param>
/// <param name="objectData">�I�u�W�F�N�g���</param>
/// <returns>������̃x�N�g��</returns>
VECTOR Player::FollowObjectUpMove(int polygonNumber, VECTOR vertex0, VECTOR vertex1, VECTOR vertex2, CollisionData objectData)
{
	//����̒��_
	VECTOR polygonCenter = calculation->TriangleCenter(vertex0, vertex1, vertex2);

	//�ŏ��̏ꍇ
	if (prevHitPolygonNumber == -1)
	{
		prevHitPolygonNumber = polygonNumber;
		prevHitPolygonCenter = polygonCenter;
	}
	//�O�o���Ă����|���S���̌��݂̃|�W�V����
	VECTOR prevVer0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevHitPolygonNumber].VIndex[0]].Position;
	VECTOR prevVer1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevHitPolygonNumber].VIndex[1]].Position;
	VECTOR prevVer2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevHitPolygonNumber].VIndex[2]].Position;
	//��L�̒��S�|�W�V����
	VECTOR nowPrevNumberCenterPos = calculation->TriangleCenter(prevVer0, prevVer1, prevVer2);
	//�|���S���̏�ɓ������ʂ��v�Z
	VECTOR resultVec = VSub(nowPrevNumberCenterPos, prevHitPolygonCenter);

	//�������ɃI�u�W�F�N�g���ړ������ꍇ�͒Ǐ]������
	if (resultVec.y <= 0.0f)
	{
		resultVec = VGet(0, 0, 0);
	}

	//�O�̃t���[���̏��Ƃ��ĕۑ�
	prevHitPolygonNumber = polygonNumber;
	prevHitPolygonCenter = polygonCenter;

	return resultVec;
}

/// <summary>
/// �������̏���
/// </summary>
void Player::MoveStateProcess()
{
	//�o�莞����
	if (nowstateKind == State::Climb)
	{
		gripPoint -= MinusGripPoint;
	}
	else
	{
		gripPoint += MinusGripPoint * 2;
		if (gripPoint > MaxGripPoint)
		{
			gripPoint = MaxGripPoint;
		}
	}

	//�˂��h���U����
	if (nowstateKind == State::Piercing)
	{
		onPiercingGauge = true;
		cameraZoom = nowstate->GetCameraZoom();
		piercingArmRotateZ = nowstate->GetArmRotateZ();
	}
	else
	{
		onPiercingGauge = false;
		cameraZoom = 0.0f;
	}

	//�W�����v��
	if (nowstateKind == State::Jump && !jumpAfterLeaveFoot && !onFootObject)
	{
		//��x�������ꂽ
		jumpAfterLeaveFoot = true;
	}
}

/// <summary>
/// �ǏՓˎ��̏���
/// </summary>
/// <param name="sinkIntoDepth">�߂荞�ݗ�</param>
void Player::WallHitProcess(VECTOR sinkIntoDepth)
{
	position = VAdd(position, sinkIntoDepth);
}

/// <summary>
/// �����Ɏg���f�[�^�̃Z�b�g
/// </summary>
void Player::MoveUseDataSet()
{
	moveUseData.inputState = inputstate;
	moveUseData.stickState = stickstate;
	moveUseData.position = position;
	moveUseData.capsuleStart = wholebodyCapStart;
	moveUseData.capsuleEnd = wholebodyCapEnd;
	moveUseData.centerPosition = centerPosition;
	moveUseData.capsuleRadius = WholeBodyCapsuleRadius;
	moveUseData.wholeBodyCapsuleHalfLength = WholeBodyCapsuleHalfLength;
	moveUseData.angle = angle;
	moveUseData.onFoot = onFootObject;
	moveUseData.lookDirection = lookDirection;
}

/// <summary>
/// �A�����͖h�~
/// </summary>
void Player::PreventionContinuousInput()
{
	//X
	if (canInputX && (Input::InputNumber::XButton & inputstate) == Input::InputNumber::XButton)
	{
		canInputX = false;
	}
	if (!canInputX && (Input::InputNumber::XButton & inputstate) != Input::InputNumber::XButton)
	{
		canInputX = true;
	}
	//A
	if (canInputA && (Input::InputNumber::AButton & inputstate) == Input::InputNumber::AButton)
	{
		canInputA= false;
	}
	if (!canInputA && (Input::InputNumber::AButton & inputstate) != Input::InputNumber::AButton)
	{
		canInputA = true;
	}
	
}

/// <summary>
/// �J�����ɓn�����߂̃|�W�V����
/// </summary>
/// <returns>�|�W�V����</returns>
VECTOR Player::GetPositionUseCamera()
{
	VECTOR returnVec;

	returnVec = position;
	returnVec.y -= positionDistanceGround;

	return returnVec;
}