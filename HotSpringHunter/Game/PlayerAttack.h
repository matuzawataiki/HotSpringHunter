#pragma once
class PlayerAttack :public IGameObject
{
public:
	PlayerAttack();
	~PlayerAttack();

	bool Start()override;
	void Update()override;
	void WeakAttack();
	void Render(RenderContext& rc)override;
};

