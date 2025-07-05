#pragma once

#include "Scene/IScene.h"

namespace Character {
	class Player;
}

namespace Enemy {
	class PoisonSnake;
}
class ProjectileManager;
class GameCamera;
class EnemyBase;
class EnemyManager;
class EnemyHPBar;
class StartWaveCollision;
class UI;
class StageManager;
class BattleManager;
class GameOver;
class SoundEffect;
class EffectHub;
class ItemExplanation;

class Game : public IScene
{
	appScene(Game);

public:
	Game();
	~Game();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	bool RequestScene(uint32_t& id) override;

	/// <summary>
	/// リザルトの時間を設定
	/// リザルト画面で表示するための受け皿
	/// GameClearやGameOverに渡します
	/// </summary>
	inline void SetResultTime(const float time) { m_gameResultTime = time; }

private:
	GameCamera*			m_gameCamera		= nullptr;
	Character::Player*	m_player			= nullptr;
	EnemyBase*			m_enemyBase			= nullptr;
	EnemyManager*		m_enemyManager		= nullptr;
	EnemyHPBar*			m_enemyHPBar		= nullptr;
	StartWaveCollision* m_waveCollision		= nullptr;
	UI*					m_ui				= nullptr;
	StageManager*		m_stageManager		= nullptr;
	BattleManager*		m_battleManager		= nullptr;
	SoundEffect*		m_soundEffect		= nullptr;
	EffectHub*			m_effectHub			= nullptr;
	GameOver*			m_gameOver			= nullptr;
	ProjectileManager*	m_projectileManager = nullptr;
	ItemExplanation*	m_itemExplanation	= nullptr;

	SpriteRender m_gameUI;

	float m_gameResultTime					= 0.0f;
 };