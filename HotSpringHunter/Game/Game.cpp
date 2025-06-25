#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyManager.h"
#include "EnemyHPBar.h"
#include "UI.h"
#include "BackGround/StageManager.h";
#include "SceneManager.h"
#include "SoundEffect.h"
#include "EffectHub.h"
#include "GameOver.h"
#include "GameClear.h"
#include "ItemExplanation.h"
#include "ProjectileManager.h"

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_sceneManager);
	DeleteGO(m_stageManager);
	DeleteGO(m_enemyManager);
	DeleteGO(m_player);
	DeleteGO(m_ui);
	//DeleteGO(m_soundEffect);
	DeleteGO(m_soundEffect);
	DeleteGO(m_projectileManager);
	DeleteGO(m_itemExplanation);
}

bool Game::Start()
{
	//ゲーム内UI
	m_gameUI.Init("Assets/modelData/image/ingame.dds", 1920.0f, 1080.0f);

	m_gameCamera		= NewGO<GameCamera>(0, "gameCamera");
	m_player			= NewGO<Character::Player>(0, "player");
	m_sceneManager		= NewGO<SceneManager>(0, "sceneManager");
	m_stageManager		= NewGO<StageManager>(0, "stageManager");
	m_enemyManager		= NewGO<EnemyManager>(0, "enemyManager");
	m_projectileManager = NewGO<ProjectileManager>(0, "projectileManager");
	m_ui				= NewGO<UI>(0, "ui");

	//m_soundEffect = NewGO<SoundEffect>(0, "soundEffect");
	m_effectHub = NewGO<EffectHub>(0, "effectHub");
	m_itemExplanation = NewGO<ItemExplanation>(0, "itemExplanation");

	//当たり判定を可視化する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{
	m_gameUI.Draw(rc);
}