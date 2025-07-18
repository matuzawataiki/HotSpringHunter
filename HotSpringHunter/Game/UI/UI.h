#pragma once
namespace Character {
	class Player;
}
class Bear;

class UI : public IGameObject
{
public:
	UI();
	~UI();

	bool Start()override;
	void Update()override;

	void Render(RenderContext& rc)override;

private:

	Bear* m_bear = nullptr;

	bool m_isBearHPBarCreated = false;			//クマのHPバーを描画したか
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

	Character::Player* m_player = nullptr;
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
	void UpdateBarWidth();
	void DeleteUI();
	void Render(RenderContext& rc)override;

private:
	Bear* m_bear = nullptr;
	Character::Player* m_player = nullptr;

	SpriteRender m_BearHPFrame;
	SpriteRender m_BearHPBar;
	SpriteRender m_BearHPBack;

	float m_BearHPBarWidth = 0.0f;
};

