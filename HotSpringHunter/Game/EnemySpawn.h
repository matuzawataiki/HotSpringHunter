#pragma once

class Game;
class SnakeEnemy;
class Bear;

class EnemySpawn : public IGameObject
{
public:
	EnemySpawn();
	~EnemySpawn();

	bool Start()override;
	//クマ生成（仮）
	void CreateBear();
	void Update()override;
	void EnemyState();
	void EnemyPos();		//座標

	void EnemyDecrease() {
		m_nowEnemyNum--;
	}

	void Render(RenderContext& rc)override;

	//インスタンス
	SnakeEnemy* m_snakeEnemyClass;
	SnakeEnemy* m_snakeEnemy[10];
	Bear* m_bear[3];
	
	//変数
	int m_nowEnemyNum = 0;		//現在の敵の数
	float m_timeEnemy = 0.0f;   
	float m_zPos = 500.0f;

	bool m_wave01 = true;
	bool m_wave02 = false;

	bool enemyDecrease = false;

	//起動中か
	enum 
	{
		waveSpawn01,
		wave01,
		waveSpawn02,
		wave02,
	};
	int m_enemyState = waveSpawn01;
};

