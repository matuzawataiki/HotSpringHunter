#include "stdafx.h"
#include "GameController.h"
#include "Game.h"
#include "SoundEffect.h"

GameController::GameController()
{
}

GameController::~GameController()
{
	m_soundEffect->StopBGM();
}

bool GameController::Start()
{
	m_controllerModel.Init("Assets/modelData/image/contllor.dds", 1920.0f, 1080.0f);
	m_soundEffect = FindGO<SoundEffect>("soundEffect");

	return true;
}

void GameController::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);
		//m_soundEffect->Stop();
	}
}


void GameController::Render(RenderContext& rc)
{
	m_controllerModel.Draw(rc);
}
