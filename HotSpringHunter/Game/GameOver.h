#pragma once
class Title;
class GameOverResult;
class SceneManager;

class GameOver :public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start()override;
	void Update()override;
	void SwitchResult();
	void Render(RenderContext& rc)override;

	float m_timer = 0.0f; //リザルトタイマーの受け皿

private:
	GameOverResult* m_overResult = nullptr;
	SceneManager* m_sceneManager = nullptr;

	SpriteRender    m_overModel;

	float m_swtchDeltaTime = 0.0f;

	
};

