#include "stdafx.h"
#include "TipsScene.h"
#include "Game.h"

namespace
{
	// TipsのIDを定義
	enum  EnTipsID
	{
		enTipsID_GameRule = 0,
		enTipsID_AttackTips,
		enTipsID_Controller,
		enTipsID_Max,
	};

	// TipsのIDによって生成する画像を変える
	SpriteRender* InializeTipsRender(const int tipsId)
	{
		SpriteRender* spritRender = nullptr;
		switch (tipsId)
		{
			case enTipsID_GameRule:
			{
				spritRender = new SpriteRender();
				spritRender->Init("Assets/modelData/image/setumei.dds", 1920.0f, 1080.0f);
				break;
			}
			case enTipsID_AttackTips:
			{
				spritRender = new SpriteRender();
				spritRender->Init("Assets/modelData/image/attack_s.dds", 1920.0f, 1080.0f);
				break;
			}
			case enTipsID_Controller:
			{
				spritRender = new SpriteRender();
				spritRender->Init("Assets/modelData/image/contllor.dds", 1920.0f, 1080.0f);
				break;
			}
			case enTipsID_Max:
			{
				// 何もしない
			}
			default:
			{
				K2_ASSERT(false, "追加漏れがあります。\n");
				break;
			}
		}
		return spritRender;
	}
}

bool TipsScene::Start()
{
	m_tipsRender = InializeTipsRender(m_currentTipsId);
	return true;
}

void TipsScene::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_currentTipsId++;
		if (m_currentTipsId < enTipsID_Max) {
			m_tipsRender = InializeTipsRender(m_currentTipsId);
		}
	}
	if(m_currentTipsId < enTipsID_Max) {
		m_currentTipsId = enTipsID_Max;
	}
}

void TipsScene::Render(RenderContext& rc)
{
	if (m_tipsRender) {
		m_tipsRender->Draw(rc);
	}
}

bool TipsScene::RequestScene(uint32_t& id)
{
	if (m_currentTipsId == enTipsID_Max) {
		id = Game::ID();
		return true;
	}
	return false;
}