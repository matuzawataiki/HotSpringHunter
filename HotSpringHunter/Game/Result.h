#pragma once
class Title;
class Result :public IGameObject
{
public:
	Result();
	~Result();
	bool Start()override;
	void Update()override;
	void SwitchTitle();
	void Render(RenderContext& rc)override;

private:
	SpriteRender m_resultModel;

	Title* m_title = nullptr;
};

