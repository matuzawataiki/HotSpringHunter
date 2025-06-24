#include "stdafx.h"
#include "EnemySpawner.h"
#include "EnemyManager.h"
#include "BackGround/StageManager.h"
#include "SceneManager.h"
#include "Player.h"
#include "SnakeEnemy.h"
#include "WildBoar.h"
#include "Bear.h"
#include "Enemy/PoisonSnake/PoisonSnake.h"


EnemySpawner::EnemySpawner()
{
}

EnemySpawner::~EnemySpawner()
{
}

bool EnemySpawner::Start()
{
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
	StageManager* stageManager = FindGO<StageManager>("stageManager");
	EnemyManager* enemyManager = FindGO<EnemyManager>("enemyManager");

	SnakeEnemy* snake = nullptr;
	Enemy::PoisonSnake* poisonSnake = nullptr;
	WildBoar* wildBoar = nullptr;
	Bear* bear = nullptr;

	switch (scene) {
	case EnGameScene::enBattleArea1:
		for (int i = 0;i < 10;i++) {
			int random = rand() % 3;
			switch (random)
			{
			case 0:
				snake = NewGO<SnakeEnemy>(0, "snake");
				snake->SetSnakePos(stageManager->GetStageObject(StageManager::enBattleStage1).enemyTargetPos[i]);
				enemyManager->SetSnake(snake);
				break;

			case 1:
				poisonSnake = NewGO<Enemy::PoisonSnake>(0, "poisonSnake");
				poisonSnake->SetPosition(stageManager->GetStageObject(StageManager::enBattleStage1).enemyTargetPos[i]);
				enemyManager->SetPoisonSnake(poisonSnake);
				break;

			case 2:
				wildBoar = NewGO<WildBoar>(0, "wildBoar");
				wildBoar->SetWildBoarPos(stageManager->GetStageObject(StageManager::enBattleStage1).enemyTargetPos[i]);
				enemyManager->SetWildBoar(wildBoar);
				break;
			}
		}
		break;

	case EnGameScene::enBattleArea2:
		for (int i = 0;i < 10;i++) {
			int random = rand() % 3;
			switch (random)
			{
			case 0:
				snake = NewGO<SnakeEnemy>(0, "snake");
				snake->SetSnakePos(stageManager->GetStageObject(StageManager::enBattleStage2).enemyTargetPos[i]);
				enemyManager->SetSnake(snake);
				break;

			case 1:
				poisonSnake = NewGO<Enemy::PoisonSnake>(0, "poisonSnake");
				poisonSnake->SetPosition(stageManager->GetStageObject(StageManager::enBattleStage2).enemyTargetPos[i]);
				enemyManager->SetPoisonSnake(poisonSnake);
				break;

			case 2:
				wildBoar = NewGO<WildBoar>(0, "wildBoar");
				wildBoar->SetWildBoarPos(stageManager->GetStageObject(StageManager::enBattleStage2).enemyTargetPos[i]);
				enemyManager->SetWildBoar(wildBoar);
				break;
			}
		}
		break;

	case EnGameScene::enBossArea:
		bear = NewGO<Bear>(0, "bear");
		bear->SetBearPos(stageManager->GetStageObject(StageManager::enBossStage).bossPos);
		enemyManager->SetBoss(bear);
		break;

	default:
		break;
	}
}