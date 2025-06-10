#pragma once
namespace Character {
	class Player;
}
class SnakeEnemy;
class WildBoar;
class Bear;
class Player;
class EnemySpawner;

enum class EnEnemyType {
	enSnake,
	enWildBoar,
	enBear,
};

class EnemyManager:public IGameObject
{
public:
	EnemyManager();
	~EnemyManager();
	bool Start()override;
	//エネミーをリストに登録
	void AddEnemy();
	void Update()override;
	//エネミーを配置
	void EnemyArrangement(const EnEnemyType tipe, const Vector3& pos, const Quaternion& rot);
	//エネミーが足りないときに追加
	auto CreateNewEnemy(const EnEnemyType type);
	/// プレイヤーと最も近い敵を検索し、その方向ベクトルを計算
	Vector3 CalcToNearestEnemyVec(const Vector3& playerPos);
	

private:

	std::vector<SnakeEnemy*>	m_snakes;				//ヘビの配列
	std::vector<WildBoar*>		m_wildBoars;			//イノシシの配列

	EnemySpawner*m_enemySpawner = nullptr;
	Bear*		 m_bear			= nullptr;
	Character::Player* m_player = nullptr;

	bool m_isArea01Spawned		= false;				//ウェーブ01の敵をを出現させたか
	bool m_isArea02Spawned		= false;				//ウェーブ02の敵をを出現させたか
};

