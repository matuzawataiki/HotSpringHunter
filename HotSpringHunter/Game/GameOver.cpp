#include "stdafx.h"
#include "GameOver.h"
#include "GameOver.h"
#include "Title.h"
#include "Result.h"
#include "Game.h"
#include "SceneManager.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	m_overModel.Init("Assets/modelData/image/gameover_2.dds", 1920.0f, 1080.0f);

	m_sceneManager = FindGO<SceneManager>("sceneManager");

	return true;
}

void GameOver::Update()
{
	SwitchResult();
}

void GameOver::SwitchResult()
{
	//3秒たったら自動的にリザルトに飛ぶ。またはAボタンでリザルトにとぶ
	const float switchTime = g_gameTime->GetFrameDeltaTime();
	m_swtchDeltaTime += switchTime;
	if (m_swtchDeltaTime >= 3.0f || g_pad[0]->IsTrigger(enButtonA))
	{
		m_overResult = NewGO<GameOverResult>(0, "GameOverResult");
		m_overResult->m_gameTime = m_timer;
		Game* game = FindGO<Game>("game");
		DeleteGO(this);
		DeleteGO(game);
		m_swtchDeltaTime = 0.0f;
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_overModel.Draw(rc);
}
