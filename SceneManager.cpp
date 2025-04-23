#include "Include.h"

SceneManager::SceneManager() :nowScene{ nullptr } {}

SceneManager::~SceneManager()
{
	for (const auto& pair : scenes)
	{
		delete pair.second;
	}
}


void SceneManager::ChangeScene(const std::string_view name)
{
	try
	{
		nowScene = scenes.at(name);
		nowScene->Initialize();
	}
	catch (...)
	{
		assert(!"存在しないキーが指定されました。");
	}
}

void SceneManager::Initialize()
{
	nowScene->Initialize();
}

void SceneManager::Update()
{
	nowScene->Update();
}

void SceneManager::Draw()
{
	nowScene->Draw();
}

