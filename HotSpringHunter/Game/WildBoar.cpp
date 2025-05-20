#include "stdafx.h"
#include "WildBoar.h"
#include "Game.h"
#include "Player.h"
#include "EnemySpawn.h"
#include "EnemyBase.h"
#include "collision/CollisionObject.h"

namespace
{
	const float FIND_RANGE = 3000.0f;			//プレイヤーを捉える距離
	const float ATK_RANGE = 300.0f;				//近接攻撃のリーチ kaeru
	const float ATK_CHARGE_RANGE = 600.0f;      //突進攻撃
	const float MELEE_ATTACK_DAMAGE = 20.0f;	//近接攻撃の攻撃力 kaeru
	const float ATK_COOLTIME = 3.0f;			//近接攻撃のクールタイム kaeru
	const Vector3 NEW_POSITION = Vector3{ 0.0f,0.0f,500.0f };		//初期位置
}

WildBoar::WildBoar()
{

}

WildBoar::~WildBoar()
{

}

bool WildBoar::Start()
{
	//アセット読み込み
	LoadAssets();

	//基底クラス生成
	m_enemyBase = NewGO<EnemyBase>(0, "enemyBase");

	//インスタンス探し
	m_player = FindGO<Player>("player");

	//キャラクターコントローラー
	m_characterController.Init(50.0f, 50.0f, m_position);

	m_position = NEW_POSITION;

	return true;
}

/// <summary>
/// アセットの読み込み
/// </summary>
void WildBoar::LoadAssets()
{
	//アニメーション読み込み
	m_animationClips[enWildBoarAnimClip_Idle].Load("Assets/animData/wildBoar/wildBoar_idle.tka");
	m_animationClips[enWildBoarAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enWildBoarAnimClip_Walk].Load("Assets/animData/wildBoar/wildBoar_walk.tka");
	m_animationClips[enWildBoarAnimClip_Walk].SetLoopFlag(true);
	m_animationClips[enWildBoarAnimClip_Run].Load("Assets/animData/wildBoar/wildBoar_run.tka");
	m_animationClips[enWildBoarAnimClip_Run].SetLoopFlag(true);
	m_animationClips[enWildBoarAnimClip_Charge].Load("Assets/animData/wildBoar/wildBoar_charge.tka");
	m_animationClips[enWildBoarAnimClip_Charge].SetLoopFlag(true);
	m_animationClips[enWildBoarAnimClip_Attack].Load("Assets/animData/wildBoar/wildBoar_attack.tka");
	m_animationClips[enWildBoarAnimClip_Attack].SetLoopFlag(false);
	m_animationClips[enWildBoarAnimClip_Hit].Load("Assets/animData/wildBoar/wildBoar_hit.tka");
	m_animationClips[enWildBoarAnimClip_Hit].SetLoopFlag(false);
	m_animationClips[enWildBoarAnimcClip_Death].Load("Assets/animData/wildBoar/wildBoar_death.tka");
	m_animationClips[enWildBoarAnimcClip_Death].SetLoopFlag(true);

	//モデル
	m_wildBoarModel.Init("Assets/modelData/wildBoar/wildBoar.tkm", m_animationClips, enWildBoarAnimClip_Num, enModelUpAxisZ);
}

void WildBoar::Update()
{
	//スポーンしているなら
	if (m_isSpawn)
	{
		//ステート管理
		ManageState();
		//行動実行
		ExecuteAction();
	}

	//いろいろ更新
	VariousUpdate();
}

/// <summary>
/// 突進チャージ
/// </summary>
void WildBoar::Accumulate()
{

}

/// <summary>
/// 突進攻撃
/// </summary>
void WildBoar::Charge()
{

}

