#include "stdafx.h"
#include "PlayerHealth.h"
#include "Player.h"
#include "PlayerGuard.h"

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

}

/// <summary>
/// 攻撃を受ける。
/// </summary>
/// <param name="reduce"></param>ダメージ量。
void PlayerHealth::TakeDamage(int reduce)
{
	//攻撃を受けた時。
	if (m_takeAttack == true && m_playerGuard->GetGuardFlag() == false) {
		//HPを減らす。
		m_playerHP -= reduce;
	}

	//まだ生きているとき。
	if (m_playerHP >= 1) {
		m_player->m_animationState = m_player->enHit;
	}
	//HPが0になったとき。
	else {
		m_playerHP = 0;
		m_player->m_animationState = m_player->enDeath;
	}
}

void PlayerHealth::Render(RenderContext& rc)
{

}