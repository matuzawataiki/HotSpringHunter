#pragma once
class AttackTips;

class GameRule : public IGameObject
{
public:
	GameRule();
	~GameRule();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

private:
	AttackTips* m_attackTips = nullptr;
	SpriteRender m_ruleModel;
};

