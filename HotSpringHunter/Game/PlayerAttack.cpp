#include "stdafx.h"
#include "PlayerAttack.h"
#include "Player.h"
#include "SnakeEnemy.h"
#include "collision/CollisionObject.h"

namespace {
	const float DAMEGE_AMOUNT = 25.0f;
}

PlayerAttack::PlayerAttack()
{
}

PlayerAttack::~PlayerAttack()
{
}

bool PlayerAttack::Start()
{
	m_player = FindGO<Player>("player");

	return true;
}

void PlayerAttack::Update()
{
	WeakAttack();
}

void PlayerAttack::WeakAttack()
{
	if (g_pad[0]->IsTrigger(enButtonY)) {
		m_player->m_animationState = m_player->enWeakAttack;
		SnakeEnemy* snakeEnemy = FindGO<SnakeEnemy>("snakeEnemy");
		if (snakeEnemy == nullptr) {
			return;
		}
		MakeCollision();		
		if (m_collision->IsHit(snakeEnemy->m_characterController)) {
			snakeEnemy->Hit();
		}
		DeleteGO(m_collision);
	}
}

void PlayerAttack::MakeCollision()
{
	//コリジョンオブジェクトを作成
	m_collision = NewGO<CollisionObject>(0,"weakAttack");
	Vector3 collisionPosition = m_player->GetPlayerPos();
	//座標をプレイヤーの少し前に設定
	collisionPosition += m_player->GetPlayerDir() * 100.0f;
	//球状のコリジョンを作成
	m_collision->CreateSphere(collisionPosition,
		Quaternion::Identity,
		150.0f);
}

void PlayerAttack::Render(RenderContext& rc)
{

}