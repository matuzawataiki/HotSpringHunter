#pragma once
class Result;

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
	Result* m_result = nullptr;
	SpriteRender m_gameClearModel;
};

