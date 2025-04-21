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
/// コンストラクタ
/// </summary>
Player::Player()
{
	Loader* loader = loader->GetInstance();

	modelHandle = loader->GetHandle(Loader::Kind::PlayerModel);

	InitializeStartScene();
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	collisionManager->RemoveCollisionData(&bodyCollisionData);
	collisionManager->RemoveCollisionData(&footCollisionData);
	delete input;
	delete nowstate;
}

/// <summary>
/// スタートシーン初期化
/// </summary>
void Player::InitializeStartScene()
{
	//ファイル読み込み
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
/// ゲームシーン初期化
/// </summary>
void Player::InitializeGame()
{
	//インスタンス化
	nowstate = new PlayerIdle(modelHandle, VGet(0.0f, 0.0f, 0.0f));
	input = new Input();
	calculation = new Calculation();

	//ファイル読み込み
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//変数初期化
	//ステータス
	MaxHP = jsonData["PlayerMaxHP"];
	HP = MaxHP;
	MaxGripPoint = jsonData["MaxGripPoint"];
	gripPoint = MaxGripPoint;
	//入力
	canInputX = false;
	canInputA = false;
	//モデル関係
	drawPosition = position;
	lookDirection = VGet(0, 0, 1);
	//あたり判定
	positionDistanceGround = jsonData["PositionDistanceGround"];
	position = VGet(jsonData["InitPositionX"], positionDistanceGround, jsonData["InitPositionZ"]);
	WholeBodyCapsuleRadius = jsonData["WholeBodyCapsuleRadius"];
	WholeBodyCapsuleHalfLength = jsonData["WholeBodyCapsuleHalfLength"];
	FootCapsuleRadius = jsonData["FootCapsuleRadius"];

	//移動関係
	angle = 0.0f;
	changeAngleSpeed = jsonData["ChangeAngleSpeed"];
	jumpAfterLeaveFoot = false;
	onGround = true;
	onFootObject = false;
	isCatch = false;
	//動き関係
	nowstateKind = State::Run;
	isChangeState = false;
	//落下関係
	fallSpeed = 0.0f;
	fallFrame = 0;
	fallDamage = 0;
	FallDamageIncrease = jsonData["FallDamageIncrease"];
	Gravity = jsonData["Gravity"];
	//突き刺し攻撃
	cameraZoom = 0.0f;
	piercingArmRotateZ = 0.0f;
	onPiercingGauge = false;
	//登り
	MinusGripPoint = jsonData["MinusGripPoint"];
	//その他
	isGameOver = false;

	//当たり判定
	collisionManager = collisionManager->GetInstance();
	//衝突後の処理を渡す
	bodyCollisionData.HitProcess = std::bind(&Player::BodyOnHitObject, this, std::placeholders::_1);
	bodyCollisionData.WallHitProcess = std::bind(&Player::WallHitProcess, this, std::placeholders::_1);
	footCollisionData.HitProcess = std::bind(&Player::FootOnHitObject, this, std::placeholders::_1);
	//当たり判定に必要なデータを渡す
	collisionManager->AddCollisionData(&bodyCollisionData);
	collisionManager->AddCollisionData(&footCollisionData);

	//ポジション反映
	MV1SetPosition(modelHandle, drawPosition);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="camera">カメラ</param>
/// <returns>ゲームオーバー</returns>
bool Player::UpdateGame(Camera* camera)
{
	//入力
	inputstate = input->GetInputState();
	stickstate = input->GetStickInput();

	//入力によって動き変更
	ChangeState();

	//動き更新に必要な情報を渡す
	MoveUseDataSet();

	//動き更新
	CollisionData latestData = hitObjectData;	
	if (nowstateKind == State::Climb)
	{
		//最新のデータを取得
		latestData = *hitObjectDataPointer;
	}
	isChangeState = nowstate->Update(moveUseData, *camera, latestData);
	rotateMatrix = nowstate->GetRotateMatrix();
	lookDirection = nowstate->GetLookDirection();
	lookDirection = VNorm(lookDirection);

	//移動
	moveVec = nowstate->GetmoveVec();
	//落下速度追加
	moveVec = VSub(moveVec, VGet(0.0f, fallSpeed, 0.0f));
	//ポジション反映
	position = VAdd(position, moveVec);
	//足がついているか確認
	CheckOnGround(camera);

	//角度更新
	UpdateAngle();

	//動き毎の処理
	MoveStateProcess();

	//描画位置設定
	DrawPositionSet();

	//連続入力防止
	PreventionContinuousInput();

	//ゲームオーバー確認
	if (HP <= 0)
	{
		isGameOver = true;
	}

	//カプセル等更新
	UpdateCapsule();
	UpdateCollisionData();

	//描画ポジション設定
	MV1SetPosition(modelHandle, drawPosition);

	//変数初期化
	isCatch = false;
	onFootObject = false;
	
	return isGameOver;
}

/// <summary>
/// ゲームオーバーシーン初期化
/// </summary>
void Player::InitializeGameOver()
{
	delete nowstate;
	nowstateKind = State::FallDown;
	nowstate = new PlayerFallDown(modelHandle);
}

/// <summary>
/// ゲームオーバーシーン更新
/// </summary>
void Player::UpdateGameOver()
{
	nowstate->UpdateGameOver();
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	MV1DrawModel(modelHandle);

	//確認用
	//DrawCapsule3D(bodyCollisionData.startPosition, bodyCollisionData.endPosition, WholeBodyCapsuleRadius, 8, GetColor(220, 20, 60),GetColor(220,20,60), FALSE);
	//nowstate->Draw();	
	//DrawLine3D(position, VAdd(position, VScale(lookDirection, 200)), GetColor(127, 255, 0));							//lookDirection
	//DrawLine3D(position, wholebodyCapStart, GetColor(220, 20, 60));		//真ん中から頭
	//DrawCapsule3D(footCapStart, footCapEnd, FootCapsuleRadius, 8, GetColor(220, 20, 60), GetColor(220, 20, 60), FALSE);	//足
}

/// <summary>
/// 衝突時の処理
/// </summary>
/// <param name="hitObjectData">衝突したオブジェクト</param>
void Player::BodyOnHitObject(CollisionData* hitObjectData)
{
	//衝突したオブジェクトごとに処理を変更
	//敵の攻撃
	if (hitObjectData->tag == ObjectTag::EnemyAttack)
	{
		//HP減少
		HP -= hitObjectData->attackPower;

		if (HP < 0)
		{
			HP = 0;
		}
	}

	//カプセル
	if (hitObjectData->tag == ObjectTag::StageObject ||
		hitObjectData->tag == ObjectTag::Enemy)
	{
		//押し戻し
		if (nowstateKind != State::Climb)
		{
			CollisionPushBack(hitObjectData);
		}
	}
}

/// <summary>
/// オブジェクト衝突時の処理(足)
/// </summary>
/// <param name="hitObjectData">衝突したオブジェクト</param>
void Player::FootOnHitObject(CollisionData* hitObjectData)
{
	if (hitObjectData->tag == ObjectTag::StageObject ||
		hitObjectData->tag == ObjectTag::Enemy)
	{
		//処理なし
	}
}

/// <summary>
/// 角度更新(水平方向の角度のみ)
/// </summary>
void Player::UpdateAngle()
{
	//移動方向にモデルの方向を近づける
	float targetAngle;		//目標の角度
	float difference;		//目標角度と現在の角度の差

	//目標の方向ベクトルから角度値を算出する
	targetAngle = static_cast<float>(atan2(lookDirection.x, lookDirection.z));

	//目標の角度と現在の角度との差を割り出す
	//最初は引き算
	difference = targetAngle - angle;

	//ある方向からある方向の差が180度以上になることはないので差が180度以上になっていたら修正する
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	//角度の差を0に近づける
	if (difference > 0.0f)//差がマイナスの場合
	{
		difference -= changeAngleSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else//差がプラスの場合
	{
		difference += changeAngleSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	//モデルの角度を更新
	angle = targetAngle - difference;
}

/// <summary>
/// ステート変更
/// </summary>
void Player::ChangeState()
{
	//待機
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

	//走る
	if (nowstateKind != State::Run && nowstateKind != State::Climb && nowstateKind != State::NormalAttack && nowstateKind != State::Piercing && nowstateKind != State::Squat && nowstateKind != State::Jump && onGround && (stickstate.X != 0.0f || stickstate.Y != 0.0f))
	{
		delete nowstate;
		nowstateKind = State::Run;
		nowstate = new PlayerRun(modelHandle, lookDirection);
		prevHitPolygonNumber = -1;
	}

	//ジャンプ
	if (nowstateKind != State::Jump && nowstateKind != State::NormalAttack && nowstateKind != State::Climb && canInputA && (Input::InputNumber::AButton & inputstate) == Input::InputNumber::AButton)
	{
		delete nowstate;
		onGround = false;
		jumpAfterLeaveFoot = false;
		nowstateKind = State::Jump;
		nowstate = new PlayerJump(modelHandle, moveVec, lookDirection);
	}

	//突き刺し攻撃
	if (nowstateKind == State::Squat && canInputX && (Input::InputNumber::XButton & inputstate) == Input::InputNumber::XButton)
	{
		delete nowstate;
		nowstateKind = State::Piercing;
		nowstate = new PlayerPiercing(modelHandle, lookDirection);
	}

	//通常攻撃　(突き刺し攻撃の後に判定しないと一度通常攻撃に入ってしまう)
	if (nowstateKind != State::NormalAttack && nowstateKind != State::Piercing && nowstateKind != State::Jump && nowstateKind != State::Squat && nowstateKind != State::Climb && canInputX && (Input::InputNumber::XButton & inputstate) == Input::InputNumber::XButton)
	{
		delete nowstate;
		nowstateKind = State::NormalAttack;
		nowstate = new PlayerNormalAttack(modelHandle, lookDirection);
	}

	//掴まり
	if (nowstateKind != State::Climb && isCatch && !onGround && (Input::InputNumber::R1 & inputstate) == Input::InputNumber::R1 && gripPoint > 20)
	{
		delete nowstate;
		nowstateKind = State::Climb;
		nowstate = new PlayerClimb(modelHandle, lookDirection);
	}

	//しゃがみ
	if (onGround && nowstateKind != State::Piercing && nowstateKind != State::Squat && nowstateKind != State::Climb && !isCatch && (Input::InputNumber::R1 & inputstate) == Input::InputNumber::R1)
	{
		delete nowstate;
		nowstateKind = State::Squat;
		nowstate = new PlayerSquat(modelHandle, lookDirection);
	}

	//落下(一定フレーム以上落ちている場合に移行)
	if (fallFrame > 25 && !onGround && nowstateKind != State::Falling && nowstateKind != State::Jump && nowstateKind != State::Climb)
	{
		delete nowstate;
		nowstateKind = State::Falling;
		nowstate = new PlayerFalling(modelHandle, lookDirection);
	}
}

/// <summary>
/// 足が着いているか確認
/// </summary>
/// <param name="camera">カメラ</param>
void Player::CheckOnGround(Camera* camera)
{
	//着地したとき
	if ((nowstateKind == State::Jump && jumpAfterLeaveFoot && (!onGround && position.y - positionDistanceGround < 0.0f || !onGround && onFootObject)) ||
		nowstateKind != State::Jump && (!onGround && position.y - positionDistanceGround < 0.0f || !onGround && onFootObject))
	{
		onGround = true;
		moveVec.y = 0.0f;

		//一定時間落下していたら落下ダメージ
		if (fallFrame >= FallDamageStartFrame)
		{
			HP -= fallDamage;
			//カメラ振動
			camera->PlayShakingVertical(FallDamageCameraShakingPower, FallDamageCameraShakingDirChangeflame, FallDamageCameraShakingPlayflame);
			//振動
			StartJoypadVibration(DX_INPUT_PAD1, FallDamageJoypadVibPower, FallDamageJoypadVibPlayflame, -1);
		}
	}

	//足が着いていなければ
	if (position.y - positionDistanceGround >= 0.0f && !onFootObject && nowstateKind != State::Climb)
	{
		onGround = false;						//着地していないに変更
		fallFrame++;							//落下フレーム加算
		fallSpeed += Gravity;					//落下スピード加算
		fallDamage += FallDamageIncrease;		//落下ダメージ加算
	}
	else
	{
		onGround = true;		//着地しているに変更
		fallFrame = 0;			//落下フレーム初期化
		fallSpeed = 0.0f;		//落下スピード初期化
		fallDamage = 0;			//落下ダメージ初期化
	}

	//下まで行かないように
	if (position.y - positionDistanceGround < 0.0f)
	{
		position.y = 0.0f + positionDistanceGround;
	}
}

/// <summary>
/// カプセル更新
/// </summary>
void Player::UpdateCapsule()
{
	//全身
	VECTOR startVec = VTransform(VGet(0, 1, 0), rotateMatrix);
	wholebodyCapStart = VAdd(position, VScale(startVec, WholeBodyCapsuleHalfLength));
	wholebodyCapEnd = VSub(position, VScale(startVec, WholeBodyCapsuleHalfLength));
	
	//中心
	centerPosition = VAdd(wholebodyCapStart, wholebodyCapEnd);
	centerPosition = VScale(centerPosition, 0.5);

	//足
	footCapStart = VAdd(position, VGet(0.0f, -(WholeBodyCapsuleHalfLength + WholeBodyCapsuleRadius - FootCapsuleRadius + 1), 0.0f));
	footCapEnd = VAdd(position, VGet(0.0f, -(WholeBodyCapsuleHalfLength + WholeBodyCapsuleRadius - FootCapsuleRadius + 1), 0.0f));
}

/// <summary>
/// 当たり判定情報更新
/// </summary>
void Player::UpdateCollisionData()
{
	//全身
	bodyCollisionData.tag = ObjectTag::PlayerWholeBody;
	bodyCollisionData.position = position;
	bodyCollisionData.startPosition = wholebodyCapStart;
	bodyCollisionData.endPosition = wholebodyCapEnd;
	bodyCollisionData.radius = WholeBodyCapsuleRadius;
	bodyCollisionData.isCollisionActive = true;

	//足
	footCollisionData.tag = ObjectTag::PlayerFoot;
	footCollisionData.position = position;
	footCollisionData.startPosition = footCapStart;
	footCollisionData.endPosition = footCapEnd;
	footCollisionData.radius = FootCapsuleRadius;
	footCollisionData.isCollisionActive = true;
}

/// <summary>
/// 描画位置設定(位置修正、回転)
/// </summary>
void Player::DrawPositionSet()
{
	//基本
	drawPosition = position;
	drawPosition.y -= positionDistanceGround;
	MV1SetRotationMatrix(modelHandle, MGetIdent());		//一度初期化して適応しなければ前の回転に追加で回転するようになる

	//ジャンプ
	if (nowstateKind == State::Jump)
	{
		drawPosition.y += JumpDrowCorrectionY;
	}
	//登り
	if (nowstateKind == State::Climb)
	{
		VECTOR axis = VNorm(VSub(wholebodyCapStart, wholebodyCapEnd));
		MATRIX modelRotate = MMult(rotateMatrix, MGetRotAxis(axis, angle));
		//回転行列モデル適応
		MV1SetRotationMatrix(modelHandle, modelRotate);
		//描画位置補正
		MV1SetPosition(modelHandle, drawPosition);		//一度修正前のポジションとボーンの位置の差を取るために反映
		VECTOR hipsFramePos = MV1GetFramePosition(modelHandle, PlayerStateProcessBase::PlayerFrameNumber::Hips);	//腰あたりのポジション	
		VECTOR correctVec = VSub(position, hipsFramePos);	//腰あたり→カプセル中心のベクトル分アニメーションのずれを補正
		drawPosition = VAdd(drawPosition, correctVec);		//修正反映
	}

	//回転
	if (nowstateKind != State::Climb)
	{
		//-z方向に正面を向くように補正
		rotateMatrix = MGetRotY(angle + DX_PI_F);
		//回転行列モデル適応
		MV1SetRotationMatrix(modelHandle, rotateMatrix);
	}

}

/// <summary>
/// 衝突後の押し戻し
/// </summary>
/// <param name="hitObjectData">オブジェクト情報</param>
void Player::CollisionPushBack(CollisionData *hitObjectData)
{
	for (int i = 0; i < hitObjectData->meshData.polygonList.PolygonNum; i++)
	{
		//三角形頂点
		VECTOR vertex0 = hitObjectData->meshData.polygonList.Vertexs[hitObjectData->meshData.polygonList.Polygons[i].VIndex[0]].Position;
		VECTOR vertex1 = hitObjectData->meshData.polygonList.Vertexs[hitObjectData->meshData.polygonList.Polygons[i].VIndex[1]].Position;
		VECTOR vertex2 = hitObjectData->meshData.polygonList.Vertexs[hitObjectData->meshData.polygonList.Polygons[i].VIndex[2]].Position;

		//同じ頂点がでてきた場合やり直し
		if (calculation->SameVector(vertex0, vertex1) || calculation->SameVector(vertex0, vertex2) || calculation->SameVector(vertex1, vertex2))
		{
			continue;
		}

		//カプセルと三角形の当たり判定
		bool bodyResult = HitCheck_Capsule_Triangle(wholebodyCapStart, wholebodyCapEnd, WholeBodyCapsuleRadius, vertex0, vertex1, vertex2);
		bool footResult = HitCheck_Capsule_Triangle(footCapStart, footCapEnd, FootCapsuleRadius, vertex0, vertex1, vertex2);

		//足が着いている
		if (footResult)
		{
			onFootObject = true;
		}

		if (bodyResult)
		{
			//データコピー
			hitObjectDataPointer = hitObjectData;
			this->hitObjectData = *hitObjectData;

			//法線ベクトル
			VECTOR normVec = calculation->Normalize(vertex0, vertex1, vertex2);	

			//掴めるようにする
			if (VDot(lookDirection, normVec) < 0)
			{
				isCatch = true;
			}
			
			//押し戻し
			VECTOR triangleClosest = VGet(0, 0, 0);
			VECTOR capsuleClosest = VGet(0, 0, 0);
			calculation->ClosestPointCapsuleAndTriangle(wholebodyCapStart, wholebodyCapEnd, vertex0, vertex1, vertex2, capsuleClosest, triangleClosest);

			//距離を取る
			float distance = calculation->LengthTwoPoint3D(triangleClosest, capsuleClosest);
			
			//押し返しベクトル
			VECTOR pushBackVec = VSub(capsuleClosest, triangleClosest);
			pushBackVec = VNorm(pushBackVec);		
			pushBackVec = VScale(pushBackVec, WholeBodyCapsuleRadius - distance);

			//足が着いていたら滑り落ち防止
			if (onFootObject && pushBackVec.y < 0.0f)
			{
				pushBackVec.y = 0.0f;
			}
			
			//ポジション反映
			if (nowstateKind == State::Run)
			{
				//moveVecを戻す
				position = VSub(position, moveVec);
				//押し返す
				position = VAdd(position, pushBackVec);
				//moveVecをめり込まないようにポリゴンの平面に映す
				VECTOR slideVec;
				slideVec = VSub(moveVec, VScale(normVec, VDot(moveVec, normVec)));
				//上方向のベクトル
				VECTOR upVec = FollowObjectUpMove(i, vertex0, vertex1, vertex2, *hitObjectData);
				//ポジション反映
				position = VAdd(position, VAdd(slideVec, upVec));
			}
			else if (nowstateKind == State::Idle)
			{
				//上方向のベクトル
				VECTOR upVec = FollowObjectUpMove(i, vertex0, vertex1, vertex2, *hitObjectData);
				//ポジション反映
				position = VAdd(position, VAdd(pushBackVec, upVec));
			}
			else
			{
				//ポジション反映
				position = VAdd(position, pushBackVec);
			}

			//カプセルを更新
			rotateMatrix = MGetIdent();
			UpdateCapsule();
		}
	}
}

/// <summary>
/// オブジェクトの上方向の動きに着いていく
/// </summary>
/// <param name="polygonNumber">ポリゴン番号</param>
/// <param name="vertex0">頂点</param>
/// <param name="vertex1">頂点</param>
/// <param name="vertex2">頂点</param>
/// <param name="objectData">オブジェクト情報</param>
/// <returns>上方向のベクトル</returns>
VECTOR Player::FollowObjectUpMove(int polygonNumber, VECTOR vertex0, VECTOR vertex1, VECTOR vertex2, CollisionData objectData)
{
	//今回の中点
	VECTOR polygonCenter = calculation->TriangleCenter(vertex0, vertex1, vertex2);

	//最初の場合
	if (prevHitPolygonNumber == -1)
	{
		prevHitPolygonNumber = polygonNumber;
		prevHitPolygonCenter = polygonCenter;
	}
	//前登っていたポリゴンの現在のポジション
	VECTOR prevVer0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevHitPolygonNumber].VIndex[0]].Position;
	VECTOR prevVer1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevHitPolygonNumber].VIndex[1]].Position;
	VECTOR prevVer2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevHitPolygonNumber].VIndex[2]].Position;
	//上記の中心ポジション
	VECTOR nowPrevNumberCenterPos = calculation->TriangleCenter(prevVer0, prevVer1, prevVer2);
	//ポリゴンの上に動いた量を計算
	VECTOR resultVec = VSub(nowPrevNumberCenterPos, prevHitPolygonCenter);

	//下方向にオブジェクトが移動した場合は追従を消す
	if (resultVec.y <= 0.0f)
	{
		resultVec = VGet(0, 0, 0);
	}

	//前のフレームの情報として保存
	prevHitPolygonNumber = polygonNumber;
	prevHitPolygonCenter = polygonCenter;

	return resultVec;
}

/// <summary>
/// 動き毎の処理
/// </summary>
void Player::MoveStateProcess()
{
	//登り時握力
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

	//突き刺し攻撃時
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

	//ジャンプ時
	if (nowstateKind == State::Jump && !jumpAfterLeaveFoot && !onFootObject)
	{
		//一度足が離れた
		jumpAfterLeaveFoot = true;
	}
}

/// <summary>
/// 壁衝突時の処理
/// </summary>
/// <param name="sinkIntoDepth">めり込み量</param>
void Player::WallHitProcess(VECTOR sinkIntoDepth)
{
	position = VAdd(position, sinkIntoDepth);
}

/// <summary>
/// 動きに使うデータのセット
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
/// 連続入力防止
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
/// カメラに渡すためのポジション
/// </summary>
/// <returns>ポジション</returns>
VECTOR Player::GetPositionUseCamera()
{
	VECTOR returnVec;

	returnVec = position;
	returnVec.y -= positionDistanceGround;

	return returnVec;
}