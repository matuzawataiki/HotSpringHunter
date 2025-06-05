#include "stdafx.h"
#include "EnemySpawn.h"
#include "Result.h"
#include "SnakeEnemy.h"
#include "Bear.h"

//#include <time.h>	//Random

//定数->無名空間(namespace)で定義
namespace 
{
	//敵の最大数.10
	const int MAX_ENEMY_NUM = 10;

	//最初の敵の出現.4
	const int FAST_SNAKE_NUM = 4;
	const int SECOND_SNAKE_NUM = 6;
}

EnemySpawn::EnemySpawn()
{
	//敵を10体生成
	/*for (int i = 0; i < 10; i++)
	{
		m_snakeEnemy[i] = NewGO<SnakeEnemy>(0, "snakeenemy");
	}*/
}

EnemySpawn::~EnemySpawn()
{
	for (int i = 0; i < 6; i++) {
		DeleteGO(m_snakeEnemy[i]);
	}
	NewGO<Result>(0, "result");
	//m_nowEnemyNum = ENEMY_SNAKE_NUM;
}

bool EnemySpawn::Start()
{
	CreateBear();

	return true;
}

/// <summary>
/// クマ生成（仮）
/// </summary>
void EnemySpawn::CreateBear()
{
	for (int i = 0; i < 2; i++) {
		m_bear[i] = NewGO<Bear>(0, "bear");
		m_bear[i]->SetBearPos({ i * 200.0f, 0.0f, 500.0f });
	}
}

void EnemySpawn::Update()
{
	//ウェーブの切り替わり
	bool switchWave = false;
	//敵が死んでいる数
	int enemyDeath = 0;

	EnemyState();

	EnemyPos();
}

void EnemySpawn::EnemyState()
{
	switch (m_enemyState)
	{
	case waveSpawn01:
		for (int i = 0; i < FAST_SNAKE_NUM; i++)
		{
			m_snakeEnemy[i] = NewGO<SnakeEnemy>(0, "snakeEnemy");

			m_snakeEnemy[i]->SetIsSpawn(true);

			//座標
			float distance = 0.0f;
			distance = 100.0f * i;
			m_snakeEnemy[i]->SetSnakePos({ distance,0.0f,m_zPos });
		}
		m_enemyState = wave01;
		//現在の敵の数
		m_nowEnemyNum = FAST_SNAKE_NUM;
		break;

	case wave01:
		if (m_nowEnemyNum == 0)
		{
			m_enemyState = waveSpawn02;
		}
		break;

	case waveSpawn02:
		for (int i = 0; i < SECOND_SNAKE_NUM; i++)
		{
			m_snakeEnemy[i] = NewGO<SnakeEnemy>(0, "snakeEnemy");

			m_snakeEnemy[i]->SetIsSpawn(true);

			//座標
			float distance = 0.0f;
			distance = 100.0f * i;
			m_snakeEnemy[i]->SetSnakePos({ distance,0.0f,m_zPos });
		}
		//現在の敵の数
		m_nowEnemyNum = SECOND_SNAKE_NUM;

		m_enemyState = wave02;
		break;

	case wave02:
		if (m_nowEnemyNum <= 0) {
			DeleteGO(this);
		}
		break;

	default:
		break;
	}
}

//座標
void EnemySpawn::EnemyPos()
{
	//float distance = 0.0f;
	//for (int i = 0; i < MAX_ENEMY_NUM; i++)
	//{
	//	//スポーン場所を作る
	//	if (!m_snakeEnemy[i]->m_isSpawn) 
	//	{
	//		m_snakeEnemy[i]->m_isSpawn = true;
	//		distance = 100.0f * i;
	//		m_snakeEnemy[i]->m_snakePos = { distance ,0.0f,500.0f };
	//		m_snakeEnemy[i]->m_snakeController->SetPosition(m_snakeEnemy[i]->m_snakePos);
	//	}
	//}
}

void EnemySpawn::Render(RenderContext& rc)
{

}
