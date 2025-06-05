#include "stdafx.h"
#include "UI.h"

#include "Player.h"
#include "Bear.h"

namespace
{
	const float PLAYER_HP_FRAME_WIDTH = 700.0f;    //プレイヤーのフレームの横
	const float PLAYER_HP_FRAME_HEIGHT = 140.0f;   //プレイヤーのフレームの縦
	const float PLAYER_HP_BAR_WIDTH = 600.0f;      //プレイヤーのバーの横
	const float PLAYER_HP_BAR_HEIGHT = 50.0f;      //プレイヤーのバーの縦

	const Vector2 PLAYER_HP_FRAME_POS = Vector2{ -590.0f,-460.0f };  //プレイヤーのフレームの位置
	const Vector2 PLAYER_HP_BAR_POS = Vector2{ -850.0f,-500.0f };    //プレイヤーのバーの位置
	const Vector2 HP_PIVOT = Vector2{ 0.0f,0.0f };   //プレイヤーのHPバーのピボット位置

	const float BOSS_HP_FRAME_WIDTH = 900.0f;   //くまのフレームの横
	const float BOSS_HP_FRAME_HEIGHT = 135.0f;  //くまのフレームの縦
	const float BOSS_HP_BAR_WIDTH = 800.0f;    //くまのバーの横
	const float BOSS_HP_BAR_HEIGHT = 50.0f;    //くまのバーの縦

	const Vector2 BOSS_HP_FRAME_POS = Vector2{ 0.0f,450.0f };  //くまのフレームの位置
	const Vector2 BOSS_HP_BAR_POS = Vector2{ -350.0f,410.0f };  //くまのバーの位置
}

UI::UI()
{
}

UI::~UI()
{
}

bool UI::Start()
{
	//プレイヤーのUI
	NewGO<PlayerHPUI>(0, "playerHPUI");
	//ボスのUI
	//NewGO<BossHPUI>(0, "bossHPUI");
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
	m_bear = FindGO<Bear>("bear");
	m_player = FindGO<Player>("player");

	m_BearHPBack.Init("Assets/modelData/UI/BossHPBack.dds", BOSS_HP_FRAME_WIDTH, BOSS_HP_FRAME_HEIGHT);
	m_BearHPBack.SetPosition(BOSS_HP_FRAME_POS);
	m_BearHPBack.Update();

	m_BearHPBar.Init("Assets/modelData/UI/BossHPBar.dds", BOSS_HP_BAR_WIDTH, BOSS_HP_BAR_HEIGHT);
	m_BearHPBar.SetPosition(BOSS_HP_BAR_POS);
	m_BearHPBar.SetPivot(HP_PIVOT);
	m_BearHPBar.Update();

	m_BearHPFrame.Init("Assets/modelData/UI/BossHPFrame.dds", BOSS_HP_FRAME_WIDTH, BOSS_HP_FRAME_HEIGHT);
	m_BearHPFrame.SetPosition(BOSS_HP_FRAME_POS);
	m_BearHPFrame.Update();

	return true;
}

void BossHPUI::Update()
{
	UpdateBarWidth();
}

void BossHPUI::UpdateBarWidth()
{
	m_BearHPBar.SetScale(Vector2{ m_bear->GetBearHP() / m_bear->GetBearMAXHP(),1.0f });
	m_BearHPBar.Update();
}

void BossHPUI::Render(RenderContext& rc)
{
	m_BearHPBack.Draw(rc);
	m_BearHPBar.Draw(rc);
	m_BearHPFrame.Draw(rc);
}
