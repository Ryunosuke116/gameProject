#pragma once
class Game : public BaseScene
{
public:
	Game(SceneManager& manager);
	~Game();

	void Initialize();
	void Update();
	void Draw();
private:
	
	//’è‹`
	Player* player = NULL;
	Camera* camera = NULL;
	Map* map = NULL;
	Input* input = NULL;
	BaseChara* dragon = NULL;
};

