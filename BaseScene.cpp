#include "Include.h"

/// <summary>
/// インスタンス化
/// </summary>
BaseScene::BaseScene(SceneManager& manager) :manager{ manager } {}

/// <summary>
/// デストラクタ
/// </summary>
BaseScene::~BaseScene()
{

}

/// <summary>
/// 初期化
/// </summary>
void BaseScene::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void BaseScene::Update()
{

}

/// <summary>
/// 描画
/// </summary>
void BaseScene::Draw()
{

}

void BaseScene::ChangeScene(const std::string_view name)
{
    manager.ChangeScene(name);
}