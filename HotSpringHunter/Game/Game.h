#pragma once
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	ModelRender m_playerModel;
	ModelRender m_mapModel;
	//SpriteRender m_testImage;
	//FontRender	m_testText;

	Vector3 m_cameraPos = Vector3(0.0f, 500.0f, -1000.0f);
};

