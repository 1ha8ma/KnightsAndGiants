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
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
PlayerClimb::PlayerClimb(int modelHandle, VECTOR lookDir) :PlayerStateProcessBase(modelHandle)
{
	//アニメーションアタッチ
	nowPlayAnim = MV1AttachAnim(modelHandle, PlayerAnimationNumber:: Climb);
	//アニメーションの総再生時間を取る
	animTotalTime = MV1GetAnimTotalTime(modelHandle, nowPlayAnim);
	animTotalTime *= 2;

	//ファイル読み込み
	using Json = nlohmann::json;
	Json jsonData;
	std::ifstream ifs("Data/GameData/PlayerData.json");
	if (ifs)
	{
		ifs >> jsonData;
	}

	//インスタンス化
	calculation = new Calculation();

	//変数初期化
	calculation = new Calculation();
	stopAnimation = false;
	newLookDirection = lookDir;
	Speed = jsonData["ClimbMoveSpeed"];
	prevClimbPolygonNumber = -1;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerClimb::~PlayerClimb()
{
	DetachAnimation(nowPlayAnim);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerClimb::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="inputstate">入力状態</param>
/// <param name="stickstate">スティック入力情報</param>
/// <param name="camera">カメラ</param>
/// <param name="objectCapsuleStart">衝突しているカプセル始点</param>
/// <param name="objectCapsuleEnd">カプセル終点</param>
/// <returns>状態を変更するか</returns>
bool PlayerClimb::Update(UsePlayerData playerData, const Camera& camera,CollisionData objectCollision)
{
	bool stateChange = false;//状態変更フラグ

	//動き
	Move(playerData, camera,objectCollision);
	//アニメーション再生
	PlayAnimation(0.5f, stopAnimation);

	//R1を離すとステート変更
	if ((Input::InputNumber::R1 & playerData.inputState) != Input::InputNumber::R1)
	{
		stateChange = true;
	}

	return stateChange;
}

/// <summary>
///登り動作
/// </summary>
/// <param name="playerData">プレイヤー情報</param>
/// <param name="camera">カメラ</param>
/// <param name="objectData">オブジェクト情報</param>
void PlayerClimb::Move(UsePlayerData playerData, Camera camera, CollisionData objectData)
{
	//初期化
	moveVec = VGet(0, 0, 0);				
	stopAnimation = true;
	playerPosition = playerData.position;

	//近いポリゴンの番号を取る
	int nearPolygonNumber;				//最も近いポリゴン番号
	VECTOR vertex0, vertex1, vertex2;	//最も近いポリゴンの頂点
	nearPolygonNumber = NearPolygon(objectData);
	vertex0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[nearPolygonNumber].VIndex[0]].Position;
	vertex1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[nearPolygonNumber].VIndex[1]].Position;
	vertex2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[nearPolygonNumber].VIndex[2]].Position;

	//オブジェクトの移動に着いていく
	nearPolygonCenter = calculation->TriangleCenter(vertex0, vertex1, vertex2);
	VECTOR objectMoveVec = FollowObjectMove(nearPolygonNumber, objectData);

	//法線ベクトル
	VECTOR normVec = calculation->Normalize(vertex0, vertex1, vertex2);

	//入力からのmoveVec
	MoveVecForInput(playerData, normVec);

	//向いている方向更新
	newLookDirection = VScale(normVec, -1);

	//回転行列更新
	UpdateRotateMatrix(newLookDirection, playerData.lookDirection, normVec);

	//スピード加算
	moveVec = VScale(moveVec, Speed);

	//オブジェクトの動いた分加算
	moveVec = VAdd(moveVec, objectMoveVec);
}

/// <summary>
/// 確認用
/// </summary>
void PlayerClimb::Draw()
{
	//ポジションと登っているポリゴン中点
	DrawLine3D(nearPolygonCenter, playerPosition, GetColor(127, 255, 212));
}

