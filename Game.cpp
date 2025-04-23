#include "Include.h"

/// <summary>
/// �C���X�^���X��
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	if (player == NULL)
	{
		//�C���X�^���X��
		player = new Player();
		dragon = new Dragon();
		camera = new Camera();
		map = new Map();
		input = new Input();
		camera->SetObserver(player);
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Game::~Game()
{

}

/// <summary>
/// ������
/// </summary>
void Game::Initialize()
{
	player->Initialize();
	dragon->Initialize();
	camera->Initialize();
	map->Initialize();
}

/// <summary>
/// �X�V
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