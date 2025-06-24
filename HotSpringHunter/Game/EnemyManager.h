#pragma once
namespace Character {
	class Player;
}
namespace Enemy {
	class PoisonSnake;
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
	void Update()override;
	//プレイヤーと最も近い敵を検索し、その方向ベクトルを計算
	Vector3 CalcToNearestEnemyVec(const Vector3& playerPos);
	//エネミーのフォーメーション
	void EnemyFormation();

	/// <summary>
	/// エネミーリストにヘビを登録
	/// </summary>
	/// <param name="snake">ヘビのインスタンス</param>
	void SetSnake(SnakeEnemy* snake) {
		m_snakes.push_back(snake);
	}

	/// <summary>
	/// エネミーリストに毒ヘビを登録
	/// </summary>
	/// <param name="poisonSnake">毒ヘビのインスタンス</param>
	void SetPoisonSnake(Enemy::PoisonSnake* poisonSnake) {
		m_poisonSnake.push_back(poisonSnake);
	}

	/// <summary>
	/// エネミーリストにイノシシを登録
	/// </summary>
	/// <param name="wildBoar"></param>
	void SetWildBoar(WildBoar* wildBoar) {
		m_wildBoars.push_back(wildBoar);
	}

	/// <summary>
	/// エネミーリストからエビを削除
	/// </summary>
	/// <param name="snake">ヘビのインスタンス</param>
	void DeleteEnemy(SnakeEnemy* snake) {
		for (std::vector<SnakeEnemy*>::iterator it = m_snakes.begin();
			it != m_snakes.end();
			it++
			) {
			if (*it == snake) {
				m_snakes.erase(it);
				return;
			}
		}
	}
	/// <summary>
	/// エネミーリストから毒ヘビを削除
	/// </summary>
	/// <param name="poisonSnake">毒ヘビのインスタンス</param>
	void DeleteEnemy(Enemy::PoisonSnake* poisonSnake) {
		for (std::vector<Enemy::PoisonSnake*>::iterator it = m_poisonSnake.begin();
			it != m_poisonSnake.end();
			it++
			) {
			if (*it == poisonSnake) {
				m_poisonSnake.erase(it);
				return;
			}
		}
	}
	/// <summary>
	/// エネミーリストからイノシシを削除
	/// </summary>
	/// <param name="wildBoar">イノシシのインスタンス</param>
	void DeleteEnemy(WildBoar* wildBoar) {
		for (std::vector<WildBoar*>::iterator it = m_wildBoars.begin();
			it != m_wildBoars.end();
			it++
			) {
			if (*it == wildBoar) {
				m_wildBoars.erase(it);
				return;
			}
		}
	}

	/// <summary>
	/// ボスを登録
	/// </summary>
	/// <param name="bear"></param>
	void SetBoss(Bear* bear) {
		m_bear = bear;
	}
	/// <summary>
	/// ボスを削除
	/// </summary>
	void DeleteBoss() {
		m_bear = nullptr;
	}

	/// <summary>
	/// 敵がいるかどうか
	/// </summary>
	/// <returns>いる場合はtrueを返す</returns>
	bool IsEnemy();

	/// <summary>
	/// ボスがいるかどうか
	/// </summary>
	/// <returns>いる場合はtrueを返す</returns>
	bool IsBoss() {
		if (m_bear != nullptr) {
			return true;
		}
		return false;
	}

private:

	std::vector<SnakeEnemy*>			m_snakes;				//ヘビの配列
	std::vector<WildBoar*>				m_wildBoars;			//イノシシの配列
	std::vector<Enemy::PoisonSnake*>	m_poisonSnake;			//毒ヘビの配列
	Bear* m_bear = nullptr;


	EnemySpawner*m_enemySpawner = nullptr;
	Character::Player* m_player = nullptr;
};

