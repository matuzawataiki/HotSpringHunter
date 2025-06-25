#include "stdafx.h"
#include "ProjectileManager.h"
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
	for (auto collision : m_collisionObject) {
		if (collision->IsHit(*characterController)) {
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
