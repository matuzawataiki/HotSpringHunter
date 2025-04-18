#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::Title()
{
	DeleteGO(m_game);
}

Title::~Title()
{

}

bool Title::Start()
{
	m_titleModel.Init("Assets/modelData/image/title.dds", 1920.0f, 1080.0f);

	return true;
}

void Title::Update()
{
	SwitchGame();
}

void Title::SwitchGame()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleModel.Draw(rc);
}