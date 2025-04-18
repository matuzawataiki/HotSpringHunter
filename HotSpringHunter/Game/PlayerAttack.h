#pragma once
class Player;
class SnakeEnemy;
class PlayerAttack :public IGameObject
{
public:
	PlayerAttack();
	~PlayerAttack();

	bool Start()override;
	void Update()override;
	void WeakAttack();					//弱攻撃。
	void MakeCollision();				//コリジョン生成。
	void Render(RenderContext& rc)override;

	CollisionObject* m_collision = nullptr;
private:

	Player* m_player = nullptr;	
};

