#include "stdafx.h"
#include "SceneManager.h"
#include "FenceManager.h"
#include "EnemySpawner.h"
#include "GameClear.h"
#include "GameCamera.h"
#include "Player.h"
#include "Bear.h"
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
	m_player		= FindGO<Character::Player>("player");
	m_bear			= FindGO<Bear>("bear");
	m_stageManager	= FindGO<StageManager>("stageManager");

	//入出検知オブジェクトに触れたらにゲームシーンを変更する
	switch (m_sceneState)
	{
	case enStartArea:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enBattleStage1).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enBattleArea1;
			SwitchingScenes();
		}
		break;

	case enBattleArea1:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enBattleStage2).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enBattleArea2;
			SwitchingScenes();
		}
		break;

	case enBattleArea2:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enBossStage).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enBossArea;
			SwitchingScenes();
		}
		break;

	case enBossArea:
		break;

	case enDefeatedBoss:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enGoalStage).inOutHitBox->IsHit()) {
			m_sceneState = EnGameScene::enGoalArea;
			SwitchingScenes();
		}
		break;

	case enGoalArea:
		if (m_stageManager->GetStageObject(StageManager::EnStageName::enGoalStage).inOutHitBox->IsHit()) {
			if (m_gameClear == nullptr) {
				m_gameClear = NewGO<GameClear>(0, "GameClear");
			}
		}
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

	switch (m_sceneState) {
	case EnGameScene::enStartArea:
		break;

	case EnGameScene::enBattleArea1:
		m_enemySpawner->TriggerEnemySpawn(EnGameScene::enBattleArea1);
		break; 

	case EnGameScene::enBattleArea2:
		m_enemySpawner->TriggerEnemySpawn(EnGameScene::enBattleArea2);
		break; 

	case EnGameScene::enDefeatedBoss:
		break;

	case EnGameScene::enGoalArea:
		//プレイヤーをゴールに強制移動
		m_isToGoal = true;		
		break;

	default:
		break;
	}
}
