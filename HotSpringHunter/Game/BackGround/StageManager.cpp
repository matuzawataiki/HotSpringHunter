#include "stdafx.h"
#include "StageManager.h"
#include "BackGround/Stage.h"

bool StageManager::Start()
{
	m_stageLevel.Init("Assets/stage/Level/LevelStage.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Start") == true) {
			m_stage[EnStageName::enStartStage] = NewGO<Stage>(0, "startStage");
			m_stage[EnStageName::enStartStage]->Init(Stage::enStart, objData);
			return true;
		}

		if (objData.EqualObjectName(L"Battle1") == true) {
			m_stage[EnStageName::enBattleStage1] = NewGO<Stage>(0, "battleStage1");
			m_stage[EnStageName::enBattleStage1]->Init(Stage::enBattle, objData);
			return true;
		}

		if (objData.EqualObjectName(L"Battle2") == true) {
			m_stage[EnStageName::enBattleStage2] = NewGO<Stage>(0, "battleStage2");
			m_stage[EnStageName::enBattleStage2]->Init(Stage::enBattle, objData);
			return true;
		}

		if (objData.EqualObjectName(L"Boss") == true) {
			m_stage[EnStageName::enBossStage] = NewGO<Stage>(0, "bossStage");
			m_stage[EnStageName::enBossStage]->Init(Stage::enBoss, objData);
			return true;
		}

		if (objData.EqualObjectName(L"Goal") == true) {
			m_stage[EnStageName::enGoalStage] = NewGO<Stage>(0, "goalStage");
			m_stage[EnStageName::enGoalStage]->Init(Stage::enGoal, objData);
			return true;
		}

		return false;
		});

	return true;
}

void StageManager::Update()
{

}
