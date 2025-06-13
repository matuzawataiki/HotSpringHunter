#pragma once
class Game;
class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start()override;
	void Update();
	void SwitchGame();
	void Render(RenderContext& rc)override;

private:
	Game* m_game = nullptr;
	SpriteRender m_titleModel;
};

