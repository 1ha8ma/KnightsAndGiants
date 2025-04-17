#pragma once
#include<vector>
#include<map>

class Effect
{
public:
	//エフェクトの種類
	enum class EffectKind :int
	{
		WeakPoint,		//弱点
		HitWeakPoint,	//弱点ヒット
		Warning,		//注意
		RockHit,		//岩衝突
	};

	Effect();
	~Effect();

	//初期化
	void Initialize();
	//エフェクト再生
	void PlayEffect(EffectKind kind, VECTOR playPosition, VECTOR initScale, VECTOR rotate, float playSpeed);
	//更新
	void Update(EffectKind kind, VECTOR playPosition, VECTOR rotate);
	//描画
	void Draw();

	//指定のエフェクトが再生中か確認
	bool IsPlayEffect(EffectKind kind);

	//再生中のエフェクトを削除(無限ループのエフェクト用)
	void RemoveEffect(EffectKind kind);

private:
	std::vector<EffectKind> playingEffectKind;		//再生中のエフェクトの種類
	std::vector<int> playingEffectHandle;			//再生中のエフェクトハンドル
	std::vector<VECTOR> playingEffectScale;			//エフェクトサイズ
	std::map<EffectKind, int> effectHandle;			//エフェクトハンドル
};