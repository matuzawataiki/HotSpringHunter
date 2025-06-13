#include "stdafx.h"
#include "SceneManager.h"
#include "FenceManager.h"
#include "EnemySpawner.h"
#include "GameClear.h"
#include "GameCamera.h"
#include "Player.h"
#include "Bear.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	m_fenceManager = NewGO<FenceManager>(0, "fenceManager");

	//最初のシーン状態
	m_sceneState = EnGameScene::enStart;

	return true;
}

void SceneManager::Update()
{
	InGameSceneManage();
	SwitchingScenes();
}

/// <summary>
/// ゲーム中のシーンを管理します。
/// </summary>
void SceneManager::InGameSceneManage()
{
	m_player		= FindGO<Character::Player>("player");
	m_bear			= FindGO<Bear>("bear");

	//プレイヤーの現在地でゲームシーンを変更する
	if (m_sceneState == EnGameScene::enStart && m_player->GetPlayerPos().z >= 1000.0f) {
		m_sceneState = EnGameScene::enObstacleArea1;
		m_isExecuted = false;
	}
	if (m_sceneState == EnGameScene::enObstacleArea1 && m_player->GetPlayerPos().z >= 9500.0f) {
		m_sceneState = EnGameScene::enBattleArea1;
		m_isExecuted = false;
	}
	if (m_sceneState == EnGameScene::enBattleArea1 && m_bear->GetBearHP() <= 0.0f) {
		m_sceneState = EnGameScene::enObstacleArea2;
		m_isExecuted = false;
	}
	if (m_sceneState == EnGameScene::enObstacleArea2 && m_player->GetPlayerPos().z >= 12500.0f) {
		//仮
		m_sceneState = EnGameScene::enGoal;
		m_isExecuted = false;
	}
	if (m_sceneState == EnGameScene::enGoal && m_isClearFrag) {
		if (m_gameClear == nullptr) {
			m_gameClear = NewGO<GameClear>(0, "GameClear");
		}			
	}
}

/// <summary>
/// シーンを切り替える処理を行います。
/// </summary>
void SceneManager::SwitchingScenes()
{
	//1回実行したなら実行しない
	if (m_isExecuted) {
		return;
	}

	m_enemySpawner	= FindGO<EnemySpawner>("enemySpawner");
	m_gameCamera	= FindGO<GameCamera>("gameCamera");
	m_player		= FindGO<Character::Player>("player");

	switch (m_sceneState) {
	case EnGameScene::enStart:

		m_fenceManager->toFenceInactive();

		break;
	case EnGameScene::enObstacleArea1:

		m_enemySpawner->TriggerEnemySpawn(EnGameScene::enObstacleArea1);


		break;
	case EnGameScene::enBattleArea1:

		m_fenceManager->toFenceActive();
		m_enemySpawner->TriggerEnemySpawn(EnGameScene::enBattleArea1);
		m_gameCamera->SwitchBattleMode();

		break; 
	case EnGameScene::enObstacleArea2:

		m_fenceManager->toFenceInactive();

		break; 
	case EnGameScene::enBattleArea2:
		break; 
	case EnGameScene::enDefeatedBoss:
		break;
	case EnGameScene::enGoal:

		//プレイヤーをゴールに強制移動
		m_isToGoal = true;		

		break;
	default:
		break;
	}

	//実行済みにする
	m_isExecuted = true;
}
