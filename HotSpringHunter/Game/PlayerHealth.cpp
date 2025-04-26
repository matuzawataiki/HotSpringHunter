#include "stdafx.h"
#include "PlayerHealth.h"
#include "Player.h"
#include "PlayerGuard.h"
#include "GameOver.h"
#include "Game.h"

PlayerHealth::PlayerHealth()
{

}

PlayerHealth::~PlayerHealth()
{

}

bool PlayerHealth::Start()
{
	m_player = FindGO<Player>("player");
	m_playerGuard = FindGO<PlayerGuard>("playerGuard");

	return true;
}

void PlayerHealth::Update()
{
	TakeDamage();
	DisplayHP();
}

void PlayerHealth::Hit(float reduce)
{
	//攻撃を受けた時。
	if (m_playerGuard->GetGuardFlag() == false) {
		//HPを減らす。
		m_playerHP -= reduce;
	}
}

/// <summary>
/// 攻撃を受ける。
/// </summary>
void PlayerHealth::TakeDamage()
{
	//まだ生きているとき。
	if (m_playerHP >= 1.0f) {
		m_player->m_animationState = m_player->enHit;
	}
	//HPが0になったとき。
	else {
		m_playerHP = 0.0f;
		m_GameOver = NewGO<GameOver>(0, "gameOver");
		DeleteGO(m_game);
		//m_player->m_animationState = m_player->enDeath;
	}
}

void PlayerHealth::DisplayHP()
{
	m_HPRender.SetScale(1.2);
	m_HPRender.SetPosition({ 800.0f,-400.0f, 0.0f });
	m_HPRender.SetColor(g_vec4Black);

	swprintf_s(m_HPText, 100, L"HP %f", float(m_playerHP));
	m_HPRender.SetText(m_HPText);
}

void PlayerHealth::Render(RenderContext& rc)
{
	m_HPRender.Draw(rc);
}