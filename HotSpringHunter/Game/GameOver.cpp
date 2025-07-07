#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Result.h"
#include "Game.h"
#include "BattleManager.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	m_overModel.Init("Assets/modelData/image/gameover_2.dds", 1920.0f, 1080.0f);

	m_battleManager = FindGO<BattleManager>("battleManager");

	return true;
}

void GameOver::Update()
{
	//3秒たったら自動的にリザルトに飛ぶ。またはAボタンでリザルトにとぶ
	const float switchTime = g_gameTime->GetFrameDeltaTime();
	m_swtchDeltaTime += switchTime;
	if (m_swtchDeltaTime >= 3.0f || g_pad[0]->IsTrigger(enButtonA))
	{
		m_canChangeScene = true;
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_overModel.Draw(rc);
}
