#include "stdafx.h"
#include "StageManager.h"
#include "BackGround/Stage.h"

StageManager::~StageManager()
{
	DeleteGO(m_startStage);
	for (int i = 0; i < 2; i++)
	{
		DeleteGO(m_battleStage[i]);
	}
	DeleteGO(m_BossStage);
	DeleteGO(m_goolStage);
}

bool StageManager::Start()
{
	int a = 0;
	m_stageLevel.Init("Assets/stage/Level/LevelStage.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Start") == true) {
			m_startStage = NewGO<Stage>(0, "startStage");
			m_startStage->Init(enStart, objData);
			return true;
		}

		if (objData.EqualObjectName(L"Battle") == true) {
			m_battleStage[a] = NewGO<Stage>(0, "battleStage");
			m_battleStage[a]->Init(enBattle, objData);
			a++;
			return true;
		}

		if (objData.EqualObjectName(L"Boss") == true) {
			m_BossStage = NewGO<Stage>(0, "bossStage");
			m_BossStage->Init(enBoss, objData);
			return true;
		}

		if (objData.EqualObjectName(L"Gool") == true) {
			m_goolStage = NewGO<Stage>(0, "goolStage");
			m_goolStage->Init(enGool, objData);
			return true;
		}

		return false;
		});

	return true;
}

void StageManager::Update()
{

}
