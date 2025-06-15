#pragma once
#include "BackGround/StageObject.h"

enum EnStage
{
	enStart,
	enBattle,
	enBoss,
	enGool,
	enNum
};

class Stage : public IGameObject
{
public:
	Stage();
	~Stage();
	bool Start()override;
	void Update()override;

	void Init(EnStage stageNum, LevelObjectData& objData);
	void InitStage();
	void InitLevel();

	void NewGround(LevelObjectData& objData, Vector3& position);
	void NewHitBox(LevelObjectData& objData, Vector3& position);

private:
	LevelRender m_stageLevel;
	StageObject m_stageObject;

	Vector3 m_position = Vector3::Zero;

	bool isInit = false;

	EnStage m_stageNum = enNum;
	const char* m_levelFilePath = nullptr;
	const wchar_t* m_grondName = nullptr;
	const wchar_t* m_hitBoxName = nullptr;

};

