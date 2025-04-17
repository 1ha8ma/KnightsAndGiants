#include"DxLib.h"
#include"Loader.h"
#include"Wall.h"

/// <summary>
/// コンストラクタ
/// </summary>
Wall::Wall()
{
	Loader* loader = loader->GetInstance();

	modelHandle = loader->GetHandle(Loader::Kind::WallModel);

	MV1SetScale(modelHandle, Scale);
	MV1SetPosition(modelHandle, Position);
}

/// <summary>
/// デストラクタ
/// </summary>
Wall::~Wall()
{

}

/// <summary>
/// 描画
/// </summary>
void Wall::Draw()
{
	MV1DrawModel(modelHandle);
}