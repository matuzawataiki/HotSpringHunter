#include "stdafx.h"
#include "GameOver.h"
#include "GameOver.h"
#include "Title.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	m_overModel.Init("Assets/modelData/image/gameOver.dds", 1920.0f, 1080.0f);
	return true;
}

void GameOver::Update()
{
	SwitchTitle();
}

void GameOver::SwitchTitle()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_overModel.Draw(rc);
}
