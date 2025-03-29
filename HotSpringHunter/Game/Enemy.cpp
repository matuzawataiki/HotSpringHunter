#include "stdafx.h"
#include "Enemy.h"

#include "Player.h"

//#include "collision/CollisionObject.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//アニメーションロード
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/snake_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/snake_walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/snake_attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);

	//エネミーモデル
	m_modelRender.Init("Assets/animal/snake_model.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_position = Vector3(0.0f, 0.0f, 500.0f);
	m_characterController.Init(50.0f, 75.0f, m_position);
	//m_modelRender.Update();


	m_player = FindGO<Player>("player");

	//physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	return true;
}

void Enemy::Update()
{
	//ステート管理
	ManageState();
	//エネミーがプレイヤーに向く
	Rotation();
	//ステート管理
	EnemyAnimation();
	//当たり判定
	Judgement();
	//プレイヤーの追従
	Tracking();
	//エネミーの体力
	EnemyHP();

	m_modelRender.Update();
}

//エネミーがプレイヤーに向く
void Enemy::Rotation()
{
	m_rotation.SetRotationYFromDirectionXZ(m_toPlayer);
	m_modelRender.SetRotation(m_rotation);
}

//プレイヤーと敵の当たり判定
void Enemy::Judgement()
{
	//プレイヤーとの距離が200以下の場合
	Vector3 diff = m_player->GetPlayerPosition() - m_position;
	if (diff.Length() <= 200.0f)
	{
		//プレイヤーが出来にあたると-10
		m_enemyHP -= 10;

		//攻撃アニメーション
		m_enemyState = enEnemyAttack;

		//前方にコリジョンを作る
	}
}

//範囲内のプレイヤーを追いかける
void Enemy::Tracking()
{
	//エネミーからプレイヤーに向かって伸びるベクトルを計算
	m_toPlayer = m_player->GetPlayerPosition() - m_position;

	//エネミーからプレイヤーに向かって伸びるベクトル
	//g_k2Engine->DrawVector(m_toPlayer, m_position);


	//プレイヤーとの距離を計算する
	float distToPlayer = m_toPlayer.Length();

	//プレイヤーとの距離が400以下だったら追いかける
	if (distToPlayer < 400.0f && distToPlayer >= 200.0f)
	{
		//プレイヤーに向かっている伸びるベクトルを正規化
		Vector3 toPlayerDir = m_toPlayer;
		toPlayerDir.Normalize();

		//正規化で求めたベクトルを利用して、エネミーの座標を動かす
		m_moveSpeed = toPlayerDir * 100.0f;

		//歩くアニメーション
		m_enemyState = enEnemyWalk;
	}
	else
	{
		m_moveSpeed;
	}

	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);
	m_modelRender.SetPosition(m_position);
}

//敵のHPの表示
void Enemy::EnemyHP()
{
	wchar_t enemy[256];
	swprintf_s(enemy, 256, L"%d", int(m_enemyHP));

	m_fontRender.SetText(enemy);
	m_fontRender.SetPosition({ 800.0f, -400.0f, 0.0f });
	m_fontRender.SetScale(1.0f);
	m_fontRender.SetColor(g_vec4Black);
}

//エネミーの攻撃コリジョン
void Enemy::EnemyAttackCollision()
{
	////コリジョンオブジェクトを作成
	//collisionObject = NewGO<CollisionObject>(0);  // ::~⇒エラー対策
	//Vector3 collisionPosition = m_position;
	////座標をプレイヤーの少し前に設定
	//collisionPosition += m_forward * 50.0f;
	////球状のコリジョンを作成
	//collisionObject->CreateSphere(collisionPosition,
	//	Quaternion::Identity,
	//	70.0f);
	////名前をつける
	//collisionObject->SetName("enemy_atk");
}


//ステート管理
void Enemy::ManageState()
{
	if (m_characterController.IsOnGround() == true)
	{
		//待機アニメーション
		m_enemyState = enEnemyIdle;
	}

}

//アニメーション再生
void Enemy::EnemyAnimation()
{
	//switch文
	switch (m_enemyState)
	{
	case enEnemyIdle:	//待機アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;

	case enEnemyWalk:	//歩くアニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Walk);
		break;

	case enEnemyAttack:	//攻撃アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack);
		break;

	default:
		break;
	}
}


void Enemy::Render(RenderContext& rc)
{
	//モデル
	m_modelRender.Draw(rc);
	//フォント
	m_fontRender.Draw(rc);
}


