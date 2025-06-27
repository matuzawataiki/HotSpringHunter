#include "stdafx.h"
#include "WildBoar.h"
#include "Game.h"
#include "Player.h"
#include "EnemySpawn.h"
#include "EnemyBase.h"
#include "EnemyHPBar.h"
#include "collision/CollisionObject.h"
#include "SoundEffect.h"
#include "EffectHub.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"

namespace
{
	const float MAX_WILD_BOAR_HP	= 150.0f;				//イノシシのHP
	const float DELTA_TIME			= 1.0f / 60.0f;			//フレームレート

	const float FIND_RANGE			= 3000.0f;				//プレイヤーを捉える距離
	const float ATK_RANGE			= 300.0f;				//近接攻撃のリーチ kaeru
	const float MELEE_ATTACK_DAMAGE = 20.0f;				//近接攻撃の攻撃力 kaeru
	const float ATK_COOLTIME		= 3.0f;					//近接攻撃のクールタイム kaeru
	const float TRACK_SPEED			= 25.0f;				//追従の移動速度

	const float ATK_CHARGE_RANGE	= 600.0f;				//突進攻撃
	const float ATK_CHARGE_TIME		= 3.0f;					//突進攻撃のクールタイム 
	const float ATK_CHARGE_SPEED	= 1000.0f;				//突進攻撃のスピード 
	const float CHARGE_COOL_TIME	= 15.0f;				//次の突進攻撃するまでのクールタイム

	const float IDLE_TIME			= 3.0f;					//Idleの時間 

	const float TO_NOT_SPAWNED_TIME = 5.0f;					//死亡してから非スポーン状態とする時間

	const Vector3 NEW_POSITION		= { 0.0f,0.0f,500.0f };		//初期位置
	const Vector3 SET_SCALE			= { 1.5f,1.5f,1.5f };       //イノシシの大きさ
}

WildBoar::WildBoar()
{
}

WildBoar::~WildBoar()
{
	DeleteGO(m_enemyBase);
	EnemyManager* enemyManager = FindGO<EnemyManager>("enemyManager");
	enemyManager->DeleteEnemy(this);
	DeleteGO(m_enemyHPBar);
}

bool WildBoar::Start()
{
	//インスタンス探し
	m_player = FindGO<Character::Player>("player");
	//エフェクト
	m_soundEffect = FindGO<SoundEffect>("soundEffect");

	//アセット読み込み
	LoadAssets();

	//基底クラス生成
	m_enemyBase = NewGO<EnemyBase>(0, "enemyBase");
	m_enemyHPBar = NewGO<EnemyHPBar>(0, "enemyHPBar");

	//イノシシのHPをセット
	m_wildBoarHP = MAX_WILD_BOAR_HP;

	//キャラクターコントローラー
	m_wildBoarController.Init(50.0f, 50.0f, m_wildBoarPos);

	m_wildBoarPos = NEW_POSITION;
	//イノシシの大きさ
	m_wildBoarModel.SetScale(Vector3(SET_SCALE));

	//HPバーの初期化
	m_enemyHPBar->Init(m_wildBoarHP, m_wildBoarPos, m_player->GetPlayerPos());

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
	m_chargeCaveat.Init("Assets/modelData/wildBoar/ChargeCaveat.tkm", nullptr, 0, false);
}

void WildBoar::Update()
{
	//ステート管理
	ManageState();
	//行動実行
	ExecuteAction();
	//いろいろ更新
	VariousUpdate();

	//HPバーの更新
	m_enemyHPBar->UpdateHpBar(m_wildBoarHP, m_wildBoarPos, m_player->GetPlayerPos());
}

