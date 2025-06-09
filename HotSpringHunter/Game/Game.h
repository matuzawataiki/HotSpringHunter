#pragma once

namespace Character {
	class Player;
}

class GameCamera;
class EnemyBase;
class EnemyManager;
class EnemyHPBar;
class StartWaveCollision;
class UI;
class WildBoar;
class Bear;
class SnakeEnemy;
class StageManager;
class GameOver;
class GameClear;
class SoundEffect;

class SoundEffect;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start()override;
	void Update()override;

private:

	GameCamera*			m_gameCamera	= nullptr;
	Character::Player*	m_player		= nullptr;
	EnemyBase*			m_enemyBase		= nullptr;
	EnemyManager*		m_enemyManager	= nullptr;
	EnemyHPBar*			m_enemyHPBar	= nullptr;
	StartWaveCollision* m_waveCollision = nullptr;
	UI*					m_ui			= nullptr;
	Bear*				m_bear			= nullptr;
	WildBoar*			m_wildBoar		= nullptr;
	SnakeEnemy*			m_snakeEnemy	= nullptr;
	StageManager*		m_stageManager	= nullptr;
  SoundEffect* m_soundEffect = nullptr;
  GameOver* m_gameOver = nullptr;
	GameClear* m_gameClear = nullptr;
};

