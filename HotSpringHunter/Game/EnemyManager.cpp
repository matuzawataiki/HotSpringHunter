#include "stdafx.h"
#include "EnemyManager.h"
#include "EnemySpawner.h"
#include "SnakeEnemy.h"
#include "WildBoar.h"
#include "Bear.h"
#include "Player.h"
#include "Enemy/PoisonSnake/PoisonSnake.h"

namespace {
	const int ENEMY_NUM				= 15;				//エネミーの数
	const float PI					= 3.1415;			//円周率
	const float TRACK_TARGET_RADIUS = 200.0f;			//追従の目標位置のプレイヤーからの半径
	const Vector3 OFF_SCREEN_POS	= { 0.0f,3000.0f,500.0f };		//出番が来てないときの敵を格納する位置
}

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
	for (auto* snake : m_snakes) {
		DeleteGO(snake);
	}
	m_snakes.clear();

	for (auto* boar : m_wildBoars) {
		DeleteGO(boar);
	}
	m_wildBoars.clear();

	for (auto* poisonSnake : m_poisonSnake) {
		DeleteGO(poisonSnake);
	}
	m_poisonSnake.clear();

	DeleteGO(m_enemySpawner);
}

bool EnemyManager::Start()
{  
	//スポナーを生成
	m_enemySpawner = NewGO<EnemySpawner>(0, "enemySpawner");

	m_player = FindGO<Character::Player>("player");

	return true;
}

void EnemyManager::Update()
{
	//敵の囲い込み
	EnemyFormation();
}

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
		//見つけた敵へのベクトルを計算
		toEnemyVec = (*snakesIt)->GetSnakePos() - m_player->GetPlayerPos();
		//一番近いなら更新
		if (toEnemyVec.Length() < toNearestEnemyVec.Length()) {
			toNearestEnemyVec = toEnemyVec;
			nearestEnemyPos = (*snakesIt)->GetSnakePos();
		}
	}

	//毒ヘビ
	for (auto poisonSnakeIt = m_poisonSnake.begin(); poisonSnakeIt != m_poisonSnake.end(); ++poisonSnakeIt) {
		//見つけた敵へのベクトルを計算
		toEnemyVec = (*poisonSnakeIt)->GetPosition() - m_player->GetPlayerPos();
		//一番近いなら更新
		if (toEnemyVec.Length() < toNearestEnemyVec.Length()) {
			toNearestEnemyVec = toEnemyVec;
			nearestEnemyPos = (*poisonSnakeIt)->GetPosition();
		}
	}

	//イノシシ
	for (auto wildBoarIt = m_wildBoars.begin(); wildBoarIt != m_wildBoars.end(); ++wildBoarIt) {
		toEnemyVec = (*wildBoarIt)->GetWildBoarPos() - m_player->GetPlayerPos();
		if (toEnemyVec.Length() < toNearestEnemyVec.Length()) {
			toNearestEnemyVec = toEnemyVec;
			nearestEnemyPos = (*wildBoarIt)->GetWildBoarPos();
		}
	}

	//クマ
	if (m_bear != nullptr) {
		toEnemyVec = m_bear->GetBearPos() - m_player->GetPlayerPos();
		if (toEnemyVec.Length() < toNearestEnemyVec.Length()) {
			toNearestEnemyVec = toEnemyVec;
			nearestEnemyPos = m_bear->GetBearPos();
		}
	}

	nearestEnemyPos.y = 0.0f;

	return nearestEnemyPos;
}

/// <summary>
/// エネミーが囲い込むような挙動をするように、移動速度を調整
/// </summary>
void EnemyManager::EnemyFormation()
{
	std::vector<SnakeEnemy*>trackingSnakes;			//追従しているヘビ
	std::vector<WildBoar*>trackingWildBoars;		//追従しているイノシシ
	std::vector<Vector3>trackTargetPos;				//追従の目標位置
	Vector3 speed = Vector3::Zero;					//追従の移動ベクトル


	//ヘビ
	for (auto snakesIt = m_snakes.begin(); snakesIt != m_snakes.end(); ++snakesIt) {
		//追従状態のヘビを見つける
		if ((*snakesIt)->GetSnakeState() == EnSnakeState::enSnakeTrack) {
			//見つけたら配列に入れる
			trackingSnakes.push_back((*snakesIt));
		}
	}

	//イノシシ
	for (auto wildBoarIt = m_wildBoars.begin(); wildBoarIt != m_wildBoars.end(); ++wildBoarIt) {
		if ((*wildBoarIt)->GetWildBoarState() == EnWildBoarState::enWildBoarTrack) {
			trackingWildBoars.push_back((*wildBoarIt));
		}
	}

	//追従しているエネミーの総数を数える
	int trackingEnemies = trackingSnakes.size() + trackingWildBoars.size();

	//追従の目標位置を計算
	for (int i = 0; i < trackingEnemies; i++) {
		float angle = 2.0f * PI * i / trackingEnemies;
		float x = m_player->GetPlayerPos().x + TRACK_TARGET_RADIUS * std::cos(angle);
		float z = m_player->GetPlayerPos().z + TRACK_TARGET_RADIUS * std::sin(angle);
		trackTargetPos.emplace_back(Vector3{ x,0.0f,z });
	}

	//計算した位置目標位置に各エネミーを動かす

	for (auto trackSnakesIt = trackingSnakes.begin(); trackSnakesIt != trackingSnakes.end(); ++trackSnakesIt) {
		speed = trackTargetPos.front() - (*trackSnakesIt)->GetSnakePos();
		speed.y = 0.0f;
		speed.Normalize();
		speed *= (*trackSnakesIt)->GetSnakeSpeed();
		(*trackSnakesIt)->SetSnakeSpeed(speed);
		trackTargetPos.erase(trackTargetPos.begin());
	}

	for (auto trackWildBoarIt = trackingWildBoars.begin(); trackWildBoarIt != trackingWildBoars.end(); ++trackWildBoarIt) {
		speed = trackTargetPos.front() - (*trackWildBoarIt)->GetWildBoarPos();
		speed.y = 0.0f;
		speed.Normalize();
		speed *= (*trackWildBoarIt)->GetWildBoarSpeed();
		(*trackWildBoarIt)->SetWildBoarSpeed(speed);
		trackTargetPos.erase(trackTargetPos.begin());
	}
}

void EnemyManager::ForEachEnemy(const ForEachEnemyFunc& func)
{
	for (auto& enemy : m_snakes) {
		if (enemy->CheckDeath())
		{
			continue;
		}
		func(enemy, enemy->GetSnakePos());
	}
	for (auto& enemy : m_poisonSnake) {
		if (enemy->CheckDeath())
		{
			continue;
		}
		func(enemy, enemy->GetPosition());
	}
	for (auto& enemy : m_wildBoars) {
		if (enemy->CheckDeath())
		{
			continue;
		}
		func(enemy, enemy->GetWildBoarPos());
	}	
}

void EnemyManager::DeleteBoss()
{
	DeleteGO(m_bear);
	m_bear = nullptr;
}

bool EnemyManager::IsEnemy()
{
	if (m_snakes.size() != 0) {
		return true;
	}
	if (m_poisonSnake.size() != 0) {
		return true;
	}
	if (m_wildBoars.size() != 0) {
		return true;
	}
	return false;
}
