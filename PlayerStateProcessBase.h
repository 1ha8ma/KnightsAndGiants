#pragma once
#include"CollisionData.h"
#include"Camera.h"

class PlayerStateProcessBase
{
public:
	PlayerStateProcessBase(int modelHandle);
	virtual ~PlayerStateProcessBase() {};

	/// <summary>
	/// 動きで使用するプレイヤーのデータ
	/// </summary>
	struct UsePlayerData
	{
		int inputState;						//ボタン入力状態
		DINPUT_JOYSTATE stickState;			//スティック入力状態
		VECTOR position;					//ポジション
		VECTOR capsuleStart;				//カプセル始点
		VECTOR capsuleEnd;					//カプセル終点
		VECTOR centerPosition;				//カプセルの中点
		float capsuleRadius;				//カプセル半径
		float wholeBodyCapsuleHalfLength;	//カプセルの長さの半分
		float angle;						//水平角度
		bool onFoot;						//プレイヤーの足が着いているか
		VECTOR lookDirection;				//向いている方向
	};

	//フレーム番号
	enum PlayerFrameNumber
	{
		Hips = 1,					//腰辺り
		RightArmFrameNumber = 45,	//右腕のフレーム番号
		RightHandFrameNumber = 50,	//右手のフレーム番号(小指)
	};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() abstract;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="inputstate">ボタン入力状態</param>
	/// <param name="stickstate">スティック入力情報</param>
	/// <param name="camera">カメラ</param>
	/// <returns>ステートからのステート変更指示</returns>
	//virtual bool Update(VECTOR position, float angle, int inputstate, DINPUT_JOYSTATE stickstate, const Camera& camera, CollisionData objectCollision) { return false; }

	virtual bool Update(UsePlayerData playerData, const Camera& camera, CollisionData objectCollision) { return false; }

	/// <summary>
	/// ゲームオーバーシーン更新
	/// </summary>
	virtual void UpdateGameOver() {};

	/// <summary>
	/// 描画(確認用)
	/// </summary>
	virtual void Draw() {};


	//Get
	VECTOR GetmoveVec() { return moveVec; }
	VECTOR GetLookDirection() { return newLookDirection; }
	MATRIX GetRotateMatrix() { return rotateMatrix; }

	//突き刺し用
	float GetCameraZoom() { return cameraZoom; }
	float GetArmRotateZ() { return armRotateZ; }

protected:
	//アニメーション番号
	enum PlayerAnimationNumber
	{
		Run,			//走る
		Jump,			//ジャンプ
		NormalAttack,	//通常攻撃
		Climb,			//登り
		Squat,			//しゃがみ
		FallDown,		//倒れる
		Idle,			//待機
		Falling,		//落下
	};
	
	//アニメーション再生
	void PlayAnimation(float playSpeed,bool stop);
	//アニメーション再生(1回のみ)
	bool PlayAnimationOnce(float playSpeed);
	//アニメーション逆再生
	bool ReversePlaybackAnimation(float playSpeed, float backPoint);
	//アニメーションの終了
	void DetachAnimation(int attachIndex);

	//モデル・アニメーション
	int modelHandle;			//モデルハンドル
	int nowPlayAnim;			//再生中のアニメーション
	float animTotalTime;		//アニメーションの総再生時間
	float nowAnimPlayTime;		//現在のアニメーション再生時間

	//移動
	VECTOR moveVec;				//移動ベクトル
	VECTOR newLookDirection;	//処理後のプレイヤーの向くべき方向
	MATRIX rotateMatrix;		//モデル回転行列

	//突き刺し攻撃用
	float cameraZoom;
	float armRotateZ;
};