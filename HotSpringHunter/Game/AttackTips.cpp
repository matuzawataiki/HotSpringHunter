#include "stdafx.h"
#include "AttackTips.h"
#include "GameController.h"

AttackTips::AttackTips()
{
}

AttackTips::~AttackTips()
{
}

bool AttackTips::Start()
{
	m_tipsModel.Init("Assets/modelData/image/attack_s.dds", 1920.0f, 1080.0f);

	return true;
}

void AttackTips::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_controller = NewGO<GameController>(0, "GameController");
		DeleteGO(this);
	}
}

void AttackTips::Render(RenderContext& rc)
{
	m_tipsModel.Draw(rc);
}
