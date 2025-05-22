#include "stdafx.h"
#include "Bear.h"
#include "Game.h"
#include "Player.h"
#include "EnemySpawn.h"
#include "EnemyBase.h"
#include "collision/CollisionObject.h"
#include "EnemyHPBar.h"

namespace {
	const float FIND_RANGE = 500.0f;			//プレイヤーを捉える距離
	const float ATK_RANGE = 300.0f;				//近接攻撃のリーチ
	const float MELEE_ATTACK_DAMAGE = 20.0f;	//近接攻撃の攻撃力
	const float ATK_COOLTIME = 3.0f;			//近接攻撃のクールタイム
	const Vector3 NEW_POSITION = Vector3{ 0.0f,0.0f,500.0f };		//初期位置
};

Bear::Bear()
{
	
}

Bear::~Bear()
{

}

bool Bear::Start()
{
	//アセット読み込み
	LoadAssets();

	//基底クラス生成
	m_enemyBase = NewGO<EnemyBase>(0, "enemyBase");

	//インスタンス探し
	m_player = FindGO<Player>("player");

	//クマを初期位置に
	m_position = NEW_POSITION;

	//キャラクターコントローラー
	m_characterController.Init(80.0f, 80.0f, m_position);

	m_hpBar = NewGO<EnemyHPBar>(0, "hpBar");
	m_hpBar->Init(m_enemyHP, m_position);

	return true;
}

void Bear::LoadAssets()
{
	//アニメーション読み込み
	m_animationClips[enBearAnimClip_Idle].Load("Assets/animData/bear/Idle.tka");
	m_animationClips[enBearAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enBearAnimClip_Run].Load("Assets/animData/bear/Run.tka");
	m_animationClips[enBearAnimClip_Run].SetLoopFlag(true);
	m_animationClips[enBearAnimClip_Attack].Load("Assets/animData/bear/Attack.tka");
	m_animationClips[enBearAnimClip_Attack].SetLoopFlag(false);
	m_animationClips[enBearAnimClip_Hit].Load("Assets/animData/bear/Hit.tka");
	m_animationClips[enBearAnimClip_Hit].SetLoopFlag(false);
	m_animationClips[enBearAnimClip_Death].Load("Assets/animData/bear/Death.tka");
	m_animationClips[enBearAnimClip_Death].SetLoopFlag(false);

	//モデル読み込み
	m_modelRender.Init("Assets/modelData/bear/bear.tkm", m_animationClips, enBearAnimClip_Num, enModelUpAxisZ);
}

void Bear::Update()
{
	//スポーンしているなら
	if (m_isSpawn)
	{
		//ステート管理
		ManageState();
		//行動実行
		ExecuteAction();
	}
	m_hpBar->SetHpBar(m_enemyHP, m_position);
	//いろいろ更新
	VariousUpdate();
}


/// <summary>
/// ステート管理
/// </summary>
void Bear::ManageState()
{
	//ステートを変えてもよいなら
	m_isCanStateChange = m_enemyBase->ChangeFlag();
	if (!m_isCanStateChange) {
		return;
	}
	//被弾した場合
	if (m_player->m_collision->IsHit(m_characterController)) {

		//HP減らす。
		m_enemyHP -= m_player->m_attackPower;

		//HPがまだ残っている。
		if (m_enemyHP > 0.0f) {
			m_bearState = enBearKnockBack;
		}
		//HPがなくなった
		else {
			m_bearState = enBearDeath;
		}
		return;
	}

	//向きの更新
	DirUpdate();

	//近接攻撃
	if (m_toPlayer.Length() < ATK_RANGE) {
		m_bearState = enBearMeleeAttack;
		return;
	}
	
	//追従
	if ((ATK_RANGE < m_toPlayer.Length()) && (m_findFlag)) {
		m_bearState = enBearTrack;
		return;
	}

	//見つけたらずっと追従し続ける
	if (m_toPlayer.Length() < FIND_RANGE) {
		m_findFlag = true;
	}
	
	//待機
	m_bearState = enBearIdle;
}

/// <summary>
/// 
/// </summary>
void Bear::DirUpdate()
{
	//向きの更新。
	m_enemyDir = m_toPlayer;
	m_enemyDir.Normalize();
}

/// <summary>
/// 行動を実行。
/// </summary>
void Bear::ExecuteAction()
{
	//移動速度を0する
	m_moveSpeed = Vector3::Zero;
	//近接攻撃のクールタイムを計算
	m_ATKCoolTime -= g_gameTime->GetFrameDeltaTime();

	switch (m_bearState) {
		//ノックバック
	case enBearKnockBack:
		m_moveSpeed = m_enemyBase->KnockBack(m_enemyDir);
		//被弾アニメーションを再生
		m_modelRender.PlayAnimation(enBearAnimClip_Hit);
		break;
		//死亡
	case enBearDeath:
		m_enemyBase->Death();
		//死亡アニメーションを再生
		m_modelRender.PlayAnimation(enBearAnimClip_Death);
		break;
		//近接攻撃
	case enBearMeleeAttack:
		//クールタイムが0だったら
		if (m_ATKCoolTime <= 0.0f)
		{
			//近接攻撃
			m_enemyBase->MeleeAttack(m_position, m_enemyDir, MELEE_ATTACK_DAMAGE);
			//近接攻撃アニメーションを再生
			m_modelRender.PlayAnimation(enBearAnimClip_Attack);
			//タイマーをセット
			m_ATKCoolTime = ATK_COOLTIME;
		}
		//攻撃しないときは待機アニメーション
		else if(!m_modelRender.IsPlayAnimation()){
			m_modelRender.PlayAnimation(enBearAnimClip_Idle);
		}		
		break;
		//追従
	case enBearTrack:
		m_moveSpeed = m_enemyBase->Tracking(m_toPlayer);
		//歩きアニメーションを再生
		m_modelRender.PlayAnimation(enBearAnimClip_Run);
		break;
		//待機
	case enBearIdle:
		//待機アニメーションを再生
		m_modelRender.PlayAnimation(enBearAnimClip_Idle);
		break;
	default:
		break;
	}
}

/// <summary>
/// いろいろ更新（座標、回転など）
/// </summary>
void Bear::VariousUpdate()
{
	//エネミーからプレイヤーに向かって伸びるベクトルを計算
	m_toPlayer = m_player->GetPlayerPos() - m_position;
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
void Bear::ExecuteSpeed()
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

void Bear::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}