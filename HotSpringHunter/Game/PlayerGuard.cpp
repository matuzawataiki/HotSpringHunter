#include "stdafx.h"
#include "playerGuard.h"
#include "Player.h"
#include "Enemy.h"

PlayerGuard::PlayerGuard()
{
}

PlayerGuard::~PlayerGuard()
{
}

bool PlayerGuard::Start()
{
	m_player = FindGO<Player>("player");
	m_enemy = FindGO<Enemy>("enemy");

	return true;
}

void PlayerGuard::Update()
{
	if (g_pad[0]->IsPress(enButtonX)) {
		GuardDirection();
	}
}

void PlayerGuard::GuardDirection()
{
	m_playerDirection = m_player->GetPlayerSpeed();
	//m_enemyDirection = m_enemy->GetEnemySpeed();

	m_playerDirection.y = 0.0f;
	m_enemyDirection.y = 0.0f;

	m_playerDirection.Normalize();
	m_enemyDirection.Normalize();

	m_enemyDirection * -1.0f;

	m_directionGap = m_playerDirection - m_enemyDirection;

	if (fabsf(m_directionGap.x) <= 0.3f && fabsf(m_directionGap.z) <= 0.3f) {
		m_directionFlag = true;
	}
	else {
		m_directionFlag = false;
	}
}

void PlayerGuard::Render(RenderContext& rc) 
{

}