#pragma once

class GameCamera;
class BackGround;
class Player;
class Snake;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start()override;
	void Update()override;

	GameCamera* m_gameCamera = nullptr;
	BackGround* m_backGround = nullptr;
	Player* m_player = nullptr;
	Snake* m_snake = nullptr;
private:
};

