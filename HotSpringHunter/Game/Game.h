#pragma once

namespace Character {
	class Player;
}

namespace Enemy {
	class PoisonSnake;
}

class GameCamera;
class EnemyBase;
class EnemyManager;
class EnemyHPBar;
class StartWaveCollision;
class UI;
class StageManager;
class SceneManager;
class GameOver;
class SoundEffect;
class EffectHub;


class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc);

private:
	GameCamera*			m_gameCamera	= nullptr;
	Character::Player*	m_player		= nullptr;
	EnemyBase*			m_enemyBase		= nullptr;
	EnemyManager*		m_enemyManager	= nullptr;
	EnemyHPBar*			m_enemyHPBar	= nullptr;
	StartWaveCollision* m_waveCollision = nullptr;
	UI*					m_ui			= nullptr;
	StageManager*		m_stageManager	= nullptr;
	SceneManager*		m_sceneManager	= nullptr;
	SoundEffect*		m_soundEffect	= nullptr;
	EffectHub*			m_effectHub		= nullptr;
	GameOver*			m_gameOver		= nullptr;
	EnemySpawner*		m_enemySpawner	= nullptr;

	SpriteRender m_gameUI;
};

