#pragma once
#include "BackGround/Stage.h"

class Stage;
class StageManager:public IGameObject
{
public:
	StageManager(){}
	~StageManager();

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

	/// <summary>
	/// フェンスを出す
	/// </summary>
	/// <param name="name">フェンスを出すステージ</param>
	void UpFence(EnStageName name) {
		m_stage[name]->UpFence();
	}

	/// <summary>
	/// 前方のフェンスを消す
	/// </summary>
	/// <param name="name">フェンスを消すステージ</param>
	void DeleteFence(EnStageName name) {
		m_stage[name]->DeleteFence();
	}

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