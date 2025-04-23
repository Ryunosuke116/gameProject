#include "Include.h"

/// <summary>
/// インスタンス化
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	if (player == NULL)
	{
		//インスタンス化
		player = new Player();
		dragon = new Dragon();
		camera = new Camera();
		map = new Map();
		input = new Input();
		camera->SetObserver(player);
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Game::~Game()
{

}

/// <summary>
/// 初期化
/// </summary>
void Game::Initialize()
{
	player->Initialize();
	dragon->Initialize();
	camera->Initialize();
	map->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void Game::Update()
{
	input->Update();
	player->Update(*input,camera->GetCameraDirection(),dragon->GetFramePosition());
	dragon->Update();
	camera->Update(player->GetFramePosition(),dragon->GetFramePosition());
}

void Game::Draw()
{
	clsDx();
	player->Draw();
	dragon->Draw();
	map->Draw();
	camera->Draw();

}