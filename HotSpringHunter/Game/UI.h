#pragma once

class Player;

class UI : public IGameObject
{
public:
	UI();
	~UI();

	bool Start()override;
	void Update()override;

	void Render(RenderContext& rc)override;
};

class PlayerHPUI : public IGameObject
{
public:
	PlayerHPUI();
	~PlayerHPUI();

	bool Start()override;
	void Update()override;
	void UpdateBarWidth();

	void Render(RenderContext& rc)override;

	Player* m_player = nullptr;
	SpriteRender m_playerHPFrame;
	SpriteRender m_playerHPBar;
	SpriteRender m_playerHPBack;

	float m_playerHPBarWidth = 0.0f;
};

class EnemyHPUI : public IGameObject
{
public:
	EnemyHPUI();
	~EnemyHPUI();

	bool Start()override;
	void Update()override;

	void Render(RenderContext& rc)override;
};

class BossHPUI : public IGameObject
{
public:
	BossHPUI();
	~BossHPUI();

	bool Start()override;
	void Update()override;

	void Render(RenderContext& rc)override;
};

