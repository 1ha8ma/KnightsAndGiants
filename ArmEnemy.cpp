#include<fstream>
#include"DxLib.h"
#include"nlohmann/json.hpp"
#include"Loader.h"
#include"Calculation.h"
#include"Camera.h"
#include"WeakPoint.h"
#include"ArmEnemyMoveBase.h"
#include"ArmEnemyIdle.h"
#include"ArmEnemyDropRock.h"
#include"ArmEnemySwing.h"
#include"ArmEnemyFallDown.h"
#include"ArmEnemyHandUp.h"
#include"ArmEnemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
ArmEnemy::ArmEnemy()
{
	//インスタンス化
	calclation = new Calculation();
	Loader* loader = loader->GetInstance();

	//モデルロード
	modelHandle = loader->GetHandle(Loader::Kind::ArmEnemyModel);

	//ファイル読み込み
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/ArmEnemyData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//変数初期化
	//ステータス初期化
	MaxHP = jsonData["MaxHP"];
	HP = MaxHP;
	//攻撃関係
	PlayerRideMoveStartFlame = jsonData["PlayerRideMoveStartFlame"];
	AttackCoolTime = jsonData["AttackCoolTime"];
	DropRockStartPlayerHeight = jsonData["DropRockStartPlayerHeight"];
	//ポジション関係
	position = VGet(jsonData["InitPositionX"], jsonData["InitPositionY"], jsonData["InitPositionZ"]);
	moveChangeflg = false;
	playerRideFlame = 0;
	isPlayerRide = false;
	playerRideMoveStartflg = false;
	attackCoolTimeFlame = 0;
	isAttackCoolTime = false;

	//描画モデル
	MV1SetScale(modelHandle, VGet(ModelScale, ModelScale, ModelScale));
	MV1SetRotationXYZ(modelHandle, ModelRotate);
	MV1SetPosition(modelHandle, position);

	//ポリゴン情報初期化
	InitializePolygonData();
	//あたり判定情報初期化
	capsuleStart = MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Upperarm);
	capsuleEnd = MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::LeftHandMiddle);
	capsuleRadius = CapsuleRadius;
	InitializeCollisionData();

	//部位当たり判定
	weakPoint = new WeakPoint(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::LeftHandMiddle - 1, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::LeftHandMiddle - 1, WeakPointRadius);

	//動き初期化
	move = new ArmEnemyIdle(modelHandle, VGet(0, 0, 0));
	nowMoveKind = MoveKind::Idle;
}

/// <summary>
/// デストラクタ
/// </summary>
ArmEnemy::~ArmEnemy()
{
	DeleteCollisionData();
	MV1ResetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Upperarm);
	MV1ResetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Forearm);
	MV1ResetFrameUserLocalMatrix(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Hand);
	delete weakPoint;
}

/// <summary>
/// 初期化
/// </summary>
void ArmEnemy::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerPos">プレイヤーポジション</param>
/// <param name="camera">カメラ</param>
/// <returns>死んでいるか</returns>
bool ArmEnemy::Update(VECTOR playerPos, Camera* camera)
{
	//死んでいるか
	bool isDead = false;

	//カプセル更新
	capsuleStart = MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Upperarm);
	capsuleEnd = MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::LeftHandMiddle);

	//プレイヤーが乗っていたら
	if (nowMoveKind != MoveKind::Swing && nowMoveKind != MoveKind::HandUp && isPlayerRide)
	{
		playerRideFlame++;

		if (playerRideFlame > PlayerRideMoveStartFlame)
		{
			playerRideFlame = 0;
			playerRideMoveStartflg = true;
		}
	}

	//弱点更新	
	weakPoint->Update();
	HP -= weakPoint->TakeDamage();

	//HP確認
	if (HP <= 0)
	{
		HP = 0;
		isDead = true;
	}

	//手をターゲットカメラに設定
	targetCameraPosition = MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Hand);

	//動き更新
	moveChangeflg = move->Update(camera, playerPos);
	if (!isAttackCoolTime && moveChangeflg && nowMoveKind != MoveKind::Idle && nowMoveKind != MoveKind::FallDown)
	{
		attackCoolTimeFlame = 0;
		isAttackCoolTime = true;
	}

	//攻撃が終わっている場合クールタイムを進める
	if (isAttackCoolTime && nowMoveKind != MoveKind::DropRock)
	{
		attackCoolTimeFlame++;

		if (attackCoolTimeFlame == AttackCoolTime)
		{
			isAttackCoolTime = false;
		}
	}

	//プレイヤーの近くの部位を判定
	CheckPlayerNearParts(playerPos);

	//動きの変更確認
	ChangeMove(playerPos);

	//ポジション反映
	MV1SetPosition(modelHandle, position);

	//メッシュ情報更新
	MV1SetupReferenceMesh(modelHandle, -1, TRUE);
	polygonList = MV1GetReferenceMesh(modelHandle, -1, TRUE);

	//あたり判定情報更新
	UpdateCollisionData();

	//プレイヤーの乗っている関係初期化
	isPlayerRide = false;

	return isDead;
}

