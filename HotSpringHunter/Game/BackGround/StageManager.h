#pragma once
#include "BackGround/Stage.h"

class Stage;
class StageManager:public IGameObject
{
public:
	StageManager(){}
	~StageManager(){}

	bool Start()override;
	void Update()override;

	enum EnStageName
	{
		enStartStage,
		enBattleStage1,
		enBattleStage2,
		enBossStage,
		enGoalStage,
		enStageNum,
	};

	Stage* GetStage(EnStageName name) {
		return m_stage[name];
	}

	StageObject& GetStageObject(EnStageName name) {
		return m_stage[name]->GetStageObject();
	}

private:

	Stage* m_stage[enStageNum];
	LevelRender m_stageLevel;
};

