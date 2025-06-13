#pragma once
class Title;
class GameOverResult;

class GameOver:public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start()override;
	void Update()override;
	void SwitchResult();
	void Render(RenderContext& rc)override;

private:
	GameOverResult* m_overResult = nullptr;

	SpriteRender m_overModel;
};

