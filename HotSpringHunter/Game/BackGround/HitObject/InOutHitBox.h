#pragma once
#include "Player.h"
namespace Character {
	class Player;
}
class InOutHitBox:public IGameObject
{
public:
	InOutHitBox() { m_collision = new CollisionObject; }
	~InOutHitBox() { delete m_collision; delete m_player; }
	
	void init(Vector3 positon, Quaternion rotation, Vector3 scale);

	bool IsHit() {
		m_player = FindGO<Character::Player>("player");
		return m_collision->IsHit(m_player->m_playerCharaCon);
	}


private:
	ModelRender m_objectModel;
	CollisionObject* m_collision;
	Character::Player* m_player;
	
};

