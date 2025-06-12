#include "stdafx.h"
#include "SnakeEnemy.h"
#include "EnemySpawn.h"
#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "collision/CollisionObject.h"
#include "SoundEffect.h"

namespace {
	const float DELTA_TIME				= 1.0f / 60.0f;		//フレームレート
	const float ANIM_INTERPOLATE_TIME	= 0.2f;				//アニメーションの補間時間
	const float MAX_SNAKE_HP			= 100.0f;			//ヘビの最大HP
	const float FIND_RANGE				= 9000.0f;			//プレイヤーを捉える距離
	const float SNAKE_SPEED				= 300.0f;			//ヘビの移動速度

	const float ATK_RANGE				= 300.0f;			//近接攻撃のリーチ
	const float MELEE_ATTACK_DAMAGE		= 20.0f;			//近接攻撃のダメージ
	const float ATK_COOLTIME			= 3.0f;				//近接攻撃のクールタイム

	const float TO_NOT_SPAWNED_TIME		= 5.0f;				//死亡してから非スポーン状態とする時間
};

SnakeEnemy::SnakeEnemy()
{

}

SnakeEnemy::~SnakeEnemy()
{
	//m_snakeController.RemoveRigidBoby();
}

bool SnakeEnemy::Start()
{
	//アセット読み込み
	LoadAsset();

	//サウンドエフェクト
	m_soundEffect = FindGO<SoundEffect>("soundEffect");

	//基底クラス生成
	m_enemyBase = NewGO<EnemyBase>(0, "enemyBase");

	//インスタンス探し
	m_player	 = FindGO<Character::Player>("player");

	//キャラクターコントローラー
	//m_snakeController.Init(30.0f, 50.0f, m_snakePos);

	//HPをセット
	m_snakeHP = MAX_SNAKE_HP;

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
	m_animationClips[enSnakeAnimClip_Hit].SetLoopFlag(false);
	m_animationClips[enSnakeAnimClip_Death].Load("Assets/animData/snake/death.tka");
	m_animationClips[enSnakeAnimClip_Death].SetLoopFlag(true);

	//モデル読み込み
	m_snakeModel.Init("Assets/modelData/snake/snake.tkm", m_animationClips, enSnakeAnimClip_Num, enModelUpAxisY);
}

