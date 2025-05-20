#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Player.h"
#include "EnemyBase.h"
#include "StartWaveCollision.h"
#include "UI.h"

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
	m_backGround = NewGO<BackGround>(0, "backGround");
	m_player = NewGO<Player>(0, "player");
	//m_waveCollision = NewGO<StartWaveCollision>(0, "startWaveCollision");
	
	m_wildBoar = NewGO<WildBoar>(0, "wildboar");

	m_ui = NewGO<UI>(0, "ui");

	return true;
}

void Game::Update()
{

}
