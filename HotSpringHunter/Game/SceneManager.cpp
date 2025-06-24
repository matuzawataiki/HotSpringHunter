#include "stdafx.h"
#include "SceneManager.h"
#include "EnemyManager.h"
#include "EnemySpawner.h"
#include "GameClear.h"
#include "GameCamera.h"
#include "Player.h"
#include "Bear.h"
#include "BackGround/StageManager.h"
#include "BackGround/StageManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	//最初のシーン状態
	m_sceneState = EnGameScene::enStartArea;
	m_sceneState = EnGameScene::enStartArea;

	return true;
}

void SceneManager::Update()
{
	InGameSceneManage();
}

/// <summary>
/// ゲーム中のシーンを管理します。
/// </summary>
void SceneManager::InGameSceneManage()
{
	EnemyManager* enemyManager = FindGO<EnemyManager>("enemyManager");
	m_stageManager	= FindGO<StageManager>("stageManager");

	//入出検知オブジェクトに触れたらにゲームシーンを変更する
	//エリア内の敵をすべて倒したらクリアにする
	switch (m_sceneState)
	{
	case enStartArea:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enBattleStage1).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enBattleArea1;
			SwitchingScenes();
		}
		break;

	case enBattleArea1:
		if (!enemyManager->IsEnemy()) {
			m_sceneState = EnGameScene::enBattleArea1Clear;
			SwitchingScenes();
		}
		break;

	case enBattleArea1Clear:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enBattleStage2).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enBattleArea2;
			SwitchingScenes();
		}
		break;

	case enBattleArea2:
		if (!enemyManager->IsEnemy()) {
			m_sceneState = EnGameScene::enBattleArea2Clear;
			SwitchingScenes();
		}
		break;

	case enBattleArea2Clear:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enBossStage).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enBossArea;
			SwitchingScenes();
		}
		break;

	case enBossArea:
		if (!enemyManager->IsBoss()) {
			m_sceneState = EnGameScene::enDefeatedBoss;
			SwitchingScenes();
		}
		break;

	case enDefeatedBoss:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enGoalStage).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enGoalArea;
			SwitchingScenes();
		}
		break;

	case enGoalArea:
		break;

	default:
		break;
	}
	
}

/// <summary>
/// シーンを切り替える処理を行います。
/// </summary>
void SceneManager::SwitchingScenes()
{
	m_enemySpawner	= FindGO<EnemySpawner>("enemySpawner");
	m_gameCamera	= FindGO<GameCamera>("gameCamera");
	m_player		= FindGO<Character::Player>("player");
	m_stageManager = FindGO<StageManager>("stageManager");


	switch (m_sceneState)
	{
	case enStartArea:
		break;

	case enBattleArea1:
		m_enemySpawner->TriggerEnemySpawn(EnGameScene::enBattleArea1);
		m_stageManager->UpFence(StageManager::enBattleStage1);
		break;

	case enBattleArea1Clear:
		m_stageManager->DeleteFence(StageManager::enBattleStage1);
		break;

	case enBattleArea2:
		m_enemySpawner->TriggerEnemySpawn(EnGameScene::enBattleArea2);
		m_stageManager->UpFence(StageManager::enBattleStage2);
		break;

	case enBattleArea2Clear:
		m_stageManager->DeleteFence(StageManager::enBattleStage2);
		break;

	case enBossArea:
		m_enemySpawner->TriggerEnemySpawn(EnGameScene::enBossArea);
		m_stageManager->UpFence(StageManager::enBossStage);
		break;

	case enDefeatedBoss:
		m_stageManager->DeleteFence(StageManager::enBossStage);
		break;

	case enGoalArea:
		m_isToGoal = true;
		break;


	default:
		break;
	}

}
