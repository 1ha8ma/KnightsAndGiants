#pragma once
#include"Camera.h"

class ArmEnemyMoveBase
{
public:
	//フレーム番号
	enum class ArmEnemyFrameIndex :int
	{
		Upperarm = 16,				//上腕
		Forearm = 17,				//前腕
		Hand = 18,					//手
		Finger1 = 22,				//親指
		LeftHandMiddle = 30,		//中指第二関節
		Finger2 = 40,				//小指の先
	};

	ArmEnemyMoveBase(int modelHandle);
	virtual ~ArmEnemyMoveBase() {};
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns>動きの終了</returns>
	virtual bool Update(Camera* camera,VECTOR playerPosition) { return false; }

	/// <summary>
	/// 倒された後用更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns>動きの終了</returns>
	virtual bool UpdateFallDown(Camera* camera) { return false; }

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()abstract;


	VECTOR GetRotate() { return rotate; }

protected:
	int modelHandle;	//モデルハンドル
	VECTOR rotate;		//回転用
	int moveState;		//動き状態
};