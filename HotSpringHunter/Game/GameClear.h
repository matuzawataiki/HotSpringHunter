#pragma once
class Result;
class SmokeEmitter;

enum ResultState
{
	enGameClear,
	enSmokeStep1,
	enSmokeStep2,
	enSmokeStep3,
	enSmokeStep4,
	enResult,
};

class GameClear:public IGameObject
{
	public:
	GameClear();
	~GameClear();

	bool Start() override;
	void Update() override;
	//void SwitchGame() override;
	void Render(RenderContext&rc) override;

public:
	Result*       m_clearResult      = nullptr;
	SmokeEmitter* m_smokeEmitter     = nullptr;

	Vector2 m_gameClearSize = Vector2(1.0f, 1.0f);

	SpriteRender* m_gameClearModel;
	SpriteRender* m_resultModel;

	int   m_resultState   = enGameClear; //ステート
	float m_elapsedTime   = 0.0f; //毎フレーム加算
	float m_timer = 0.0f;
};

