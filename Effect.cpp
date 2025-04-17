#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"Loader.h"
#include"Effect.h"

/// <summary>
/// コンストラクタ
/// </summary>
Effect::Effect()
{
	Loader* loader = loader->GetInstance();

	effectHandle[EffectKind::WeakPoint] = loader->GetHandle(Loader::Kind::WeakPointEffect);
	effectHandle[EffectKind::HitWeakPoint] = loader->GetHandle(Loader::Kind::HitWeakPointEffect);
	effectHandle[EffectKind::Warning] = loader->GetHandle(Loader::Kind::WarningEffect);
	effectHandle[EffectKind::RockHit] = loader->GetHandle(Loader::Kind::RockHitEffect);

	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
Effect::~Effect()
{
	//エフェクト終了
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		//Effekseerでは再生を止める事はできても描画を消すことが出来なかったため下に飛ばしている
		SetPosPlayingEffekseer3DEffect(playingEffectHandle[i], 0.0f, -10000.0f, 0.0f);
		StopEffekseer3DEffect(playingEffectHandle[i]);
	}
	UpdateEffekseer3D();

	//情報削除
	playingEffectHandle.clear();
	playingEffectKind.clear();
	playingEffectScale.clear();
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		playingEffectHandle.erase(playingEffectHandle.begin() + i);
		playingEffectKind.erase(playingEffectKind.begin() + i);
		playingEffectScale.erase(playingEffectScale.begin() + i);
	}

	DrawEffekseer3D_End();
}

/// <summary>
/// 初期化
/// </summary>
void Effect::Initialize()
{
	playingEffectHandle.clear();
	playingEffectKind.clear();
	playingEffectScale.clear();
}

/// <summary>
/// エフェクト再生指示
/// </summary>
/// <param name="kind">エフェクトの種類</param>
/// <param name="playPosition">初期再生位置</param>
/// <param name="initSize">初期サイズ</param>
/// <param name="rotate">回転</param>
/// <param name="playSpeed">再生速度</param>
void Effect::PlayEffect(EffectKind kind, VECTOR playPosition, VECTOR initScale, VECTOR rotate, float playSpeed)
{
	//再生しているエフェクトを追加
	playingEffectKind.push_back(kind);
	//ハンドル追加
	playingEffectHandle.push_back(PlayEffekseer3DEffect(effectHandle[kind]));

	//スケール設定
	playingEffectScale.push_back(initScale);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle.back(), playingEffectScale.back().x, playingEffectScale.back().y, playingEffectScale.back().z);

	//エフェクト再生速度設定
	SetSpeedPlayingEffekseer3DEffect(playingEffectHandle.back(), playSpeed);
	//表示角度設定
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle.back(), rotate.x, rotate.y, rotate.z);
	//色変更
	if (kind == EffectKind::WeakPoint || kind == EffectKind::HitWeakPoint)
	{
		SetColorPlayingEffekseer3DEffect(playingEffectHandle.back(), 0, 255, 255, 255);
	}
	//ポジション反映
	SetPosPlayingEffekseer3DEffect(playingEffectHandle.back(), playPosition.x, playPosition.y, playPosition.z);
}

/// <summary>
/// エフェクト毎の更新
/// </summary>
/// <param name="kind">ポジション等を更新させたい種類</param>
/// <param name="playPosition">再生ポジション</param>
/// <param name="rotate">再生角度</param>
void Effect::Update(EffectKind kind,VECTOR playPosition, VECTOR rotate)
{
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		//指定と同じものを更新
		if (playingEffectKind[i] == kind)
		{
			//表示角度設定
			SetRotationPlayingEffekseer3DEffect(playingEffectHandle[i], rotate.x, rotate.y, rotate.z);
			//ポジション反映
			SetPosPlayingEffekseer3DEffect(playingEffectHandle[i], playPosition.x, playPosition.y, playPosition.z);
			//再生終了していたら再生中から削除
			if (IsEffekseer3DEffectPlaying(playingEffectHandle[i]) == -1)
			{
				playingEffectHandle.erase(playingEffectHandle.begin() + i);
				playingEffectKind.erase(playingEffectKind.begin() + i);
				playingEffectScale.erase(playingEffectScale.begin() + i);
			}

			break;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Effect::Draw()
{
	DrawEffekseer3D();
}

/// <summary>
/// 指定のエフェクトが再生中か
/// </summary>
/// <param name="kind">エフェクトの種類</param>
/// <returns>再生中：true 未再生：false</returns>
bool Effect::IsPlayEffect(EffectKind kind)
{
	bool playing = false;

	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		if (playingEffectKind[i] == kind)
		{
			playing = true;
			break;
		}
	}

	return playing;
}

/// <summary>
/// エフェクト削除(無限ループエフェクト用)
/// </summary>
/// <param name="kind">削除するエフェクト</param>
void Effect::RemoveEffect(EffectKind kind)
{
	//エフェクト終了
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		//Effekseerでは再生を止める事はできても描画を消すことが出来なかったため下に飛ばしている
		if (playingEffectKind[i] == kind)
		{
			SetPosPlayingEffekseer3DEffect(playingEffectHandle[i], 0.0f, -10000.0f, 0.0f);
			StopEffekseer3DEffect(playingEffectHandle[i]);

			break;
		}
	}
	UpdateEffekseer3D();

	//情報削除
	playingEffectHandle.clear();
	playingEffectKind.clear();
	playingEffectScale.clear();
	for (int i = 0; i < playingEffectHandle.size(); i++)
	{
		playingEffectHandle.erase(playingEffectHandle.begin() + i);
		playingEffectKind.erase(playingEffectKind.begin() + i);
		playingEffectScale.erase(playingEffectScale.begin() + i);
	}
}