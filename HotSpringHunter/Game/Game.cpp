#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "CharacterBase.h"
#include "Player.h"
#include "Snake.h"


Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_backGround = NewGO<BackGround>(0, "backGround");
	m_player = NewGO<Player>(0, "player");
	//m_snake = NewGO<Snake>(0, "snake");

	return true;
}

void Game::Update()
{

}