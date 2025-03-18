#pragma once
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	ModelRender m_model;
};

