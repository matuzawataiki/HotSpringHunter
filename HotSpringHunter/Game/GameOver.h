#pragma once
class Title;
class Result;

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
	Result* m_result = nullptr;

	SpriteRender m_overModel;
};

