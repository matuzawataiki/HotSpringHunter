#pragma once
class Test:public IGameObject
{
public:
	Test();
	~Test();

private:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	ModelRender m_playerModel;
	ModelRender m_mapModel;
	//SpriteRender m_testImage;
	//FontRender	m_testText;
	PointLight m_pointLight;

	Vector3 m_cameraPos = Vector3(0.0f, 500.0f, -1000.0f);
};

