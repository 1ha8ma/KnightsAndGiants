#include"SceneBase.h"
#include"TitleScene.h"

/// <summary>
/// �ŏ��̃V�[����ݒ�
/// </summary>
/// <returns>�V�[��</returns>
SceneBase* SceneBase::InitializeBase()
{
	return new TitleScene();
}