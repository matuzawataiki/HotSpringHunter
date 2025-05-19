#include "stdafx.h"
#include "UI.h"
#include "Player.h"

namespace
{
	const float PLAYER_HP_FRAME_WIDTH = 700.0f;
	const float PLAYER_HP_FRAME_HEIGHT = 140.0f;
	const float PLAYER_HP_BAR_WIDTH = 600.0f;
	const float PLAYER_HP_BAR_HEIGHT = 50.0f;

	const Vector2 PLAYER_HP_FRAME_POS = Vector2{ -590.0f,-460.0f };
	const Vector2 PLAYER_HP_BAR_POS = Vector2{ -850.0f,-500.0f };
	const Vector2 HP_PIVOT = Vector2{ 0.0f,0.0f };

}

UI::UI()
{
}

UI::~UI()
{
}

bool UI::Start()
{
	NewGO<PlayerHPUI>(0, "playerHPUI");
	return true;
}

void UI::Update()
{
}

void UI::Render(RenderContext& rc)
{
}

/*****************************************************************
///プレイヤーのHPUI
******************************************************************/

PlayerHPUI::PlayerHPUI()
{
}

PlayerHPUI::~PlayerHPUI()
{
}

bool PlayerHPUI::Start()
{
	m_player = FindGO<Player>("player");

	m_playerHPBack.Init("Assets/modelData/UI/playerHPBack.dds", PLAYER_HP_FRAME_WIDTH, PLAYER_HP_FRAME_HEIGHT);
	m_playerHPBack.SetPosition(PLAYER_HP_FRAME_POS);
	m_playerHPBack.Update();

	m_playerHPBar.Init("Assets/modelData/UI/playerHPBar.dds", PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT);
	m_playerHPBar.SetPosition(PLAYER_HP_BAR_POS);
	m_playerHPBar.SetPivot(HP_PIVOT);
	m_playerHPBar.Update();

	m_playerHPFrame.Init("Assets/modelData/UI/playerHPFrame.dds", PLAYER_HP_FRAME_WIDTH, PLAYER_HP_FRAME_HEIGHT);
	m_playerHPFrame.SetPosition(PLAYER_HP_FRAME_POS);
	m_playerHPFrame.Update();

	return true;
}

void PlayerHPUI::Update()
{
	UpdateBarWidth();
}

void PlayerHPUI::UpdateBarWidth()
{

	// TODO:simoka 基点が今中心に来ているので、中央左に行くように修正。

	m_playerHPBar.SetScale(Vector2{ m_player->GetPlayerHP() / m_player->GetPlayerMAXHP(),1.0f });
	m_playerHPBar.Update();
}

void PlayerHPUI::Render(RenderContext& rc)
{
	m_playerHPBack.Draw(rc);
	m_playerHPBar.Draw(rc);
	m_playerHPFrame.Draw(rc);
	
}

/*****************************************************************
///エネミーのHPUI
******************************************************************/

EnemyHPUI::EnemyHPUI()
{
}

EnemyHPUI::~EnemyHPUI()
{
}

bool EnemyHPUI::Start()
{
	return true;
}

void EnemyHPUI::Update()
{
}

void EnemyHPUI::Render(RenderContext& rc)
{
}

/*****************************************************************
///ボスのHPUI
******************************************************************/

BossHPUI::BossHPUI()
{
}

BossHPUI::~BossHPUI()
{
}

bool BossHPUI::Start()
{
	return true;
}

void BossHPUI::Update()
{
}

void BossHPUI::Render(RenderContext& rc)
{
}
