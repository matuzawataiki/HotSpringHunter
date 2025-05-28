#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Player.h"
#include "EnemyBase.h"
#include "StartWaveCollision.h"
#include "UI.h"
#include "BackGround/StageManager.h";
#include "Bear.h"
#include "WildBoar.h"

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_backGround);
	DeleteGO(m_player);
}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	//m_backGround = NewGO<BackGround>(0, "backGround");
	m_stageManager = NewGO<StageManager>(0, "stageManager");
	m_player = NewGO<Player>(0, "player");

	//m_waveCollision = NewGO<StartWaveCollision>(0, "startWaveCollision");
	m_bear = NewGO<Bear>(0, "bear");
	//m_wildBoar = NewGO<WildBoar>(0, "wildboar");

	m_ui = NewGO<UI>(0, "ui");

	//当たり判定を可視化する。
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{

}
