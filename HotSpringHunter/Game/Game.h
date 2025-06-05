#pragma once

namespace Character {
	class Player;
}

class GameCamera;
class EnemyBase;
class StartWaveCollision;
class UI;
class WildBoar;
class Bear;
class StageManager;

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
	StartWaveCollision* m_waveCollision = nullptr;
	UI*					m_ui			= nullptr;
	Bear*				m_bear			= nullptr;
	WildBoar*			m_wildBoar		= nullptr;
	StageManager*		m_stageManager	= nullptr;
};

