#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Player.h"
#include "EnemyBase.h"
#include "Bear.h"
#include "StartWaveCollision.h"


Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_backGround);
	DeleteGO(m_player);
	DeleteGO(m_bear);
}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_backGround = NewGO<BackGround>(0, "backGround");
	m_player = NewGO<Player>(0, "player");
	//m_waveCollision = NewGO<StartWaveCollision>(0, "startWaveCollision");
	m_bear = NewGO<Bear>(0, "bear");

	return true;
}

void Game::Update()
{

}
