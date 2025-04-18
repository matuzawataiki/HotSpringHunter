#pragma once

class Player;
class PlayerGuard;
class GameOver;
class Game;
class PlayerHealth:public IGameObject
{
public:
	PlayerHealth();
	~PlayerHealth();
	bool Start()override;
	void Update()override;
	void Hit(float reduce);
	void TakeDamage();				//攻撃被弾。
	void DisplayHP();
	void Render(RenderContext& rc)override;
private:
	Player* m_player = nullptr;
	PlayerGuard* m_playerGuard = nullptr;
	GameOver* m_GameOver = nullptr;
	Game* m_game = nullptr;

	float m_playerHP = 100.0f;				//player体力。
	bool m_takeAttack = false;			//攻撃を受けたかのフラッグ。

	//チャージ量表示（仮）
	FontRender m_HPRender;
	wchar_t m_HPText[100];
};

