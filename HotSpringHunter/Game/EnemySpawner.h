#pragma once

namespace Character {
	class Player;
}

class EnemyManager;
class EnemySpawner:public IGameObject
{
public:
	EnemySpawner();
	~EnemySpawner();

	bool Start()override;
	void Update()override;
	//エネミーを出す指示を出す
	void TriggerEnemySpawn();

private:

	EnemyManager*		m_enemyManager		= nullptr;
	Character::Player*	m_player			= nullptr;

	bool m_isArea01Spawned = false;				//ウェーブ01の敵をを出現させたか
	bool m_isArea02Spawned = false;				//ウェーブ02の敵をを出現させたか
};

