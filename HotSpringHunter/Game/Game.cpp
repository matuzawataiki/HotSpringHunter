#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyManager.h"
#include "EnemyHPBar.h"
#include "StartWaveCollision.h"
#include "UI.h"
#include "BackGround/StageManager.h";
#include "Bear.h"
#include "WildBoar.h"
#include "SoundEffect.h"
#include "SnakeEnemy.h"

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_stageManager = NewGO<StageManager>(0, "stageManager");
	m_player = NewGO<Character::Player>(0, "player");
	//m_waveCollision = NewGO<StartWaveCollision>(0, "startWaveCollision");
	//m_bear = NewGO<Bear>(0, "bear");
	//m_wildBoar = NewGO<WildBoar>(0, "wildBoar");

	//m_wildBoar = NewGO<WildBoar>(0, "wildBoar");
	m_enemyManager = NewGO<EnemyManager>(0, "enemyManager");
	//m_enemyHPBar = NewGO<EnemyHPBar>(0, "enemyHPBar");

	m_ui = NewGO<UI>(0, "ui");

	m_soundEffect = NewGO<SoundEffect>(0, "soundEffect");

	//当たり判定を可視化する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{

}
