#pragma once
class StageManager : public IGameObject
{
public:
	StageManager();
	~StageManager();

	bool Start();

private:
	LevelRender m_stageLevel_1;
	LevelRender m_RunLevel_1;
	LevelRender m_RunLevel_2;
	LevelRender m_BattleLevel_1;
	LevelRender m_BossBattleLevel_1;
	LevelRender m_GoolLevel_1;
};

