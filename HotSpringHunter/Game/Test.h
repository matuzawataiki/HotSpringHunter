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

	//SpriteRender m_testImage;
	//FontRender	m_testText;
	PointLight m_pointLight;
	SpotLight m_spotLight;
	HemisphereLight m_hemisphereLight;

};

