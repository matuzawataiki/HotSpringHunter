#include "stdafx.h"
#include "ProjectileManager.h"
#include "Player.h"
#include "collision/CollisionObject.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
}

void ProjectileManager::DeleteProjectile(CollisionObject* collisionObject)
{
	for (std::vector<CollisionObject*>::iterator it = m_collisionObject.begin();
		it < m_collisionObject.begin();
		it++
	){
		if (*it == collisionObject) {
			m_collisionObject.erase(it);
			return;
		}
	}
}

bool ProjectileManager::IsHit(CharacterController* characterController)
{
	Character::Player* player = FindGO<Character::Player>("player");
	for (auto collision : m_collisionObject) {
		if (collision->IsHit(*characterController)){
			if (!(player->GetPowerUpSelect() == 1 &&
				player->GetUpgradeSelect() == 2)
				) {
				collision->Dead();
			}
			return true;
		}
	}
	return false;
}

bool ProjectileManager::IsChargeHit(CharacterController* characterController)
{
	if (m_chargeCollision->IsHit(*characterController)) {
		return true;
	}
	return false;
}
