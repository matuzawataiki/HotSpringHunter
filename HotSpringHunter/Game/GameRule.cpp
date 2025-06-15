#include "stdafx.h"
#include "GameRule.h"
#include "AttackTips.h"

GameRule::GameRule()
{

}

GameRule::~GameRule()
{

}

bool GameRule::Start()
{
	m_ruleModel.Init("Assets/modelData/image/setumei.dds", 1920.0f, 1080.0f);

	return true;
}

void GameRule::Update()
{
	//切り替わり
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_attackTips = NewGO<AttackTips>(0, "attackTips");
		DeleteGO(this);
	}
}

void GameRule::Render(RenderContext& rc)
{
	m_ruleModel.Draw(rc);
}