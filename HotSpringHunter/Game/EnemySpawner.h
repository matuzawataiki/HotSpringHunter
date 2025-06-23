#pragma once
#include "SceneManager.h"

namespace Character {
	class Player;
}

class EnemySpawner:public IGameObject
{
public:
	EnemySpawner();
	~EnemySpawner();

	bool Start()override;
	void Update()override;
	//エネミーを出す指示を出す
	void TriggerEnemySpawn(EnGameScene Scene);

private:
	Character::Player*	m_player			= nullptr;
};