/// <summary>
/// 入力からのmoveVec
/// </summary>
/// <param name="playerData">プレイヤー情報</param>
/// <param name="normVec">法線</param>
void PlayerClimb::MoveVecForInput(UsePlayerData playerData, VECTOR normVec)
{
	//入力からのベクトル作成
	VECTOR inputDir = VGet(0.0f, 0.0f, 0.0f);

	//上
	if (playerData.stickState.Y < 0.0f)
	{
		inputDir = VAdd(inputDir, VSub(playerData.capsuleStart, playerData.capsuleEnd));
	}
	//下
	if (playerData.stickState.Y > 0.0f)
	{
		inputDir = VAdd(inputDir, VSub(playerData.capsuleEnd, playerData.capsuleStart));
	}
	//右
	if (playerData.stickState.X > 0.0f)
	{
		VECTOR cross = VCross(VSub(playerData.capsuleStart, playerData.capsuleEnd), playerData.lookDirection);
		inputDir = VAdd(inputDir, cross);
	}
	//左
	if (playerData.stickState.X < 0.0f)
	{
		VECTOR cross = VCross(playerData.lookDirection, VSub(playerData.capsuleStart, playerData.capsuleEnd));
		inputDir = VAdd(inputDir, cross);
	}

	//入力があった場合
	if (VSize(inputDir) != 0)
	{
		//アニメーションを止めない
		stopAnimation = false;

		//平面を移動するように補正
		inputDir = VNorm(inputDir);
		moveVec = ProjectOnPlane(inputDir, normVec);
		moveVec = VNorm(moveVec);
	}
}

/// <summary>
/// 回転行列更新
/// </summary>
/// <param name="newLookDirection">新しい向いている方向</param>
/// <param name="prevLookDirection">前に向いていた方向</param>
void PlayerClimb::UpdateRotateMatrix(VECTOR newLookDirection, VECTOR prevLookDirection, VECTOR normVec)
{
	//回転軸
	VECTOR rotateAxis = VCross(VGet(0, 1, 0), normVec);
	//回転角度
	float dot = VDot(VGet(0, 1, 0), normVec);
	float angle = acos(dot) - DX_PI_F / 2;
	rotateMatrix = MGetRotAxis(rotateAxis, angle);
}

/// <summary>
/// 最も近い距離のポリゴンの番号を取る
/// </summary>
/// <param name="objectData">オブジェクト情報</param>
/// <returns>一番近いポリゴンの番号</returns>
int PlayerClimb::NearPolygon(const CollisionData objectData)
{
	float nearDistance;				//一番近い距離
	int nearPolygonNumber = -1;		//一番近いポリゴンの番号

	for (int i = 0; i < objectData.meshData.polygonList.PolygonNum; i++)
	{
		//頂点
		VECTOR vertex0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[i].VIndex[0]].Position;
		VECTOR vertex1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[i].VIndex[1]].Position;
		VECTOR vertex2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[i].VIndex[2]].Position;

		//距離を取る
		float distance = Triangle_Point_MinLength(vertex0, vertex1, vertex2, playerPosition);

		//一回目の処理
		if (i == 0)
		{
			nearDistance = distance;
			nearPolygonNumber = i;
		}

		//最も近いポリゴンとその距離
		if (nearDistance > distance)
		{
			nearDistance = distance;
			nearPolygonNumber = i;
		}
	}

	return nearPolygonNumber;
}

/// <summary>
/// 入力された情報をポリゴンの平面に映す
/// </summary>
/// <param name="inputDir">入力された方向</param>
/// <param name="polygonNorm">ポリゴン法線</param>
/// <returns>結果のベクトル</returns>
VECTOR PlayerClimb::ProjectOnPlane(VECTOR inputDir, VECTOR polygonNorm)
{
	float dot = VDot(inputDir, polygonNorm);
	return VSub(inputDir, VScale(polygonNorm, VDot(inputDir, polygonNorm)));
}

/// <summary>
/// オブジェクトの動きに着いていく
/// </summary>
/// <param name="nearPolygonNumber">一番近いポリゴンの番号</param>
/// <param name="objectData">オブジェクト情報</param>
/// <returns>オブジェクトの動いた量</returns>
VECTOR PlayerClimb::FollowObjectMove(int nearPolygonNumber,CollisionData objectData)
{
	//最初の場合
	if (prevClimbPolygonNumber == -1)
	{
		prevClimbPolygonNumber = nearPolygonNumber;
		prevClimbPolygonCenter = nearPolygonCenter;
	}
	//前登っていたポリゴンの現在のポジション
	VECTOR prevVer0 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevClimbPolygonNumber].VIndex[0]].Position;
	VECTOR prevVer1 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevClimbPolygonNumber].VIndex[1]].Position;
	VECTOR prevVer2 = objectData.meshData.polygonList.Vertexs[objectData.meshData.polygonList.Polygons[prevClimbPolygonNumber].VIndex[2]].Position;
	//上記の中心ポジション
	VECTOR nowPrevNumberCenterPos = calculation->TriangleCenter(prevVer0, prevVer1, prevVer2);
	//ポリゴンの動いた量を計算
	VECTOR resultVec = VSub(nowPrevNumberCenterPos, prevClimbPolygonCenter);
	//前のフレームの情報として保存
	prevClimbPolygonNumber = nearPolygonNumber;
	prevClimbPolygonCenter = nearPolygonCenter;

	return resultVec;
}