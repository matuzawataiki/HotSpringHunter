#pragma once
namespace nsK2Engine
{
	class CollisionObject;
}

class ProjectileManager:public IGameObject
{
public:
	ProjectileManager();
	~ProjectileManager();

	void AddProjectile(CollisionObject* collisionObject) { m_collisionObject.push_back(collisionObject); }
	void DeleteProjectile(CollisionObject* collisionObject);

	bool IsHit(CharacterController* characterController);
	bool IsChargeHit(CharacterController* characterController);


private:
	std::vector<CollisionObject*> m_collisionObject;
	CollisionObject* m_chargeCollision;

};

