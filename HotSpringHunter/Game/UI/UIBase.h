#pragma once

class UIBase:public IGameObject
{
public:
	UIBase();
	virtual ~UIBase();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
};