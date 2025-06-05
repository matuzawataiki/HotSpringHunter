#pragma once

namespace Character {
	class Player;
}
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
	//点滅表示
	bool IsBlinkRender();
	//ステート変更のフラッグを返す
	bool ChangeFlag();

	//セッター
	//ステート変更フラッグを設定
	void SetChangeFlag(bool isChange) {
		m_isCanChange = isChange;
	}

	CollisionObject* m_enemyATCollision = nullptr;
	Character::Player* m_player = nullptr;

	bool m_isCanChange		= true;					//ステートを変えてもいいか
	bool m_isKnockSetFlag	= false;				//ノックバックをセットしたか

	float m_knockDecreased	= 1.0f;					//ノックバックの減速量
	float m_renderTime		= 0.0f;					//点滅表示の時間

	Vector3 m_knockBackVec = Vector3::Zero;			//ノックバックのベクトル
};