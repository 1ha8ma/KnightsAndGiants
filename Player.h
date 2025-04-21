#pragma once
#include"PlayerStateProcessBase.h"
#include"CollisionData.h"
#include"CollisionManager.h"

class Input;
class Camera;
class Calculation;

class Player
{
public:
	Player();
	~Player();

	//スタートシーン初期化
	void InitializeStartScene();
	//スタートシーン終了処理
	void StartSceneEnd() { delete nowstate; }

	//ゲーム初期化
	void InitializeGame();
	//ゲーム更新
	bool UpdateGame(Camera* camera);

	//ゲームオーバーシーン初期化
	void InitializeGameOver();
	//ゲームオーバーシーン更新
	void UpdateGameOver();
	//描画
	void Draw();

	//衝突した時の処理
	void BodyOnHitObject(CollisionData* hitObjectData);
	void FootOnHitObject(CollisionData* hitObjectData);
	//壁衝突時の処理
	void WallHitProcess(VECTOR sinkIntoDepth);
	//動き毎の処理
	void MoveStateProcess();

	//GetSet
	VECTOR GetPosition() { return position; }
	VECTOR GetPositionUseCamera();
	float GetAngle() { return angle; }
	int GetHP() { return HP; }
	int GetGripPoint() { return gripPoint; }
	float GetCameraZoom() { return cameraZoom; }
	float GetPiercingArmRotateZ() { return piercingArmRotateZ; }
	bool GetOnPiercingGauge() { return onPiercingGauge; }

private:
	//状態
	enum class State :int
	{
		Idle,			//待機
		Run,			//走る
		Jump,			//ジャンプ
		NormalAttack,	//通常攻撃
		Climb,			//登り
		Squat,			//しゃがみ
		Piercing,		//突き刺し攻撃
		FallDown,		//倒れる(負けた時)
		Falling,		//落下
	};

	const float FallDamageCameraShakingPower = 0.8f;				//落下ダメージカメラ揺れ強さ
	const int FallDamageCameraShakingDirChangeflame = 10;			//落下ダメージカメラの揺れの上下を切り替えるフレーム
	const int FallDamageCameraShakingPlayflame = 20;				//落下ダメージ揺れ再生フレーム
	const int FallDamageJoypadVibPower = 500;						//落下ダメージコントローラー揺れ強さ
	const int FallDamageJoypadVibPlayflame = 100;					//落下ダメージコントローラー揺れフレーム
	const int FallDamageStartFrame = 100;							//落下ダメージ加算開始フレーム
	const float JumpDrowCorrectionY = -60.0f;						//ジャンプ時描画位置修正

	//角度更新
	void UpdateAngle();
	//ステート変更
	void ChangeState();
	//足がついているか確認
	void CheckOnGround(Camera* camera);
	//カプセル更新
	void UpdateCapsule();
	//当たり判定情報更新
	void UpdateCollisionData();
	//描画位置設定
	void DrawPositionSet();
	//押し戻し
	void CollisionPushBack(CollisionData *hitCollisionData);
	//オブジェクトの上の動きに着いていく
	VECTOR FollowObjectUpMove(int polygonNumber, VECTOR vertex0, VECTOR vertex1, VECTOR vertex2, CollisionData objectData);
	//連続入力防止
	void PreventionContinuousInput();
	//動きに使う構造体の中身のセット
	void MoveUseDataSet();
	
	//他クラス
	PlayerStateProcessBase* nowstate;
	Input* input;
	CollisionManager* collisionManager;
	CollisionData bodyCollisionData;
	CollisionData footCollisionData;
	CollisionData hitObjectData;
	CollisionData* hitObjectDataPointer;
	PlayerStateProcessBase::UsePlayerData moveUseData;	//動きに使うデータ構造体
	Calculation* calculation;

	//ステータス
	int MaxHP;			//最大体力
	int HP;				//体力
	int MaxGripPoint;	//最大握力
	int gripPoint;		//握力ゲージ

	//入力
	int inputstate;									//入力情報
	DINPUT_JOYSTATE stickstate;						//スティック入力情報
	bool canInputX;									//攻撃連続入力防止
	bool canInputA;

	//モデル関係
	int modelHandle;								//モデルハンドル
	VECTOR drawPosition;							//描画用ポジション
	MATRIX rotateMatrix;							//モデル回転行列
	VECTOR lookDirection;							//モデルの向いている方向

	//当たり判定
	float WholeBodyCapsuleRadius;					//全身カプセル半径
	float WholeBodyCapsuleHalfLength;				//カプセルの半分の長さ
	float FootCapsuleRadius;						//足カプセル半径
	float positionDistanceGround;					//ポジションと地面の距離
	VECTOR position;								//ポジション
	VECTOR wholebodyCapStart;						//カプセル始点(上)
	VECTOR wholebodyCapEnd;							//カプセル終点(下)
	VECTOR centerPosition;							//カプセルの真ん中
	VECTOR footCapStart;							//足カプセル
	VECTOR footCapEnd;								//足カプセル

	//移動関係
	float angle;									//プレイヤーの向き
	float changeAngleSpeed;							//角度変更速度
	VECTOR moveVec;									//移動量
	bool onGround;									//足がついているか
	bool onFootObject;								//足がオブジェクトに着いている
	bool isCatch;									//掴める状態か
	int prevHitPolygonNumber;						//前のフレームで当たっていたポリゴン番号
	VECTOR prevHitPolygonCenter;					//前のフレームで当たっていたポリゴン中点ポジション

	//動き関係
	State nowstateKind;								//現在の状態
	bool isChangeState;								//状態からの状態変更指示がある場合

	//落下
	float Gravity;									//重力
	float fallSpeed;								//落下速度
	int fallFrame;									//落下時間
	float fallDamage;								//落下ダメージ
	float FallDamageIncrease;						//落下ダメージの増加量

	//突き刺し攻撃時
	float cameraZoom;								//カメラのズーム
	float piercingArmRotateZ;						//腕の回転度
	bool onPiercingGauge;							//突き刺し攻撃ゲージ表示

	//登り時
	int MinusGripPoint;								//減らす握力量

	//ジャンプ時
	bool jumpAfterLeaveFoot;						//ジャンプした後に足が離れた


	//その他
	bool isGameOver;								//ゲームオーバーフラグ
};