/// <summary>
/// 突進チャージ
/// </summary>
void WildBoar::Accumulate()
{
	//エネミーベースのステート変えたらダメな場合
	m_enemyBase->SetChangeFlag(false);

	//突進チャージのコリジョンを作成
	ChargeCaveat();

	//突進する前のチャージ時間 3.0f
	m_chargeTime += g_gameTime->GetFrameDeltaTime();

	//どこまで突進するか
	m_toCharge = m_player->GetPlayerPos();

	//終着点までのベクトルを求める
	//イノシシからプレイヤーまでのベクトルを求める
	Vector3 chargeOverVec = m_toCharge - m_wildBoarPos;

	//突進する方向を求める
	chargeOverVec.Normalize();
	//突進したときにプレイヤーより100.0f先まで走るようにする
	chargeOverVec *= 100.0f;
	m_toCharge += chargeOverVec;
	//突進する距離を求める
	m_chargeVec = m_toCharge - m_wildBoarPos;

	//3.0f経ったら突進するようにする
	if (m_chargeTime >= ATK_CHARGE_TIME)
	{
		//突進ステートに変える
		m_wildBoarState = enWildBoarCharge;

		//突進コリジョン
		ChargeCollision();

		//イノシシの突進攻撃の効果音を止める
		m_soundEffect->Stop(m_runSound);
		m_runSound = nullptr;
		m_soundEffect->Stop(m_chargeSound);
		m_chargeSound = nullptr;
	}
}

/// <summary>
/// イノシシの突進警告を表示および更新
/// </summary>
void WildBoar::ChargeCaveat()
{
	//警告表示する場所
	m_chargeCaveat.SetPosition(m_wildBoarPos);

	//回転の更新。
	m_wildBoarRot.SetRotationYFromDirectionXZ(m_wildBoarDir);
	m_chargeCaveat.SetRotation(m_wildBoarRot);

	//警告表示の長さ
	m_chargeCaveat.SetScale(1.0f, 1.0f, m_chargeVec.Length() / 100.0f);

	m_chargeCaveat.Update();
}

/// <summary>
/// 突進攻撃
/// </summary>
void WildBoar::Charge()
{
	if (m_isChargeSoundPlay)
	{
		//突進の効果音
		m_runSound=m_soundEffect->Play(emWildBoarRunSE, true);
		m_isChargeSoundPlay = false;
	}
	//イノシシからプレイヤーまでのベクトルを求める
	m_chargeVec = m_toCharge - m_wildBoarPos;

	//突進する方向を求める
	m_wildBoarSpeed = m_chargeVec;
	m_wildBoarSpeed.Normalize();
	//突進する速度を定数で入れる
	m_wildBoarSpeed *= ATK_CHARGE_SPEED;

	//突進のコリジョンを移動させる
	collisionObject->SetPosition(m_wildBoarPos);

	//ダメージ判定
	if (collisionObject->IsHit(m_player->m_playerCharaCon) == true)
	{
		//プレイヤーにコリジョンが当たったらfalseにする
		if (m_isHitCollision == false)
		{
			//プレイヤーにダメージを与える
			m_player->Hit(20.0f);
			//プレイヤーにあたった効果音
			m_soundEffect->Play(enwildBoarCahrgeAttackSE, false);
			//エフェクト
			PlayEffect();
			//一度当たったら判定をtrueにする
			m_isHitCollision = true;

			//イノシシの突進攻撃の効果音を止める
			m_soundEffect->Stop(m_runSound);
			m_runSound = nullptr;
			m_soundEffect->Stop(m_chargeSound);
			m_chargeSound = nullptr;

			m_isChargeSoundPlay = true;
		}
	}

	//走り終わったらIdleにもどす
	if (CanIdleState())
	{
		//コリジョンを消す
		DeleteGO(collisionObject);

		//イノシシが待機する
		m_wildBoarState = enWildBoarIdle;

		//当たった判定を false にする
		m_isHitCollision = false;

		//イノシシの突進攻撃の効果音を止める
		m_soundEffect->Stop(m_runSound);
		m_runSound = nullptr;
		m_soundEffect->Stop(m_chargeSound);
		m_chargeSound = nullptr;

		m_isChargeSoundPlay = true;

		m_chargeCaveat.SetScale(1.0f, 1.0f, 1.0f);
	}

	//移動前の座標を更新する
	m_chargeOldPos = m_wildBoarPos;
	//チャージタイムリセット
	m_chargeTime = 0.0f;
}

/// <summary>
/// イノシシの突進エフェクトを再生
/// </summary>
void WildBoar::PlayEffect()
{
	EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(EnEffectVar::enImpact);
	Vector3 effectPos = m_wildBoarPos;
	effectPos += (m_wildBoarDir * 20.0f);
	effectPos.y += 30.0f;	
	m_effect->SetPosition(effectPos);
	m_effect->SetRotation(Quaternion::Identity);
	m_effect->SetScale({ 15.0f,15.0f,15.0f });
	m_effect->Play();
}

