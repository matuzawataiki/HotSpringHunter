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
	int hoge;
}

/// <summary>
/// エネミーを出す指示を出す
/// </summary>
void EnemySpawner::TriggerEnemySpawn(EnGameScene Scene)
{

	switch (Scene) {
	case EnGameScene::enObstacleArea1:

		m_enemyManager->EnemyArrangement(EnEnemyType::enSnake, Vector3(-400.0f, 30.0f, 3000.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enSnake, Vector3(-600.0f, 30.0f, 3000.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enSnake, Vector3(400.0f, 30.0f, 5000.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enSnake, Vector3(600.0f, 30.0f, 5000.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enSnake, Vector3(-200.0f, 30.0f, 7000.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enSnake, Vector3(0.0f, 30.0f, 7000.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enSnake, Vector3(200.0f, 30.0f, 7000.0f), Quaternion::Identity);
		
		break;
	case EnGameScene::enBattleArea1:

		m_enemyManager->EnemyArrangement(EnEnemyType::enWildBoar, Vector3(800.0f, 30.0f, 10500.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enWildBoar, Vector3(-800.0f, 30.0f, 10500.0f), Quaternion::Identity);
		m_enemyManager->EnemyArrangement(EnEnemyType::enBear, Vector3(0.0f, 30.0f, 11000.0f), Quaternion::Identity);

		break;
	default:
		break;
	}
}