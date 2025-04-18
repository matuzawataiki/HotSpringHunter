#include "stdafx.h"
#include "Bear.h"

#include "Game.h"
#include "Player.h"

#include "collision/CollisionObject.h"

namespace {
	const Vector3 BACK_YARD_POS = { 100.0f,300.0f,500.0f };
}

Bear::Bear()
{
	//プレイヤー
	m_player = FindGO<Player>("player");

	//アニメーションロード
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/bear/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/bear/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/bear/Attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Hit].Load("Assets/animData/bear/Hit.tka");
	m_animationClips[enAnimationClip_Hit].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/bear/Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(true);

	//エネミーモデル
	m_modelRender.Init("Assets/modelData/bear/bear.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_position.y = 100.0f;
	////初期値のランダム配置
	//キャラクターコントローラー
	m_characterController.Init(30.0f, 50.0f, m_position);
	//m_modelRender.Update();

	//physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

Bear::~Bear()
{

}

void Bear::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		int a = 0;
		a++;
	}
	if (m_isSpawn == true)
	{
		if (m_isAlive == true) {
			//プレイヤーの追従
			Tracking();
			//プレイヤーに攻撃
			SnakeAttack();
		}
		else {
			//エネミーの吹っ飛び
			EnemyDeath();
		}

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
	else
	{
		m_position = BACK_YARD_POS;
	}

	//アニメーションの再生
	EnemyAnimation();
	//ステート管理
	ManageState();
	//エネミーがプレイヤーに向く
	Rotation();

	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}

//エネミーがプレイヤーに向く
void Bear::Rotation()
{
	m_rotation.SetRotationYFromDirectionXZ(m_toPlayer);
	m_modelRender.SetRotation(m_rotation);
}

//プレイヤーと敵の当たり判定
void Bear::SnakeAttack()
{
	//プレイヤーとの距離が200以下の場合
	Vector3 diff = m_player->GetPlayerPos() - m_position;

	if (diff.Length() < 200.0f && m_enemyATK == false)
	{
		//前方にコリジョンを作る
		EnemyAttackCollision();
	}
	else if (diff.Length() >= 220.0f)
	{
		m_enemyATK = false;
	}

	if (m_enemyHP <= 0.0f)
	{
		// 死亡したらキャラクターコントローラーを消して壁貫通するようにする
		m_characterController.RemoveRigidBoby();
		m_isAlive = false;
	}
}

//範囲内のプレイヤーを追いかける
void Bear::Tracking()
{
	//エネミーからプレイヤーに向かって伸びるベクトルを計算
	m_toPlayer = m_player->GetPlayerPos() - m_position;
	m_toPlayer.y = 0.0f;

	//プレイヤーとの距離を計算する
	float distToPlayer = m_toPlayer.Length();

	if (distToPlayer < 200.0f) {
		m_moveSpeed.Set(Vector3::Zero);
		m_moveStop = true;
		MoveStop();
	}
	else if (distToPlayer < 1000.0f) {
		//プレイヤーとの距離が400以下だったら追いかける
		//プレイヤーに向かっている伸びるベクトルを正規化。
		Vector3 toPlayerDir = m_toPlayer;
		toPlayerDir.Normalize();

		//正規化で求めたベクトルを利用して、エネミーの座標を動かす
		m_moveSpeed = toPlayerDir * 100.0f;
	}
}

//一時停止
void Bear::MoveStop()
{
	if (m_moveStop == true)
	{
		m_moveSpeed = toPlayerDir * 0.0f;
		m_moveStop = false;

		return;
	}
}

void Bear::Hit(float takeDamage)
{
	m_enemyHP -= takeDamage;
}

//プレイヤーと逆方向に飛んでいく
void Bear::EnemyDeath()
{
	if (enemyDead)
	{
		//プレイヤーに向かっている伸びるベクトルを正規化
		Vector3 toPlayerDir = m_toPlayer;
		toPlayerDir.Normalize();
		toPlayerDir *= -1.0f;

		//正規化で求めたベクトルを利用して、エネミーの座標を動かす
		m_moveSpeed = toPlayerDir * 2000.0f;
		m_moveSpeed.y = 800.0f;

		//吹っ飛ぶアニメーション
		m_animationState = enDeath;
	}
	enemyDead = true;
}

//エネミーの攻撃コリジョン
void Bear::EnemyAttackCollision()
{
	//コリジョンオブジェクトを作成
	collisionObject = NewGO<CollisionObject>(0, "bear_atk");
	Vector3 collisionPosition = Vector3::Zero;

	//エネミーの少し前に設定
	m_enemyDirection = m_toPlayer;
	m_enemyDirection.Normalize();
	m_enemyDirection *= 100.0f;
	collisionPosition = m_position + m_enemyDirection;

	//球状のコリジョンを作成
	collisionObject->CreateSphere(
		collisionPosition,
		Quaternion::Identity,
		100.0f
	);

	//プレイヤーが敵にあたると-10
	if (collisionObject->IsHit(m_player->m_playerCharaCon)) {
		m_enemyHP -= 10;
		//ノックバック
		KnockBack();
	}

	DeleteGO(collisionObject);

	m_enemyATK = true;
}

//プレイヤーに攻撃されたときノックバックする
void Bear::KnockBack()
{
	//// 後退
	////プレイヤーに向かっている伸びるベクトルを正規化
	//Vector3 toPlayerDir = m_toPlayer;
	//toPlayerDir.Normalize();
	//toPlayerDir.y = 0;
	//toPlayerDir * -1.0f;

	////正規化で求めたベクトルを利用して、エネミーの座標を動かす
	//m_moveSpeed = toPlayerDir * -500.0f;

	////ノックバックアニメーション
	//m_animationState = enHit;
}


//ステート管理
void Bear::ManageState()
{
	// 死亡したらキャラクターコントローラーは無くなるので処理しない。
	if (m_characterController.IsOnGround() == true)
	{
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//歩くアニメーション
			m_animationState = enWalk;
		}
		else
		{
			//待機アニメーション
			m_animationState = enIdle;
		}
		if (m_toPlayer.Length() <= 200.0f)
		{
			//攻撃アニメーション
			m_animationState = enAttack;
		}
		//プレイヤーが敵にあたると-
		if (collisionObject->IsHit(m_player->m_playerCharaCon))
		{
			//ノックバックアニメーション
			m_animationState = enHit;
		}
	}
}

//アニメーション再生
void Bear::EnemyAnimation()
{
	//switch文
	switch (m_animationState)
	{
	case enIdle:	//待機アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;

	case enWalk:	//歩くアニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;

	case enAttack:	//攻撃アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack);
		break;

	case enHit:   //ノックバックアニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Hit);
		break;

	case enDeath:	//吹っ飛ぶアニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Death);
		break;

	default:
		break;
	}
}


void Bear::Render(RenderContext& rc)
{
	//モデル
	m_modelRender.Draw(rc);
}
