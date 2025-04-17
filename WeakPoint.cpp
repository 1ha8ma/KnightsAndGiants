#include"DxLib.h"
#include"CollisionManager.h"
#include"SoundEffect.h"
#include"Effect.h"
#include"WeakPoint.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="tag">当たり判定タグ</param>
/// <param name="partsName">パーツの名前</param>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="frameIndex1">カプセル用ボーン番号1</param>
/// <param name="frameIndex2">カプセル用ボーン番号２</param>
/// <param name="capsuleRadius">カプセル半径</param>
WeakPoint::WeakPoint(int modelHandle, int frameIndex1, int frameIndex2, float capsuleRadius)
{
	//ハンドルコピー
	this->modelHandle = modelHandle;
	//タグ設定
	this->tag = ObjectTag::WeakPoint;

	//フレーム番号取得
	this->frameIndex1 = frameIndex1;
	this->frameIndex2 = frameIndex2;

	//カプセル設定
	this->capsuleRadius = capsuleRadius;
	capsuleStart = MV1GetFramePosition(modelHandle, frameIndex1);
	capsuleEnd = MV1GetFramePosition(modelHandle, frameIndex2);

	//インスタンス化
	se = new SoundEffect();

	//private変数初期化
	hitWeakPointEffectflg = false;
	hitWeakPointEffectflame = 0;

	//当たり判定情報追加
	collisionManager = collisionManager->GetInstance();
	collisionData.HitProcess = std::bind(&WeakPoint::OnHitObject, this, std::placeholders::_1);
	UpdateCollisionData();
	collisionManager->AddCollisionData(&collisionData);

	//エフェクト
	effect = new Effect();
	effect->PlayEffect(Effect::EffectKind::WeakPoint, capsuleStart, WeakPointEffectScale, WeakPointEffectRotate, WeakPointEffectPlaySpeed);
}

/// <summary>
/// デストラクタ
/// </summary>
WeakPoint::~WeakPoint()
{
	MV1ResetFrameUserLocalMatrix(modelHandle, frameIndex1);
	MV1ResetFrameUserLocalMatrix(modelHandle, frameIndex2);
	collisionManager->RemoveCollisionData(&collisionData);
	//se削除
	delete se;
	//エフェクト削除
	effect->RemoveEffect(Effect::EffectKind::WeakPoint);
	delete effect;
}

/// <summary>
/// 初期化
/// </summary>
void WeakPoint::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void WeakPoint::Update()
{
	//カプセル設定
	//フレームと親フレームの間
	capsuleStart = MV1GetFramePosition(modelHandle, frameIndex1);
	capsuleEnd = MV1GetFramePosition(modelHandle, frameIndex2);

	//当たり判定情報更新
	UpdateCollisionData();

	//弱点ならエフェクトを付ける
	bool playing = effect->IsPlayEffect(Effect::EffectKind::WeakPoint);

	//終了していたらまた再生
	if (!playing)
	{
		effect->PlayEffect(Effect::EffectKind::WeakPoint, capsuleStart, WeakPointEffectScale, WeakPointEffectRotate, WeakPointEffectPlaySpeed);
	}

	effect->Update(Effect::EffectKind::WeakPoint, capsuleStart, WeakPointEffectRotate);


	//弱点ヒットのエフェクト更新
	if (hitWeakPointEffectflg)
	{
		effect->Update(Effect::EffectKind::HitWeakPoint, capsuleStart, HitEffectRotate);

		if (hitWeakPointEffectflame == PlayHitWeakPointEffectFlame)
		{
			effect->RemoveEffect(Effect::EffectKind::HitWeakPoint);
			hitWeakPointEffectflg = false;
		}

		//フレーム加算
		hitWeakPointEffectflame++;
	}
}

/// <summary>
/// 描画
/// </summary>
void WeakPoint::Draw()
{
	effect->Draw();

	//確認用
	//DrawCapsule3D(capsuleStart, capsuleEnd, capsuleRadius, 8, GetColor(102, 0, 255), GetColor(102, 0, 255), false);
	//DrawSphere3D(movePrevPos, capsuleRadius, 8, GetColor(102, 0, 255), GetColor(102, 0, 255), false);	//ポジションの点
}

/// <summary>
/// ダメージを渡す
/// </summary>
/// <returns>ダメージ</returns>
int WeakPoint::TakeDamage()
{
	//ダメージコピー
	int takeDamage = damage;
	//ダメージ初期化
	damage = 0;

	return takeDamage;
}

/// <summary>
/// 衝突後の処理
/// </summary>
/// <param name="hitObjectData">衝突したオブジェクト</param>
void WeakPoint::OnHitObject(CollisionData* hitObjectData)
{
	//プレイヤーの攻撃だった場合
	if (hitObjectData->tag == ObjectTag::Attack_P)
	{
		se->PlaySE(SoundEffect::SEKind::HitWeakPoint);
		damage = hitObjectData->attackPower * WeakPointDamageMultiplier;
		if (!hitWeakPointEffectflg)
		{
			effect->PlayEffect(Effect::EffectKind::HitWeakPoint, capsuleStart, HitEffectScale, HitEffectRotate, HitEffectPlaySpeed);
			hitWeakPointEffectflame = 0;
			hitWeakPointEffectflg = true;
		}
	}
}

/// <summary>
/// 当たり判定更新
/// </summary>
void WeakPoint::UpdateCollisionData()
{
	collisionData.tag = tag;
	collisionData.startPosition = capsuleStart;
	collisionData.endPosition = capsuleEnd;
	collisionData.radius = capsuleRadius;
	collisionData.attackPower = 0;
	collisionData.isCollisionActive = true;
}