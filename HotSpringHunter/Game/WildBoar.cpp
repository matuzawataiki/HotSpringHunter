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
	const float MELEE_ATTACK_DAMAGE = 20.0f;	//近接攻撃の攻撃力 kaeru
	const float ATK_COOLTIME = 3.0f;			//近接攻撃のクールタイム kaeru

	const float ATK_CHARGE_RANGE = 600.0f;      //突進攻撃
	const float ATK_CHARGE_TIME = 3.0f;         //突進攻撃のクールタイム 
	const float ATK_CHARGE_SPEED = 1000.0f;	    //突進攻撃のスピード 
	const float CHARGE_COOL_TIME = 15.0f;       //次の突進攻撃するまでのクールタイム

	const float IDLE_TIME = 3.0f;				//Idleの時間 

	const Vector3 NEW_POSITION = { 0.0f,0.0f,500.0f };		//初期位置
	const Vector3 SET_SCALE = { 1.5f,1.5f,1.5f };            //イノシシの大きさ
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
	//イノシシの大きさ
	m_wildBoarModel.SetScale(Vector3(SET_SCALE));

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

	//警告表示
	m_chargeCaveat.Init("Assets/modelData/wildBoar/ChargeCaveat.tkm");
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
	//エネミーベースのステート変えたらダメな場合
	m_enemyBase->m_isCanChange = false;

	ChargeCaveat();

	//突進する前のチャージ時間 3.0f
	m_chargeTime += g_gameTime->GetFrameDeltaTime();

	//どこまで突進するか
	m_toCharge = m_player->GetPlayerPos();

	//終着点までのベクトルを求める
	//イノシシからプレイヤーまでのベクトルを求める
	Vector3 chargeOverVec = m_toCharge - m_position;

	//突進する方向を求める
	chargeOverVec.Normalize();
	//突進したときにプレイヤーより100.0f先まで走るようにする
	chargeOverVec *= 100.0f;
	m_toCharge += chargeOverVec;
	//突進する距離を求める
	m_chargeVec = m_toCharge - m_position;

	//3.0f経ったら突進するようにする
	if (m_chargeTime >= ATK_CHARGE_TIME)
	{
		//突進ステートに変える
		m_wildBoarState = enWildBoarCharge;

		//突進コリジョン
		ChargeCollision();
	}
}

/// <summary>
/// 突進攻撃
/// </summary>
void WildBoar::Charge()
{
	//イノシシからプレイヤーまでのベクトルを求める
	m_chargeVec = m_toCharge - m_position;

	//突進する方向を求める
	m_moveSpeed = m_chargeVec;
	m_moveSpeed.Normalize();
	//突進する速度を定数で入れる
	m_moveSpeed *= ATK_CHARGE_SPEED;



	//突進のコリジョンを移動させる
	collisionObject->SetPosition(m_position);

	//ダメージ判定
	if (collisionObject->IsHit(m_player->m_playerCharaCon) == true)
	{
		//プレイヤーにコリジョンが当たったらfalseにする
		if (m_isHitCollision == false)
		{
			//プレイヤーにダメージを与える
			m_player->Hit(20.0f);
			//一度当たったら判定をtrueにする
			m_isHitCollision = true;
		}
	}

	//走り終わったらIdleにもどす
	if (CanIdleState())
	{
		//コリジョンを消す
		DeleteGO(collisionObject);

		//イノシシが待機する
		m_wildBoarState = enWildBoarIdle;

		//当たった判定をfalseにする
		m_isHitCollision = false;

		m_chargeCaveat.SetScale(1.0f, 1.0f, 1.0f);
	}

	//移動前の座標を更新する
	m_chargeOldPos = m_position;
	//チャージタイムリセット
	m_chargeTime = 0.0f;
}

/// <summary>
/// アイドル状態に戻していいか
/// </summary>
bool WildBoar::CanIdleState()
{
	if (m_chargeVec.Length() <= 50.0f)
	{
		return true;
	}
	else if (collisionObject->IsHit(m_player->m_playerCharaCon) == true)
	{
		return true;
	}
	else if (m_chargeOldPos.x - m_position.x == 0.0f || m_chargeOldPos.z - m_position.z == 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void WildBoar::ManageState()
{
	//ダメージリアクションは最優先で
	//被弾した場合
	if (m_player->m_collision->IsHit(m_characterController)) {

		//HPを減らす。
		m_wildBoarHP -= m_player->m_attackPower;

		//HPがまだ残っている。
		if (m_wildBoarHP > 0.0f) {
			//ノックバック
			m_wildBoarState = enWildBoarKnockBack;
			DeleteGO(collisionObject);
		}
		//HPがなくなった
		else {
			//死亡
			m_wildBoarState = enWildBoarDeath;
			DeleteGO(collisionObject);
		}
		return;
	}

	//ステートを変えてもよいなら
	m_isCanStateChange = m_enemyBase->ChangeFlag();
	if (!m_isCanStateChange) {
		return;
	}

	//突進攻撃
	if (m_chargeCoolTime >= CHARGE_COOL_TIME)
	{
		m_wildBoarState = enWildBoarAccum;
		m_chargeCoolTime = 0.0f;
		return;
	}

	//近接攻撃
		//攻撃範囲まで近づいたら
	if (m_toPlayer.Length() < ATK_RANGE) {
		m_wildBoarState = enWildBoarAttack;
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

	m_chargeCoolTime += g_gameTime->GetFrameDeltaTime();


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
		//3秒立ったらステートがtrueになる
		m_enemyBase->m_isCanChange = false;
		if (m_idleTime >= IDLE_TIME)
		{
			m_enemyBase->m_isCanChange = true;
			m_idleTime = 0.0f;
		}

		m_idleTime += g_gameTime->GetFrameDeltaTime();
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
	if ((m_wildBoarState != enWildBoarDeath) && (m_wildBoarState != enWildBoarCharge) && (FindPlayer())) {
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

void WildBoar::ChargeCollision()
{
	//コリジョンオブジェクトを作成
	collisionObject = NewGO<CollisionObject>(0, "chargeCollision");
	Vector3 collisionPos = m_position;
	//球状のコリジョンを作成
	collisionObject->CreateSphere(collisionPos,
		Quaternion::Identity,
		90.0f);
	collisionObject->SetIsEnableAutoDelete(false);
}

void WildBoar::ChargeCaveat()
{
	//警告表示する場所
	m_chargeCaveat.SetPosition(m_position);

	//回転の更新。
	m_rotation.SetRotationYFromDirectionXZ(m_enemyDir);
	m_chargeCaveat.SetRotation(m_rotation);

	//警告表示の長さ
	m_chargeCaveat.SetScale(1.0f, 1.0f, m_chargeVec.Length() / 100.0f);

	m_chargeCaveat.Update();
}

void WildBoar::Render(RenderContext& rc)
{
	//イノシシの表示
	m_wildBoarModel.Draw(rc);

	if (m_wildBoarState == enWildBoarAccum || m_wildBoarState == enWildBoarCharge)
	{
		//突進チャージの警告を表示する
		m_chargeCaveat.SetPosition(m_position);

		//警告を描画する
		m_chargeCaveat.Draw(rc);
	}
}
