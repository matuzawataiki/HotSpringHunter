#pragma once
#include "Scene/IScene.h"

class Title;
class GameOverResult;
class BattleManager;

class GameOver :public IGameObject
{
	appScene(GameOver);

public:
	GameOver();
	~GameOver();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	bool CanChangeScene() const { return m_canChangeScene; }

private:
	GameOverResult* m_overResult   = nullptr;
	BattleManager* m_battleManager = nullptr;

	SpriteRender    m_overModel;

	float m_swtchDeltaTime	= 0.0f;

	bool m_canChangeScene = false;
};

