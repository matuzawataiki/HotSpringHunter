#pragma once
class Title;
class GameOver:public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start()override;
	void Update()override;
	void SwitchTitle();
	void Render(RenderContext& rc)override;

private:
	Title* m_title = nullptr;

	SpriteRender m_overModel;
};