/// <summary>
/// イノシシの突進攻撃用のコリジョンオブジェクトを作成
/// </summary>
void WildBoar::ChargeCollision()
{
	//コリジョンオブジェクトを作成
	collisionObject = NewGO<CollisionObject>(0, "chargeCollision");
	Vector3 collisionPos = m_wildBoarPos;
	//球状のコリジョンを作成
	collisionObject->CreateSphere(collisionPos,
		Quaternion::Identity,
		90.0f);
	collisionObject->SetIsEnableAutoDelete(false);
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
	else if (m_chargeOldPos.x - m_wildBoarPos.x == 0.0f || m_chargeOldPos.z - m_wildBoarPos.z == 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// イノシシの状態を管理し、ダメージ、攻撃、追従、待機などの行動を切り替える
/// </summary>
void WildBoar::ManageState()
{
	ProjectileManager* projectileManager = FindGO<ProjectileManager>("projectileManager");
	if (projectileManager->IsChargeHit(&m_wildBoarController)) {
		m_wildBoarHP -= 0.1;
		if (m_wildBoarHP <= 0.0f) {
			//死亡
			m_wildBoarState = enWildBoarDeath;
			//死亡の効果音
			m_soundEffect->Play(enWildBoarDeathSE, false);
			DeleteGO(collisionObject);
		}
	}
	//ダメージリアクションは最優先で
	//被弾した場合
	if (m_player->m_collision->IsHit(m_wildBoarController) ||
		projectileManager->IsHit(&m_wildBoarController)) {

		//HPを減らす。
		m_wildBoarHP -= m_player->m_attackPower;

		//0以下になったら0にする
		if (m_wildBoarHP < 0.0f) {
			m_wildBoarHP = 0.0f;
		}

		//被弾エフェクト
		EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
		m_effect->Init(EnEffectVar::enEnemyHit);
		Vector3 effectPos = m_wildBoarPos;
		effectPos.y += 30.0f;
		m_effect->SetPosition(effectPos);
		m_effect->SetRotation(Quaternion::Identity);
		m_effect->SetScale({ 15.0f,15.0f,15.0f });
		m_effect->Play();

		//HPがまだ残っている。
		if (m_wildBoarHP > 0.0f) {
			//ノックバック
			m_wildBoarState = enWildBoarKnockBack;
			//被弾の効果音
			m_soundEffect->Play(enWildBoarHitSE, false);
			DeleteGO(collisionObject);
		}
		//HPがなくなった
		else {
			//死亡
			m_wildBoarState = enWildBoarDeath;
			//死亡の効果音
			m_soundEffect->Play(enWildBoarDeathSE, false);
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

		//突進チャージの効果音
		m_chargeSound=m_soundEffect->Play(enWildBoarChargeSE, true);
		
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

/// <summary>
/// プレイヤーが発見範囲内にいるかどうかを判定
/// </summary>
/// <returns>プレイヤーが発見範囲内にいる場合は true、そうでない場合は false を返す</returns>
bool WildBoar::FindPlayer()
{
	if (m_toPlayer.Length() < FIND_RANGE) {
		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// 野生のイノシシの行動を現在の状態に応じて実行
/// </summary>
void WildBoar::ExecuteAction()
{
	//移動速度を0する
	m_wildBoarSpeed = Vector3::Zero;
	//攻撃のクールタイムを計算
	m_ATKCoolTime -= g_gameTime->GetFrameDeltaTime();
	//突進のクールタイムを計算
	m_chargeCoolTime += g_gameTime->GetFrameDeltaTime();

	switch (m_wildBoarState) {

		//ノックバック
	case enWildBoarKnockBack:
		m_wildBoarSpeed = m_enemyBase->KnockBack(m_wildBoarDir);
		//被弾アニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Hit);
		break;

		//死亡
	case enWildBoarDeath:
		//死亡経過時間を計算
		m_elapsedTime += g_gameTime->GetFrameDeltaTime();
		//吹っ飛ばす
		m_wildBoarSpeed = m_enemyBase->DeathBlown(m_wildBoarDir);
		//死亡アニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimcClip_Death);
		//キャラコンを削除
		if (!m_isRemoveController) {
			m_wildBoarController.RemoveRigidBoby();
			m_isRemoveController = true;
		}
		//一定時間がたったら
		if (m_elapsedTime >= TO_NOT_SPAWNED_TIME) {
			DeleteGO(this);
		}

		break;

		//突進チャージ
	case enWildBoarAccum:
		//突進のチャージ
		Accumulate();
		//突進チャージのアニメーション再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Charge);

		break;

		//突進攻撃
	case enWildBoarCharge:
		//突進させる
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
			m_enemyBase->MeleeAttack(m_wildBoarPos, m_wildBoarDir, MELEE_ATTACK_DAMAGE);
			//近接攻撃アニメーションを再生
			m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Attack);
			//近接攻撃の効果音
			m_soundEffect->Play(enWildBoarAttackSE, false);
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
		m_wildBoarSpeed = m_enemyBase->Tracking(m_toPlayer);
		//歩きアニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Walk);
		break;

		//待機
	case enWildBoarIdle:
		//待機アニメーションを再生
		m_wildBoarModel.PlayAnimation(enWildBoarAnimClip_Idle);
		//3秒立ったらステートをtrueにする
		m_enemyBase->SetChangeFlag(false);
		if (m_idleTime >= IDLE_TIME)
		{
			m_enemyBase->SetChangeFlag(true);
			m_idleTime = 0.0f;
		}

		m_idleTime += g_gameTime->GetFrameDeltaTime();
		break;

	default:
		break;
	}
}

/// <summary>
/// イノシシの状態やプレイヤーの位置に基づいて、向き、速度、位置、回転などを更新
/// </summary>
void WildBoar::VariousUpdate()
{
	//プレイヤーへのベクトルを更新
	m_toPlayer = m_player->GetPlayerPos() - m_wildBoarPos;

	//死亡していないとき、且つプレイヤーを捉えているときだけ
	//向きの更新をする
	if ((m_wildBoarState != enWildBoarDeath) && (m_wildBoarState != enWildBoarCharge) && (FindPlayer())) {
		//向きの更新。
		m_wildBoarDir = m_toPlayer;
		m_wildBoarDir.Normalize();
	}

	//速度を適応。
	ExecuteSpeed();

	//回転の更新。
	m_wildBoarRot.SetRotationYFromDirectionXZ(m_wildBoarDir);
	m_wildBoarModel.SetRotation(m_wildBoarRot);

	//座標の更新。
	m_wildBoarModel.SetPosition(m_wildBoarPos);

	//モデルの更新。
	m_wildBoarModel.Update();
}

/// <summary>
/// イノシシの移動速度を適用し、位置を更新
/// </summary>
void WildBoar::ExecuteSpeed()
{
	//キャラコンがまだあるとき
	if(!m_isRemoveController)
	{
		m_wildBoarPos = m_wildBoarController.Execute(m_wildBoarSpeed, DELTA_TIME);
	}
	//キャラコンが削除されているとき
	else
	{
		const Vector3 moveAmount = m_wildBoarSpeed * DELTA_TIME;
		m_wildBoarPos.Add(moveAmount);
	}
}

void WildBoar::Render(RenderContext& rc)
{
	//死亡時以外は通常表示
	if (m_wildBoarState != enWildBoarDeath) {
		m_wildBoarModel.Draw(rc);
	}
	//死亡時は点滅表示を行う
	else if (m_enemyBase->IsBlinkRender()) {
		m_wildBoarModel.Draw(rc);
	}
	
	//突進中もしくは、突進チャージ中なら
	if (m_wildBoarState == enWildBoarAccum || m_wildBoarState == enWildBoarCharge)
	{
		//警告を描画する
		m_chargeCaveat.Draw(rc);
	}
}

/// <summary>
/// イノシシの移動速度を取得
/// </summary>
/// <returns>イノシシの移動速度</returns>
float WildBoar::GetWildBoarSpeed()const
{
	return TRACK_SPEED;
}
