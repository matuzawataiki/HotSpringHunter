#pragma once

class GameCamera;
class BackGround;
class Player;
class Bear;
class StartWaveCollision;
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
	StartWaveCollision* m_waveCollision = nullptr;
	Bear* m_bear = nullptr;
private:
};

