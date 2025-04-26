#include "stdafx.h"
#include "playerGuard.h"
#include "Player.h"
#include "SnakeEnemy.h"

namespace {
	const float GUARD_TOLERANCE = 0.3f;				//ガード方向の許容角度。
}
PlayerGuard::PlayerGuard()
{
}

PlayerGuard::~PlayerGuard()
{
}

bool PlayerGuard::Start()
{
	m_player = FindGO<Player>("player");
	m_snakeEnemy = FindGO<SnakeEnemy>("snakeEnemy");

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
	/*
	Vector3 enemyPos = m_enemy->GetPosition();
	Vector3 playerPos = m_player->GetPosition();
	Vector3 toEnemyDirection = playerPos - enemyPos;
	toEnemyDirection.Normalize();
	Vector3 playerDirection = m_player->GetPlayerDirection();
	Vector3 guard = Dot(playerDirection,toEnemyDirection)

	if(guard <= GUARD_TOLERANCE){
		m_guardFlag = true;
	}	
	else {
		m_guardFlag = false;
	}
	*/
	m_player->m_animationState = m_player->enGuardStart;
}

void PlayerGuard::Render(RenderContext& rc) 
{

}