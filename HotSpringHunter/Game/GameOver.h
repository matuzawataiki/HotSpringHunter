#pragma once
#include "Scene/IScene.h"

class Title;
class GameOverResult;
class BattleManager;

class GameOver :public IScene
{
	appScene(GameOver);

public:
	GameOver();
	~GameOver();
	bool Start()override;
	void Update()override;
	void SwitchResult();
	void Render(RenderContext& rc)override;

	/// <summary>
	/// リザルトの時間を設定
	/// </summary>
	inline void SetResultTime(const float time) { m_resultTime = time; }

private:
	GameOverResult* m_overResult   = nullptr;
	BattleManager* m_battleManager = nullptr;

	SpriteRender    m_overModel;

	float m_resultTime		= 0.0f; //リザルトタイムの受け皿
	float m_swtchDeltaTime	= 0.0f;
};

