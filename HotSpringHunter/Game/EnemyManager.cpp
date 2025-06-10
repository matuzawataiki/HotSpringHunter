#include "stdafx.h"
#include "EnemyManager.h"
#include "EnemySpawner.h"
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
	//スポナーを生成
	m_enemySpawner = NewGO<EnemySpawner>(0, "enemySpawner");

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

		auto* wildBoar = NewGO<WildBoar>(0, "wildBoar");
		wildBoar->SetWildBoarPos(OFF_SCREEN_POS);
		m_wildBoars.push_back(wildBoar);
	}

	//クマは1体だけ
	m_bear = NewGO<Bear>(0, "bear");
	m_bear->SetBearPos(OFF_SCREEN_POS);
}

void EnemyManager::Update()
{
}

/// <summary>
/// 指定した種類の敵を、指定した位置と回転で配置
/// </summary>
/// <param name="type">配置する敵の種類</param>
/// <param name="pos">敵を配置する位置</param>
/// <param name="rot">敵の回転</param>
void EnemyManager::EnemyArrangement(const EnEnemyType type, const Vector3& pos, const Quaternion& rot)
{
	SnakeEnemy* arrangeSnake	= nullptr;
	WildBoar*	arrangeWildBoar = nullptr;

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

		//arrangeSnake = CreateNewEnemy(EnEnemyType::enSnake);
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
//		SnakeEnemy* snake = NewGO<SnakeEnemy>(0, "snakeEnemy");
//		snake->SetSnakePos(OFF_SCREEN_POS);
//		m_snakes.push_back(snake);
//
//		//追加したエネミーを配置対象にする
//		return snake;
//
//		break;
//
//		//新しいイノシシを追加
//	case EnEnemyType::enWildBoar:
//
//		auto* wildBoar = NewGO<WildBoar>(0, "wildBoar");
//		wildBoar->SetWildBoarPos(OFF_SCREEN_POS);
//		m_wildBoars.push_back(wildBoar);
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

/// <summary>
/// プレイヤーの位置から最も近い敵の位置を返す
/// </summary>
/// <param name="playerPos">プレイヤーの現在位置を表すベクトル</param>
/// <returns>最も近い敵の位置</returns>
Vector3 EnemyManager::CalcToNearestEnemyVec(const Vector3& playerPos)
{
	Vector3 toNearestEnemyVec = Vector3{ 10000.0f,0.0f,10000.0f };			//一番近い敵に向かって伸びるベクトル
	Vector3 toEnemyVec			= Vector3::Zero;			//敵に向かって伸びるベクトル（暫定）
	Vector3 nearestEnemyPos		= Vector3::Zero;			//一番近い敵の位置

	//ヘビ
	for (auto snakesIt = m_snakes.begin(); snakesIt != m_snakes.end(); ++snakesIt) {
		//スポーンしている敵を見つける
		if ((*snakesIt)->GetIsSnakeSpawn()) {
			//見つけた敵へのベクトルを計算
			toEnemyVec = (*snakesIt)->GetSnakePos() - m_player->GetPlayerPos();
			//一番近いなら更新
			if (toEnemyVec.Length() < toNearestEnemyVec.Length()) {
				toNearestEnemyVec = toEnemyVec;
				nearestEnemyPos = (*snakesIt)->GetSnakePos();
			}
		}
	}

	//イノシシ
	for (auto wildBoarIt = m_wildBoars.begin(); wildBoarIt != m_wildBoars.end(); ++wildBoarIt) {
		if ((*wildBoarIt)->GetIsWildBoarIsSpawn()) {
			toEnemyVec = (*wildBoarIt)->GetWildBoarPos() - m_player->GetPlayerPos();
			if (toEnemyVec.Length() < toNearestEnemyVec.Length()) {
				toNearestEnemyVec = toEnemyVec;
				nearestEnemyPos = (*wildBoarIt)->GetWildBoarPos();
			}
		}
	}

	//クマ
	if (m_bear->GetIsBearSpawn()) {
		toEnemyVec = m_bear->GetBearPos() - m_player->GetPlayerPos();
		if (toEnemyVec.Length() < toNearestEnemyVec.Length()) {
			toNearestEnemyVec = toEnemyVec;
			nearestEnemyPos = m_bear->GetBearPos();
		}
	}

	nearestEnemyPos.y = 0.0f;

	return nearestEnemyPos;
}