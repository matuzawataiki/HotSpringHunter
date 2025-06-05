#include "stdafx.h"
#include "EnemyBase.h"
#include "EnemySpawn.h"
#include "Game.h"
#include "Player.h"
#include "collision/CollisionObject.h"

namespace
{
	const float MOVE_AMOUNT = 100.0f;
	const float ATK_COLLISION_POS = 140.0f;
	const float ATK_COLLISION_RADIUS = 150.0f;
	const float KNOCK_BACK_AMOUNT = -500.0f;			//ノックバックの初速
	const float KNOCK_BACK_DECREASE = 0.02f;			//ノックバック減少率
	const float KNOCK_BACK_STOP = 0.1f;					//ノックバック状態を解除する減少率
	const float BLOWN_HORIZONTAL = -1500.0f;			//やられた時の吹っ飛ぶ量（水平）
	const float BLOWN_HEIGHT = 50.0f;					//やられた時の吹っ飛ぶ量（高さ）
}

EnemyBase::EnemyBase()
{
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Start()
{
	//プレイヤー
	m_player = FindGO<Character::Player>("player");

	return true;
}

/// <summary>
/// プレイヤーへの追従
/// </summary>
/// <param name="position"></param>
Vector3 EnemyBase::Tracking(Vector3 toPlayer)
{
	Vector3 moveSpeed = Vector3::Zero;

	//プレイヤーとの距離が400以下だったら追いかける
		//プレイヤーに向かっている伸びるベクトルを正規化。
	Vector3 toPlayerDir = toPlayer;
	toPlayerDir.Normalize();

	//正規化で求めたベクトルを利用して、エネミーの座標を動かす
	moveSpeed = toPlayerDir * MOVE_AMOUNT;

	return moveSpeed;
}

/// <summary>
///プレイヤーへの攻撃 
/// </summary>
void EnemyBase::MeleeAttack(Vector3 position, Vector3 enemyDir, float damage)
{
	//プレイヤーとの距離が200以下の場合
	//前方にコリジョンを作る
	EnemyAttackCollision(position, enemyDir);
	//プレイヤーにヒットしたら
	if (m_enemyATCollision->IsHit(m_player->m_playerCharaCon)) {
		m_player->Hit(damage);
	}
	//コリジョン削除
	DeleteGO(m_enemyATCollision);
}

/// <summary>
/// 攻撃時に発生するコリジョンの生成
/// </summary>
void EnemyBase::EnemyAttackCollision(Vector3 position,Vector3 enemyDir)
{
	//コリジョンオブジェクトを作成
	m_enemyATCollision = NewGO<CollisionObject>(0, "enemy_atk");
	Vector3 collisionPosition = Vector3::Zero;

	//エネミーの少し前に設定
	enemyDir *= ATK_COLLISION_POS;
	collisionPosition = position + enemyDir;

	//球状のコリジョンを作成
	m_enemyATCollision->CreateSphere(
		collisionPosition,
		Quaternion::Identity,
		ATK_COLLISION_RADIUS
	);
}

/// <summary>
/// ノックバック。
/// </summary>
/// <param name="enemyDir"></param>enemyの向き
/// <returns></returns>ノックバックのベクトル
Vector3 EnemyBase::KnockBack(Vector3 enemyDir)
{
	Vector3 knockBackVec = Vector3::Zero;		//ノックバックのベクトル

	//最初にいろいろセット
	if (!m_isKnockSetFlag) {
		//減速率をセット
		m_knockDecreased = 1.0f;
		//ステート変更を不可にする
		m_isCanChange = false;
		//セット済みにする
		m_isKnockSetFlag = true;
	}

	//ノックバックの方向はenemyの向きの逆方向
	knockBackVec = enemyDir;
	knockBackVec *= KNOCK_BACK_AMOUNT;

	//減速率を徐々に大きくする
	m_knockDecreased -= KNOCK_BACK_DECREASE;

	//速度を減速させる。
	knockBackVec *= m_knockDecreased;

	//ノックバックが終わったら
	if (m_knockDecreased <= KNOCK_BACK_STOP) {
		//ステート変更を可能に
		m_isCanChange = true;
		//フラッグと減速率をリセット
		m_isKnockSetFlag = false;
		m_knockDecreased = 1.0f;
	}

	return knockBackVec;	
}

/// <summary>
/// エネミーが死んだ時
/// </summary>
Vector3 EnemyBase::DeathBlown(Vector3 enemyDir)
{
	Vector3 blownVec = Vector3::Zero;		//吹っ飛びベクトル

	//吹っ飛びのベクトルはenemyの向きの逆方向
	blownVec = enemyDir;
	blownVec *= BLOWN_HORIZONTAL;
	//ベクトルに高さを持たせる
	blownVec.y = BLOWN_HEIGHT;

	//ステート変更をできなくする
	m_isCanChange = false;

	return blownVec;
}

/// <summary>
/// 
/// </summary>
void EnemyBase::Death()
{
	//ステート変更をできなくする
	m_isCanChange = false;
}

bool EnemyBase::ChangeFlag()
{
	if (!m_isCanChange) {
		return false;
	}
	return true;
}