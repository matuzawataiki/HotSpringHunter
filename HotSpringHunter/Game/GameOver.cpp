#include "stdafx.h"
#include "GameOver.h"
#include "GameOver.h"
#include "Title.h"
#include "Result.h"
#include "Game.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	m_overModel.Init("Assets/modelData/image/gameover_2.dds", 1920.0f, 1080.0f);
	return true;
}

void GameOver::Update()
{
	SwitchResult();
}

void GameOver::SwitchResult()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_overResult = NewGO<GameOverResult>(0, "GameOverResult");
		Game*game = FindGO<Game>("game");
		DeleteGO(this);
		DeleteGO(game);
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_overModel.Draw(rc);
}
