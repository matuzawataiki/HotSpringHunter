#include "stdafx.h"
#include "EnemySpawner.h"
#include "EnemyManager.h"
#include "SceneManager.h"
#include "Player.h"

EnemySpawner::EnemySpawner()
{
}

EnemySpawner::~EnemySpawner()
{
}

bool EnemySpawner::Start()
{
	m_enemyManager	= FindGO<EnemyManager>("enemyManager");
	m_player		= FindGO<Character::Player>("player");

	return true;
}



void EnemySpawner::Update()
{
}

/// <summary>
/// エネミーを出す指示を出す
/// </summary>
void EnemySpawner::TriggerEnemySpawn(EnGameScene scene)
{
	switch (scene) {
	case EnGameScene::enBattleArea1:

		m_enemyManager->EnemyArrangement(EnEnemyType::enWildBoar, Vector3(800.0f, 30.0f, 10500.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enWildBoar, Vector3(-800.0f, 30.0f, 10500.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enBear, Vector3(0.0f, 30.0f, 11000.0f), Quaternion::Identity);

		break;
	default:
		break;
	}
}