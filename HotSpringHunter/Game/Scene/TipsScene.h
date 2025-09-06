#pragma once
#include "Scene/IScene.h"

class TipsScene : public IScene
{
	appScene(TipsScene);

public:
	TipsScene() {}
	~TipsScene();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	virtual bool RequestScene(uint32_t& id) override;

private:
	SpriteRender* m_tipsRender = nullptr;
	SpriteRender m_pressButtonRender;

	int m_currentTipsId = 0;
};

