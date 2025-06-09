#pragma once

class GameCamera;
class BackGround;
class Player;
class EnemyBase;
class StartWaveCollision;
class UI;
class WildBoar;
class Bear;
class StageManager;
class GameOver;
class GameClear;
class SoundEffect;

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
	UI* m_ui = nullptr;
	Bear* m_bear = nullptr;
	WildBoar* m_wildBoar = nullptr;
	GameOver* m_gameOver = nullptr;
	GameClear* m_gameClear = nullptr;

	SoundEffect* m_soundEffect = nullptr;

private:
	StageManager* m_stageManager = nullptr;
};

