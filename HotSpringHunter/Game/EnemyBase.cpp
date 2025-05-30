#include "stdafx.h"
#include "EnemyBase.h"
#include "EnemySpawn.h"
#include "Game.h"
#include "Player.h"
#include "collision/CollisionObject.h"

namespace
{
	const float MOVE_AMOUNT				= 100.0f;		//移動：移動量

	const float ATK_COLLISION_POS		= 140.0f;		//近接攻撃：コリジョンの位置
	const float ATK_COLLISION_RADIUS	= 150.0f;		//近接攻撃：コリジョンの半径

	const float KNOCK_BACK_AMOUNT		= -500.0f;		//ノックバック：初速
	const float KNOCK_BACK_DECREASE		= 0.02f;		//ノックバック：減速量
	const float KNOCK_BACK_STOP			= 0.1f;			//ノックバック：状態を解除する減少率

	const float BLOWN_HORIZONTAL		= -1500.0f;		//吹っ飛び：水平方向
	const float BLOWN_HEIGHT			= 300.0f;		//吹っ飛び：垂直方向

	const float BLINK_TIME				= 0.2f;			//点滅表示：切り替える時間
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
	m_player = FindGO<Player>("player");

	return true;
}

/// <summary>
/// プレイヤーへの追従
/// </summary>
/// <param name="position">エネミーからプレイヤーに向かって伸びるベクトル</param>
Vector3 EnemyBase::Tracking(Vector3 toPlayer)
{
	Vector3 toPlayerDir = toPlayer;
	toPlayerDir.Normalize();

	//正規化で求めたベクトルを利用して、エネミーの座標を動かす
	Vector3 moveSpeed = toPlayerDir * MOVE_AMOUNT;

	return moveSpeed;
}

/// <summary>
/// 近接攻撃
/// </summary>
/// <param name="position">コリジョンを出す位置</param>
/// <param name="enemyDir">エネミーの向き</param>
/// <param name="damage">プレイヤーに与えるダメージ量</param>
void EnemyBase::MeleeAttack(Vector3 position, Vector3 enemyDir, float damage)
{
	//前方にコリジョンを作る
	EnemyAttackCollision(position, enemyDir);
	//プレイヤーにヒットしたら
	if (m_enemyATCollision->IsHit(m_player->m_playerCharaCon)) {
		//プレイヤーにダメージを与える
		m_player->Hit(damage);
	}
	//コリジョン削除
	DeleteGO(m_enemyATCollision);
}

/// <summary>
/// 攻撃時に発生するコリジョンの生成
/// </summary>
/// <param name="position">コリジョンの位置</param>
/// <param name="enemyDir">エネミーの向き</param>
void EnemyBase::EnemyAttackCollision(Vector3 position, Vector3 enemyDir)
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
/// ノックバック
/// </summary>
/// <param name="enemyDir">エネミーの向き</param>
/// <returns>ノックバックのベクトル</returns>
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

	//ノックバックの方向はエネミーの向きの逆方向
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
/// 死亡時の吹っ飛び
/// </summary>
/// <param name="enemyDir">エネミーの向き</param>
/// <returns>吹っ飛ぶ方向のベクトル</returns>
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
/// 死亡（吹っ飛ばない）
/// </summary>
void EnemyBase::Death()
{
	//ステート変更をできなくする
	m_isCanChange = false;
}


/// <summary>
/// 死亡時のの点滅表示
/// </summary>
/// <returns>描画するならtrue 、描画しないなら false </returns>
bool EnemyBase::IsBlinkRender()
{
	m_renderTime += g_gameTime->GetFrameDeltaTime();

	//一周したらリセット
	if (m_renderTime >= BLINK_TIME * 2.0f) {
		m_renderTime = 0.0f;
	}

	//点滅時間の前半は描画
	if (m_renderTime <= BLINK_TIME) {
		//描画させる
		return true;
	}
	//点滅時間の後半は描画しない
	else if (m_renderTime <= BLINK_TIME * 2.0f) {
		//描画しない
		return false;
	}
}

/// <summary>
/// ステートを変更できるかを返す
/// </summary>
/// <returns>フラグを変更できる場合は true 、できない場合は false を返す</returns>
bool EnemyBase::ChangeFlag()
{
	if (!m_isCanChange) {
		return false;
	}
	return true;
}