#pragma once
class Stage;
class StageManager:public IGameObject
{
public:
	StageManager(){}
	~StageManager();

	bool Start()override;
	void Update()override;


	int playerPos = 0;

private:

	LevelRender m_stageLevel;
	Stage* m_startStage = nullptr;
	Stage* m_goolStage = nullptr;
	Stage* m_BossStage = nullptr;
	Stage* m_battleStage[2] = {nullptr,nullptr};
};