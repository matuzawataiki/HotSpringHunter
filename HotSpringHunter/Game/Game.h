#pragma once

class GameCamera;
class BackGround;
class Player;
class EnemyBase;
class Bear;
class StartWaveCollision;
class UI;
class StageManager;
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
	EnemyBase* m_enemyBase = nullptr;
	StartWaveCollision* m_waveCollision = nullptr;
	Bear* m_bear = nullptr;
	UI* m_ui = nullptr;

private:
	StageManager* m_stageManager = nullptr;
};

