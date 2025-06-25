#include "stdafx.h"
#include "SceneManager.h"
#include "EnemyManager.h"
#include "EnemySpawner.h"
#include "GameClear.h"
#include "GameOver.h"
#include "GameCamera.h"
#include "Player.h"
#include "Bear.h"
#include "BackGround/StageManager.h"
#include "SoundEffect.h"

namespace
{
	//タイムの位置
	const Vector3 TIME_POS = { 740.0f,430.0f,0.0f };
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	m_soundEffect = FindGO<SoundEffect>("soundEffect");
	m_soundEffect->Play(enNomalBGM);

	//最初のシーン状態
	m_sceneState = EnGameScene::enStartArea;
	m_sceneState = EnGameScene::enStartArea;

	return true;
}

void SceneManager::Update()
{
	InGameSceneManage();
	GameTimeUpdate();
}

/// <summary>
/// ゲーム中のシーンを管理します。
/// </summary>
void SceneManager::InGameSceneManage()
{
	EnemyManager* enemyManager = FindGO<EnemyManager>("enemyManager");
	m_stageManager	= FindGO<StageManager>("stageManager");
	m_player = FindGO<Character::Player>("player");

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
			m_soundEffect->Play(enBossBGM);
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
  
  if (m_player->GetPlayerHP() == 0.0f)
	{
	  if (m_gameOver == nullptr)
		{
			m_gameOver = NewGO<GameOver>(0, "gameOver");
			m_gameOver->m_timer = m_gamePlayTime;
		}
	}
}

/// <summary>
/// シーンを切り替える処理を行います。
/// </summary>
void SceneManager::SwitchingScenes()
{
	m_enemySpawner	= FindGO<EnemySpawner>("enemySpawner");
	m_gameCamera	= FindGO<GameCamera>("gameCamera");
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
    m_gameClear = NewGO<GameClear>(0, "GameClear");
		m_gameClear-> m_timer = m_gamePlayTime;
		m_isToGoal = true;
		break;


	default:
		break;
	}

}

void SceneManager::GameTimeUpdate()
{
	if (m_isTimeOff == true)
	{
		m_gamePlayTime += g_gameTime->GetFrameDeltaTime();
	}

	wchar_t time[256];
	swprintf_s(time, 256, L"%d", int(m_gamePlayTime));
	
	m_timeRender.SetText(time);
	m_timeRender.SetPosition(TIME_POS);
	m_timeRender.SetScale(1.3f);
	m_timeRender.SetColor(g_vec4Black);

	if (m_sceneState == EnGameScene::enGoalArea || m_player->GetPlayerHP() <= 0.0f)
	{
		m_isTimeOff = false;
	}
}