#include "stdafx.h"
#include "Game.h"
#include "Test.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	m_test = NewGO<Test>(0, "test");
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}
