#include "stdafx.h"
#include "SnakeEnemy.h"
#include "EnemySpawn.h"
#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "collision/CollisionObject.h"

namespace {
	const float FIND_RANGE = 500.0f;			//プレイヤーを捉える距離
	const float ATK_RANGE = 200.0f;				//近接攻撃のリーチ
	const float MELEE_ATTACK_DAMAGE = 20.0f;	//近接攻撃のダメージ
	const float ATK_COOLTIME = 3.0f;			//近接攻撃のクールタイム
};

SnakeEnemy::SnakeEnemy()
{
	
}

SnakeEnemy::~SnakeEnemy()
{

}

bool SnakeEnemy::Start()
{
	//アセット読み込み
	LoadAsset();

	//基底クラス生成
	m_enemyBase = NewGO<EnemyBase>(0, "enemyBase");

	//インスタンス探し
	m_player = FindGO<Character::Player>("player");
	m_enemySpawn = FindGO<EnemySpawn>("enemySpawn");
	m_enemyBase = NewGO<EnemyBase>(0,"enemyBase");

	//キャラクターコントローラー
	m_characterController.Init(30.0f, 50.0f, m_position);
	
	return true;
}

/// <summary>
/// アセットを読み込む
/// </summary>
void SnakeEnemy::LoadAsset()
{
	//アニメーション読み込み
	m_animationClips[enSnakeAnimClip_Idle].Load("Assets/animData/snake/idle.tka");
	m_animationClips[enSnakeAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enSnakeAnimClip_Walk].Load("Assets/animData/snake/walk.tka");
	m_animationClips[enSnakeAnimClip_Walk].SetLoopFlag(true);
	m_animationClips[enSnakeAnimClip_Attack].Load("Assets/animData/snake/attack.tka");
	m_animationClips[enSnakeAnimClip_Attack].SetLoopFlag(false);
	m_animationClips[enSnakeAnimClip_Hit].Load("Assets/animData/snake/hit.tka");
	m_animationClips[enSnakeAnimClip_Hit].SetLoopFlag(true);
	m_animationClips[enSnakeAnimClip_Death].Load("Assets/animData/snake/death.tka");
	m_animationClips[enSnakeAnimClip_Death].SetLoopFlag(true);

	//モデル読み込み
	m_modelRender.Init("Assets/modelData/snake/snake.tkm"/*, m_animationClips, enSnakeAnimClip_Num, enModelUpAxisY*/);
}

void SnakeEnemy::Update()
{
	//スポーンしているなら
	if (m_isSpawn) {
		//ステート管理
		ManageState();
		//行動を実行
		ExecuteAction();
	}

	//いろいろ更新
	VariousUpdate();
}


/// <summary>
/// ステート管理
/// </summary>
void SnakeEnemy::ManageState()
{
	//ステートを変えてもよいなら
	m_isCanStateChange = m_enemyBase->ChangeFlag();
	if (!m_isCanStateChange) {
		return;
	}
	//被弾した場合
	if (m_player->m_collision->IsHit(m_characterController)) {

		//HPを減らす
		m_enemyHP -= m_player->m_attackPower;

		//HPがまだ残っている
		if (m_enemyHP > 0.0f) {
			//ノックバック
			m_snakeState = enSnakeKnockBack;
		}
		//HPがなくなった
		else {
			//死亡（吹っ飛び）
			m_snakeState = enSnakeDeath;
		}
		return;
	}

	//近接攻撃
	//攻撃範囲まで近づいたら
	if (m_toPlayer.Length() < ATK_RANGE) {
		m_snakeState = enSnakeAttack;
		return;
	}

	//追従
	//攻撃範囲外へ出ていて、かつプレイヤーを捉えていたら
	if ((m_toPlayer.Length() > ATK_RANGE) && (FindPlayer())) {
		m_snakeState = enSnakeTrack;
		return;
	}

	//待機
	m_snakeState = enSnakeIdle;
}

/// <summary>
/// プレイヤーを探す
/// </summary>
/// <returns></returns>
bool SnakeEnemy::FindPlayer()
{
	if (m_toPlayer.Length() < FIND_RANGE) {
		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// 行動を実行。
/// </summary>
void SnakeEnemy::ExecuteAction()
{
	//移動速度を0する
	m_moveSpeed = Vector3::Zero;
	//近接攻撃のクールタイムを計算
	m_ATKCoolTime -= g_gameTime->GetFrameDeltaTime();

	switch (m_snakeState) {

		//ノックバック
	case enSnakeKnockBack:
		//ノックバックをさせる
		m_moveSpeed = m_enemyBase->KnockBack(m_enemyDir);
		//被弾アニメーションを再生
		//m_modelRender.PlayAnimation(enSnakeAnimClip_Hit);
		break;

		//死亡
	case enSnakeDeath:
		//吹っ飛ばせる
		m_moveSpeed = m_enemyBase->DeathBlown(m_enemyDir);
		//死亡アニメーション（ふっとび）を再生
		//m_modelRender.PlayAnimation(enSnakeAnimClip_Death);
		break;

		//近接攻撃
	case enSnakeAttack:
		//クールタイムが終わっていたら
		if (m_ATKCoolTime <= 0.0f) {
			m_enemyBase->MeleeAttack(m_position, m_enemyDir, MELEE_ATTACK_DAMAGE);
			//近接攻撃アニメーションを再生
			//m_modelRender.PlayAnimation(enSnakeAnimClip_Attack);
			//タイマーをセット
			m_ATKCoolTime = ATK_COOLTIME;
		}		
		break;

		//追従
	case enSnakeTrack:
		m_moveSpeed = m_enemyBase->Tracking(m_toPlayer);
		//歩きアニメーションを再生
		//m_modelRender.PlayAnimation(enSnakeAnimClip_Walk);
		break;

		//待機
	case enSnakeIdle:
		//待機アニメーションを再生
		//m_modelRender.PlayAnimation(enSnakeAnimClip_Idle);
		break;
	default:
		break;
	}
}

/// <summary>
/// いろいろ更新（座標、回転など）
/// </summary>
void SnakeEnemy::VariousUpdate()
{
	//プレイヤーへのベクトルを更新
	m_toPlayer = m_player->GetPlayerPos() - m_position;

	//プレイヤーを捉えているときだけ向きの更新をする
	if (FindPlayer()) {
		//向きの更新。
		m_enemyDir = m_toPlayer;
		m_enemyDir.Normalize();
	}

	//速度を適応。
	ExecuteSpeed();

	//回転の更新。
	m_rotation.SetRotationYFromDirectionXZ(m_enemyDir);
	m_modelRender.SetRotation(m_rotation);

	//座標の更新。
	m_modelRender.SetPosition(m_position);

	//モデルの更新。
	m_modelRender.Update();
}

/// <summary>
/// 速度を適応。
/// </summary>
void SnakeEnemy::ExecuteSpeed()
{
	//敵が吹っ飛ぶときにキャラコンを消して透明な壁をすり抜ける
	if (m_isAlive) {
		// キャラクターコントローラーがある時は移動処理の結果を受け取るだけ
		m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else {
		// ないときは自分で移動結果を計算
		const Vector3 move = m_moveSpeed * 1.0f / 60.0f;
		m_position.Add(move);
	}
}

void SnakeEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}