/// <summary>
/// 衝突後の処理
/// </summary>
/// <param name="hitObjectData">衝突したオブジェクトの情報</param>
void ArmEnemy::OnHitObject(CollisionData* hitObjectData)
{
	//プレイヤーが乗っている時の処理
	if (hitObjectData->tag == ObjectTag::PlayerWholeBody ||
		hitObjectData->tag == ObjectTag::PlayerFoot)
	{
		isPlayerRide = true;
	}

	//プレイヤーの攻撃だった場合
	if (hitObjectData->tag == ObjectTag::Attack_P)
	{
		HP -= hitObjectData->attackPower;
	}
}

/// <summary>
/// 倒された後の初期化
/// </summary>
void ArmEnemy::InitializeFallDown()
{
	VECTOR prevRotate = move->GetRotate();
	delete move;
	nowMoveKind = MoveKind::FallDown;
	move = new ArmEnemyFallDown(modelHandle);
}

/// <summary>
/// 倒された後の更新
/// </summary>
/// <returns>動きが終わったか</returns>
bool ArmEnemy::UpdateFallDown(Camera* camera)
{
	bool moveEnd = false;
	moveEnd = move->UpdateFallDown(camera);

	//パーツ、エフェクトの更新
	weakPoint->Update();

	//手をターゲットカメラに設定
	targetCameraPosition = MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Hand);

	MV1SetPosition(modelHandle, position);
	return moveEnd;
}

/// <summary>
/// 描画
/// </summary>
void ArmEnemy::Draw()
{
	MV1DrawModel(modelHandle);
	weakPoint->Draw();
	move->Draw();

	//確認用
	/*DrawPolygon();
	DrawCapsule();*/
}

/// <summary>
/// プレイヤーが腕のどの部位に近いか判定
/// </summary>
/// <param name="playerPos">プレイヤーポジション</param>
void ArmEnemy::CheckPlayerNearParts(VECTOR playerPos)
{
	float nearDistance = -1;		//一番近い距離
	std::vector<VECTOR> partsPos;	//部位ポジション

	//ポジション設定
	partsPos.push_back(VScale(VAdd(MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Upperarm), MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Forearm)), 0.5f));	//上腕
	partsPos.push_back(VScale(VAdd(MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Forearm), MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Hand)), 0.5f));		//前腕
	partsPos.push_back(VScale(VAdd(MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Hand), MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::LeftHandMiddle)), 0.5f));	//手

	//距離判定
	for (int i = 0; i < PartsNum; i++)
	{
		float distance = calclation->LengthTwoPoint3D(playerPos, partsPos[i]);
		if (nearDistance > distance || nearDistance == -1)
		{
			nearDistance = distance;
			playerRidePlace = i;
		}
	}

	//リストクリア
	partsPos.clear();
}

/// <summary>
/// 動きの変更
/// </summary>
/// <param name="playerPos">プレイヤーポジション</param>
void ArmEnemy::ChangeMove(VECTOR playerPos)
{
	//手のポジション
	VECTOR handpos = MV1GetFramePosition(modelHandle, (int)ArmEnemyMoveBase::ArmEnemyFrameIndex::Hand);

	//待機
	if (nowMoveKind == MoveKind::DropRock && moveChangeflg ||
		nowMoveKind == MoveKind::Swing && moveChangeflg ||
		nowMoveKind == MoveKind::HandUp && moveChangeflg
		)
	{
		//プレイヤーが乗っている時の動きのフラグを初期化
		if (nowMoveKind == MoveKind::Swing || nowMoveKind == MoveKind::HandUp)
		{
			playerRideMoveStartflg = false;
		}
		VECTOR prevRotate = move->GetRotate();
		delete move;
		nowMoveKind = MoveKind::Idle;
		move = new ArmEnemyIdle(modelHandle, prevRotate);
	}

	//岩落とし
	if (nowMoveKind == MoveKind::Idle && !isPlayerRide && playerPos.y < handpos.y && playerPos.y>DropRockStartPlayerHeight && !isAttackCoolTime)
	{
		VECTOR prevRotate = move->GetRotate();
		delete move;
		nowMoveKind = MoveKind::DropRock;
		move = new ArmEnemyDropRock(modelHandle, prevRotate);
	}

	//振り回し...プレイヤーが上腕に乗っている場合
	if (nowMoveKind != MoveKind::Swing && nowMoveKind != MoveKind::HandUp && playerRideMoveStartflg && playerRidePlace == (int)PartsName::Upperarm && isAttackCoolTime)
	{
		VECTOR prevRotate = move->GetRotate();
		delete move;
		nowMoveKind = MoveKind::Swing;
		move = new ArmEnemySwing(modelHandle, prevRotate);
	}

	//腕を上げる...プレイヤーが手に乗っている場合
	if (nowMoveKind != MoveKind::HandUp && nowMoveKind != MoveKind::Swing && playerRideMoveStartflg && playerRidePlace == (int)PartsName::Hand && isAttackCoolTime)
	{
		VECTOR prevRotate = move->GetRotate();
		delete move;
		nowMoveKind = MoveKind::HandUp;
		move = new ArmEnemyHandUp(modelHandle, prevRotate);
	}
}