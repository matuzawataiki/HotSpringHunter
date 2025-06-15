#pragma once
class Game;

class GameController:public IGameObject
{
public:
	GameController();
	~GameController();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc)override;

private:
	Game* m_game = nullptr;
	SpriteRender m_controllerModel;
};

