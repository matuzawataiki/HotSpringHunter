#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	m_model.Init("Assets/modelData/unityChan.tkm");

	return true;
}

void Game::Update()
{
}

void Game::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
