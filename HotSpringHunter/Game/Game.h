#pragma once
class Test;
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	Test* m_test = nullptr;
};

