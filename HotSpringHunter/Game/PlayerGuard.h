#pragma once
class Player;
class Enemy;
class PlayerGuard:public IGameObject
{
public:
	PlayerGuard();
	~PlayerGuard();

	bool Start()override;
	void Update()override;
	void GuardDirection();						//ガードの方向を加味。
	void Render(RenderContext& rc)override;

	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;

	Vector3 m_playerDirection = Vector3::Zero;			//player向き。
	Vector3 m_enemyDirection = Vector3::Zero;			//enemy向き。
	Vector3 m_directionGap = Vector3::Zero;				//向きの差分。

	bool m_directionFlag = false;				//ガード方向のフラッグ。
};