void WildBoar::ManageState()
{
	//ステートを変えてもよいなら
	m_isCanStateChange = m_enemyBase->ChangeFlag();
	if (!m_isCanStateChange) {
		return;
	}
	//被弾した場合
	if (m_player->m_collision->IsHit(m_characterController)) {

		//HPを減らす。
		m_wildBoarHP -= m_player->m_attackPower;

		//HPがまだ残っている。
		if (m_wildBoarHP > 0.0f) {
			//ノックバック
			m_wildBoarState = enWildBoarKnockBack;
		}
		//HPがなくなった
		else {
			//死亡
			m_wildBoarState = enWildBoarDeath;
		}
		return;
	}

	//近接攻撃
	//攻撃範囲まで近づいたら
	if (m_toPlayer.Length() < ATK_RANGE) {
		m_wildBoarState = enWildBoarAttack;
		return;
	}

	//tossin
	if (m_toPlayer.Length() < ATK_CHARGE_RANGE)
	{
		m_wildBoarState = enWildBoarAccum;
		return;
	}

	//追従
	//攻撃範囲外へ出ていて、かつプレイヤーを捉えていたら
	if ((m_toPlayer.Length() > ATK_RANGE) && (FindPlayer())) {
		m_wildBoarState = enWildBoarTrack;
		return;
	}	

	//待機
	m_wildBoarState = enWildBoarIdle;
}

/// <returns></returns>
bool WildBoar::FindPlayer()
{
	if (m_toPlayer.Length() < FIND_RANGE) {
		return true;
	}
	else {
		return false;
	}
}

void WildBoar::ExecuteAction()
{
	//移動速度を0する
	m_moveSpeed = Vector3::Zero;
	//近接攻撃のクールタイムを計算
	m_ATKCoolTime -= g_gameTime->GetFrameDeltaTime();

	switch (m_wildBoarState) {

		//ノックバック
	case enWildBoarKnockBack:
		m_moveSpeed = m_enemyBase->KnockBack(m_enemyDir);
		//被弾アニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Hit);
		break;

		//死亡
	case enWildBoarDeath:
		m_moveSpeed = m_enemyBase->DeathBlown(m_enemyDir);
		//死亡アニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimcClip_Death);
		break;

		//突進チャージ
	case enWildBoarAccum:
		Accumulate();
		//突進チャージのアニメーション再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Charge);
		break;

		//突進攻撃
	case enWildBoarCharge:
		Charge();
		//突進攻撃のアニメーション再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Run);
		break;

		//近接攻撃
	case enWildBoarAttack:
		//クールタイムが0だったら
		if (m_ATKCoolTime <= 0.0f)
		{
			//近接攻撃
			m_enemyBase->MeleeAttack(m_position, m_enemyDir, MELEE_ATTACK_DAMAGE);
			//近接攻撃アニメーションを再生
			m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Attack);
			//タイマーをセット
			m_ATKCoolTime = ATK_COOLTIME;
		}
		//攻撃しないときは待機アニメーション
		else if (!m_wildBoarModel.IsPlayAnimation()) {
			m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Idle);
		}
		break;

		//追従
	case enWildBoarTrack:
		m_moveSpeed = m_enemyBase->Tracking(m_toPlayer);
		//歩きアニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Walk);
		break;

		//待機
	case enWildBoarIdle:
		//待機アニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Idle);
		break;

	default:
		break;
	}
}

void WildBoar::VariousUpdate()
{
	//プレイヤーへのベクトルを更新
	m_toPlayer = m_player->GetPlayerPos() - m_position;

	//死亡していないとき、且つプレイヤーを捉えているときだけ
	//向きの更新をする
	if ((m_wildBoarState != enWildBoarDeath) && (FindPlayer())) {
		//向きの更新。
		m_enemyDir = m_toPlayer;
		m_enemyDir.Normalize();
	}

	//速度を適応。
	ExecuteSpeed();

	//回転の更新。
	m_rotation.SetRotationYFromDirectionXZ(m_enemyDir);
	m_wildBoarModel.SetRotation(m_rotation);

	//座標の更新。
	m_wildBoarModel.SetPosition(m_position);

	//モデルの更新。
	m_wildBoarModel.Update();
}

void WildBoar::ExecuteSpeed()
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

void WildBoar::Render(RenderContext& rc)
{
	m_wildBoarModel.Draw(rc);
}
