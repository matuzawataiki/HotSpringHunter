#include "stdafx.h"
#include "GameController.h"
#include "Game.h"

GameController::GameController()
{
}

GameController::~GameController()
{
}

bool GameController::Start()
{
	m_controllerModel.Init("Assets/modelData/image/contllor.dds", 1920.0f, 1080.0f);

	return true;
}

void GameController::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}


void GameController::Render(RenderContext& rc)
{
	m_controllerModel.Draw(rc);
}
