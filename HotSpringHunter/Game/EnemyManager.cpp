#include "stdafx.h"
#include "EnemyManager.h"
#include "SnakeEnemy.h"
#include "WildBoar.h"
#include "Bear.h"
#include "Player.h"

namespace {
	const int ENEMY_NUM = 15;		//エネミーの数
	const Vector3 OFF_SCREEN_POS = { 0.0f,3000.0f,500.0f };		//出番が来てないときの敵を格納する位置
}

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

bool EnemyManager::Start()
{  
	AddEnemy();

	m_player = FindGO<Character::Player>("player");

	return true;
}

/// <summary>
/// エネミーをリストに登録
/// </summary>
void EnemyManager::AddEnemy()
{
	// 各エネミーを10体ずつNewGOして、リストに入れる
	//同時にエネミーを見えない位置へ
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		auto* snake = NewGO<SnakeEnemy>(0, "snakeEnemy");
		snake->SetSnakePos(OFF_SCREEN_POS);
		m_snakes.push_back(snake);
		m_snakeNumber++;

		auto* wildBoar = NewGO<WildBoar>(m_wildBoarNumber, "wildBoar");
		wildBoar->SetWildBoarPos(OFF_SCREEN_POS);
		m_wildBoars.push_back(wildBoar);
		m_wildBoarNumber++;
	}

	//クマは1体だけ
	m_bear = NewGO<Bear>(0, "bear");
	m_bear->SetBearPos(OFF_SCREEN_POS);
}

void EnemyManager::Update()
{
	TriggerEnemySpawn();
}

/// <summary>
/// エネミーを出す指示を出す
/// </summary>
void EnemyManager::TriggerEnemySpawn()
{
	//一旦は一定位置に到達したら
	if (m_player->GetPlayerPos().z >= 900.0f && !m_isArea01Spawned) {
		EnemyArrangement(EnEnemyType::enSnake, Vector3(-400.0f, 30.0f, 3000.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enSnake, Vector3(-600.0f, 30.0f, 3000.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enSnake, Vector3(400.0f, 30.0f, 5000.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enSnake, Vector3(600.0f, 30.0f, 5000.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enSnake, Vector3(-200.0f, 30.0f, 7000.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enSnake, Vector3(0.0f, 30.0f, 7000.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enSnake, Vector3(200.0f, 30.0f, 7000.0f), Quaternion::Identity);
		
		m_isArea01Spawned = true;
	}
	
	if (m_player->GetPlayerPos().z >= 9000.0f && !m_isArea02Spawned) {
		EnemyArrangement(EnEnemyType::enWildBoar, Vector3(800.0f, 30.0f, 10500.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enWildBoar, Vector3(-800.0f, 30.0f, 10500.0f), Quaternion::Identity);
		EnemyArrangement(EnEnemyType::enBear, Vector3(0.0f, 30.0f, 11000.0f), Quaternion::Identity);

		m_isArea02Spawned = true;
	}
}

/// <summary>
/// 指定した種類の敵を、指定した位置と回転で配置
/// </summary>
/// <param name="type">配置する敵の種類</param>
/// <param name="pos">敵を配置する位置</param>
/// <param name="rot">敵の回転</param>
void EnemyManager::EnemyArrangement(EnEnemyType type, Vector3 pos, Quaternion rot)
{
	SnakeEnemy* arrangeSnake = nullptr;
	WildBoar* arrangeWildBoar = nullptr;

	//敵の種類を判定し、出番が来ていない個体を探す
	switch (type) {

		//配置する敵がヘビのとき
	case EnEnemyType::enSnake:

		for (auto snakesIt = m_snakes.begin(); snakesIt != m_snakes.end(); ++snakesIt) {
			//スポーンしていない敵を見つけたら
			if ((*snakesIt)->GetIsSnakeSpawn() == false) {
				//配置するエネミーに設定
				arrangeSnake = *snakesIt;
				//for文を抜ける
				break;
			}
		}

		//エネミーを配置
		arrangeSnake->SetSnakePos(pos);
		arrangeSnake->SetSnakeCharaConPos(pos);
		arrangeSnake->SetSnakeRot(rot);
		arrangeSnake->SetSnakeIsSpawn(true);

		break;

		//配置する敵がイノシシのとき
	case EnEnemyType::enWildBoar:

		for (auto wildBoarsIt = m_wildBoars.begin(); wildBoarsIt != m_wildBoars.end(); ++wildBoarsIt) {
			if ((*wildBoarsIt)->GetIsWildBoarIsSpawn() == false) {
				arrangeWildBoar = *wildBoarsIt;
				break;
			}
		}

		arrangeWildBoar->SetWildBoarPos(pos);
		arrangeWildBoar->SetWildBoarCharaConPos(pos);
		arrangeWildBoar->SetWildBoarRot(rot);
		arrangeWildBoar->SetWildBoarIsSpawn(true);

		break;

		//配置する敵がクマのとき
	case EnEnemyType::enBear:

		m_bear->SetBearPos(pos);
		m_bear->SetBearCharaConPos(pos);
		m_bear->SetBearNewPos(pos);
		m_bear->SetBearRot(rot);
		m_bear->SetBearIsSpawn(true);

		break;

	default:
		break;
	}
}

/// <summary>
/// 新しいエネミーを追加
/// </summary>
/// <param name="type">作成する敵の種類/param>
//auto EnemyManager::CreateNewEnemy(EnEnemyType type)
//{
//	switch (type) {
//
//		//新しいヘビを追加
//	case EnEnemyType::enSnake:
//
//		SnakeEnemy* snake = NewGO<SnakeEnemy>(m_snakeNumber, "snakeEnemy");
//		snake->SetSnakePos(OFF_SCREEN_POS);
//		m_snakes.push_back(snake);
//		m_snakeNumber++;
//
//		//追加したエネミーを配置対象にする
//		return snake;
//
//		break;
//
//		//新しいイノシシを追加
//	case EnEnemyType::enWildBoar:
//
//		auto* wildBoar = NewGO<WildBoar>(m_wildBoarNumber, "wildBoar");
//		wildBoar->SetWildBoarPos(OFF_SCREEN_POS);
//		m_wildBoars.push_back(wildBoar);
//		m_wildBoarNumber++;
//
//		return wildBoar;
//
//		break;
//
//	default:
//		return nullptr;
//		break;
//	}
//}