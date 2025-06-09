#include "stdafx.h"
#include "GameClear.h"
#include "Game.h"
#include "Result.h"


GameClear::GameClear()
{
}

GameClear::~GameClear()
{
}

bool GameClear::Start()
{
	m_gameClearModel.Init("Assets/modelData/UI/aioue.dds", 1920.0f, 1080.0f);

	return true;
}

void GameClear::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_result = NewGO<Result>(0, "Result");
		Game* game = FindGO<Game>("game");
		DeleteGO(this);
		DeleteGO(game);
	}
}

void GameClear::Render(RenderContext&rc)
{
	m_gameClearModel.Draw(rc);
}

