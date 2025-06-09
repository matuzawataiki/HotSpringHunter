#pragma once
namespace Character {
	class Player;
}
class SnakeEnemy;
class WildBoar;
class Bear;
class Player;

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
	//エネミーを出す指示を出す
	void TriggerEnemySpawn();
	//エネミーを配置
	void EnemyArrangement(EnEnemyType tipe, Vector3 pos, Quaternion rot);
	//エネミーが足りないときに追加
	auto CreateNewEnemy(EnEnemyType type);
	

private:

	std::vector<SnakeEnemy*>	m_snakes;				//ヘビの配列
	std::vector<WildBoar*>		m_wildBoars;			//イノシシの配列

	Bear*		m_bear			= nullptr;
	Character::Player*m_player	= nullptr;

	int m_snakeNumber			= 0;					//ヘビの総数
	int m_wildBoarNumber		= 0;					//イノシシの総数

	bool m_isArea01Spawned		= false;				//ウェーブ01の敵をを出現させたか
	bool m_isArea02Spawned		= false;				//ウェーブ02の敵をを出現させたか
};

