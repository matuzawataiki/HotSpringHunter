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

	bool GetGuardFlag(){						//ガードフラッグのゲッター。
		return m_guardFlag;
	}
private:
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;

	Vector3 m_directionGap = Vector3::Zero;				//向きの差分。

	bool m_guardFlag = false;				//ガードのフラッグ。
};

