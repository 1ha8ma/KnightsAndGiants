#include"DxLib.h"
#include"Loader.h"
#include"Wall.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Wall::Wall()
{
	Loader* loader = loader->GetInstance();

	modelHandle = loader->GetHandle(Loader::Kind::WallModel);

	MV1SetScale(modelHandle, Scale);
	MV1SetPosition(modelHandle, Position);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Wall::~Wall()
{

}

/// <summary>
/// �`��
/// </summary>
void Wall::Draw()
{
	MV1DrawModel(modelHandle);
}