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
class SoundEffect;
class EffectHub;
class ItemExplanation;

class Game : public IScene
{
	appScene(Game);

public:
	enum EnResultType
	{
		enResultType_Clear,		// ゲームクリア
		enResultType_Over,		// ゲームオーバー
		enResultType_None,		// リザルト設定されていない
	};

public:
	Game();
	~Game();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	bool RequestScene(uint32_t& id) override;

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
	ProjectileManager*	m_projectileManager = nullptr;
	ItemExplanation*	m_itemExplanation	= nullptr;

	SpriteRender m_gameUI;

	EnResultType m_resultType				= EnResultType::enResultType_None;
 };