void SnakeEnemy::Update()
{
	//スポーンしているなら
	if (!m_isSpawn) {
		return;
	}

	//ステート管理
	ManageState();
	//行動を実行
	ExecuteAction();
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
	if (m_player->m_collision->IsHit(m_snakeController)) {

		//HPを減らす
		m_snakeHP -= m_player->m_attackPower;
		//HPがまだ残っている
		if (m_snakeHP > 0.0f) {
			//ノックバック
			m_snakeState = enSnakeKnockBack;
			//被弾の効果音
			m_soundEffect->Play(enSnakeHitSE, false);
		}
		//HPがなくなった
		else {
			//死亡（吹っ飛び）
			m_snakeState = enSnakeDeath;
			//死亡の効果音
			m_soundEffect->Play(enSnakeDeathSE, false);
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
/// <returns>見つけられる範囲内なら true 、範囲外なら false　</returns>
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
	//m_snakeSpeed = Vector3::Zero;
	//近接攻撃のクールタイムを計算
	m_ATKCoolTime -= g_gameTime->GetFrameDeltaTime();

	switch (m_snakeState) {

		//ノックバック
	case enSnakeKnockBack:
		//ノックバックをさせる
		m_snakeSpeed = m_enemyBase->KnockBack(m_snakeDir);
		//被弾アニメーションを再生
		m_snakeModel.PlayAnimation(enSnakeAnimClip_Hit, ANIM_INTERPOLATE_TIME);
		break;

		//死亡
	case enSnakeDeath:
		//死亡経過時間を計算
		m_elapsedTime += g_gameTime->GetFrameDeltaTime();
		//吹っ飛ばす
		m_snakeSpeed = m_enemyBase->DeathBlown(m_snakeDir);
		//死亡アニメーション（ふっとび）を再生
		m_snakeModel.PlayAnimation(enSnakeAnimClip_Death, ANIM_INTERPOLATE_TIME);
		//キャラコンを消す
		if (!m_isRemoveController) {
			m_snakeController.RemoveRigidBoby();
			m_isRemoveController = true;
		}
		//一定時間がたったら
		if (m_elapsedTime >= TO_NOT_SPAWNED_TIME) {
			//時間をリセット
			m_elapsedTime = 0.0f;
			//非スポーン状態にする
			m_isSpawn = false;
		}

		break;

		//近接攻撃
	case enSnakeAttack:
		//クールタイムが終わっていたら
		if (m_ATKCoolTime <= 0.0f) {
			m_enemyBase->MeleeAttack(m_snakePos, m_snakeDir, MELEE_ATTACK_DAMAGE);
			//近接攻撃アニメーションを再生
			m_snakeModel.PlayAnimation(enSnakeAnimClip_Attack, ANIM_INTERPOLATE_TIME);
      //近接攻撃の効果音
      m_soundEffect->Play(enSnakeAttackSE, false);
			//タイマーをセット
			m_ATKCoolTime = ATK_COOLTIME;
		}
		else if (!m_snakeModel.IsPlayAnimation()) {
			//待機アニメーションを再生
			m_snakeModel.PlayAnimation(enSnakeAnimClip_Idle, ANIM_INTERPOLATE_TIME);
		}
		break;

		//追従
	case enSnakeTrack:
		//m_snakeSpeed = m_enemyBase->Tracking(m_toPlayer);
		//歩きアニメーションを再生
		m_snakeModel.PlayAnimation(enSnakeAnimClip_Walk, ANIM_INTERPOLATE_TIME);
		break;

		//待機
	case enSnakeIdle:
		//待機アニメーションを再生
		m_snakeModel.PlayAnimation(enSnakeAnimClip_Idle, ANIM_INTERPOLATE_TIME);
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
	m_toPlayer = m_player->GetPlayerPos() - m_snakePos;

	//プレイヤーを捉えているときだけ向きの更新をする
	if (FindPlayer()) {
		//向きの更新。
		m_snakeDir = m_toPlayer;
		m_snakeDir.Normalize();
	}

	//速度を適応。
	ExecuteSpeed();

	//回転の更新。
	m_snakeRot.SetRotationYFromDirectionXZ(m_snakeDir);
	m_snakeModel.SetRotation(m_snakeRot);

	//座標の更新。
	m_snakeModel.SetPosition(m_snakePos);

	//モデルの更新。
	m_snakeModel.Update();
}

/// <summary>
/// 速度を適応。
/// </summary>
void SnakeEnemy::ExecuteSpeed()
{
	//まだキャラコンが削除されていないとき
	if (!m_isRemoveController) {
		m_snakePos = m_snakeController.Execute(m_snakeSpeed, DELTA_TIME);
	}
	//キャラコンが削除されているとき
	else {
		const Vector3 moveAmount = m_snakeSpeed * DELTA_TIME;
		m_snakePos.Add(moveAmount);
	}
}


/// <summary>
/// スポーンした時の処理。
/// </summary>
void SnakeEnemy::Spawned()
{
	//キャラクターコントローラー
	m_snakeController.Init(30.0f, 50.0f, m_snakePos);
	//HPをセット
	m_snakeHP = MAX_SNAKE_HP;
	//スポーン状態に
	m_isSpawn = true;
	//待機状態に
	m_snakeState = EnSnakeState::enSnakeIdle;
	//ステート変更可能に
	m_isCanStateChange = true;
}

void SnakeEnemy::Render(RenderContext& rc)
{
	//スポーンしていないなら実行しない
	if (!m_isSpawn) {
		return;
	}

	//死亡時は点滅表示を行う
	if ((m_snakeState == enSnakeDeath) && (m_enemyBase->IsBlinkRender())) {
		m_snakeModel.Draw(rc);
	}
	///死亡時以外は通常表示
	if (m_snakeState != enSnakeDeath) {
		m_snakeModel.Draw(rc);
	}
}

/// <summary>
/// ヘビの移動速度を取得
/// </summary>
/// <returns>ヘビの移動速度</returns>
float SnakeEnemy::GetSnakeSpeed() const
{
	return SNAKE_SPEED;
}
