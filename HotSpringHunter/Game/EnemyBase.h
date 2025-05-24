#pragma once

class Player;
class PlayerChargeAttack;
class EnemySpawn;

class EnemyBase : public IGameObject
{
public:
	EnemyBase();
	~EnemyBase();

	bool Start() override;
	//追従
	Vector3 Tracking(Vector3 toPlayer);
	//近接攻撃
	void MeleeAttack(Vector3 position, Vector3 enemyDir,float damage);
	//近接攻撃のコリジョン生成
	void EnemyAttackCollision(Vector3 position,Vector3 enemyDir);
	//ヒット時のノックバック。
	Vector3 KnockBack(Vector3 enemyDir);
	//死亡
	void Death();
	//死亡時の吹っ飛び
	Vector3 DeathBlown(Vector3 enemyDir);
	//ステート変更のフラッグを返す
	bool ChangeFlag();

	CollisionObject* m_enemyATCollision = nullptr;
	Player* m_player = nullptr;

	bool m_isCanChange = true;						//ステートを変えてもいいか
	bool m_isKnockSetFlag = false;					//ノックバックをセットしたか

	float m_knockDecreased = 1.0f;					//ノックバックの減速量

	Vector3 m_knockBackVec = Vector3::Zero;			//ノックバックのベクトル
};