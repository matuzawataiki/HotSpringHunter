#pragma once

class Player;
class PlayerGuard;
class PlayerHealth:public IGameObject
{
public:
	PlayerHealth();
	~PlayerHealth();
	bool Start()override;
	void Update()override;
	void TakeDamage(int reduce);				//攻撃被弾。
	void Render(RenderContext& rc)override;
private:
	Player* m_player = nullptr;
	PlayerGuard* m_playerGuard = nullptr;

	int m_playerHP = 100;				//player体力。
	bool m_takeAttack = false;			//攻撃を受けたかのフラッグ。
};

