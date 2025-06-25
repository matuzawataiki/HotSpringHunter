#include "stdafx.h"
#include "Bear.h"
#include "Player.h"
#include "SnakeEnemy.h"
#include "Enemy/PoisonSnake/PoisonSnake.h"
#include "WildBoar.h"
#include "UI.h"
#include "EnemySpawn.h"
#include "EnemyBase.h"
#include "EnemyManager.h"
#include "GameCamera.h"
#include "collision/CollisionObject.h"
#include "EnemyHPBar.h"
#include "SoundEffect.h"
#include "EffectHub.h"
#include "ProjectileManager.h"



namespace {
	const float MAX_BEAR_HP				= 5.0f;			//クマの最大HP
	const float FIND_RANGE				= 1500.0f;		//プレイヤーを捉える距離
	const float PI						= 3.14;			//円周率
	const float ANIM_INTERPOLATE_TIME	= 0.2f;			//アニメーションの補間時間
	const float CONTACT_TIME			= 4.0f;			//クマ登場のイベント時間
	const float DELTA_TIME				= 1.0f / 60.0f;	//フレームレート

	const float ATK_RANGE				= 300.0f;		//近接攻撃：リーチ
	const float MELEE_ATTACK_DAMAGE		= 20.0f;		//近接攻撃：攻撃力
	const float ATK_COOLTIME			= 3.0f;			//近接攻撃：クールタイム

	const float COVER_HIT_TIME			= 1.0f;			//拘束攻撃：当たり判定を出す時間
	const float COVER_COLLISION_DIS		= 20.0f;		//拘束攻撃：当たり判定を前に出す量
	const float COVER_COLLISION_SIZE	= 3000.0f;		//拘束攻撃：当たり判定の半径
	const float COVER_COOLTIME			= 5.0f;		//拘束攻撃：クールタイム
	const float ON_THE_PLAYER_TIME		= 1.0f;			//拘束攻撃：プレイヤーに乗りかかる時間
	const float ON_THE_PLAYER_DIS		= 50.0f;		//拘束攻撃：プレイヤーに乗りかかる距離
	const float COVER_TIME				= 5.0f;			//拘束攻撃：拘束時間

	const float BLOW_POS_DIS			= 1000.0f;		//ぶっ飛ばし：ぶっ飛ばす距離

	const float FOLLOW_RANGE			= 1000.0f;		//追従：追従する距離

	const float SLOW_RANGE				= 1500.0f;		//投石：投石攻撃ができる距離
	const float SLOW_COOLTIME			= 3.0f;			//投石：投石のクールタイム
	const float SET_STONE_POS_DIS		= 160.0f;		//投石：岩をセットする位置（どれだけ前に出すか）
	const float SET_STONE_TIME			= 0.3f;			//投石：岩をセットする時間
	const float SLOW_SPEED				= 500.0f;		//投石：投球速度
	const float STONE_COL_SIZE			= 60.0f;		//投石：岩の当たり判定の大きさ
	const float STONE_REMOVE_DIS		= 10.0f;		//投石：岩を非投擲状態とする距離
	const float STONE_DAMAGE			= 30.0f;		//投石：投石のダメージ
	const float STONE_ROT_AMOUNT		= 20.0f;		//投石：岩が回転する量
	const float SLOW_HIGHT				= 250.0f;		//投石：投石時の岩の最高度
	const float VEC_DIVIDE				= 1000.0f;		//投石：投石の距離の長さベクトルを割る数

	const float GO_NEW_POS_SPEED		= 500.0f;		//召喚：初期位置に移動する速さ
	const float MOVE_STOP_DIS			= 100.0f;		//召喚：初期位置への移動をやめる距離
	const float SUMMON_PCT				= 0.5f;			//召喚：召喚を行うHPの割合
	const float RADIUS					= 300.0f;		//召喚：召喚する円の半径
	const float SUMMON_YPOS				= 20.0f;		//召喚：召喚したときの高さ
	const float SUMMON_COOLTIME			= 15.0f;		//召喚：クールタイム
	const int SUMMON_NUM				= 4;			//召喚：雑魚を召喚する数

	const float SINKING_TIME			= 0.05f;		//死亡：下に沈む速さ

	/// <summary>
	/// 渡された値を設定した最小、最大の範囲内に設定して返す
	/// </summary>
	/// <param name="value">設定する値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>設定後の値</returns>
	inline float Clamp(float value, float min, float max) 
	{
		if (value < min) {
			value = min;
		}
		if (value > max) {
			value = max;
		}

		return value;
	}
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

	//サウンドエフェクト
	m_soundEffect = FindGO<SoundEffect>("soundEffect");

	//インスタンス探し
	m_player		= FindGO<Character::Player>("player");
	m_enemySpawn	= FindGO<EnemySpawn>("enemySpawn");
	m_gameCamera	= FindGO<GameCamera>("gameCamera");

	//キャラクターコントローラー
	m_bearController.Init(80.0f, 80.0f, m_bearPos);

	//クマのHPをセット
	m_bearHP = MAX_BEAR_HP;

	return true;
}
/// <summary>
/// アセットを読み込む
/// </summary>
void Bear::LoadAssets()
{
	//アニメーション読み込み
	m_animationClips[enBearAnimClip_Idle].Load("Assets/animData/bear/Idle.tka");
	m_animationClips[enBearAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enBearAnimClip_Run].Load("Assets/animData/bear/Run.tka");
	m_animationClips[enBearAnimClip_Run].SetLoopFlag(true);
	m_animationClips[enBearAnimClip_NeleeAttack].Load("Assets/animData/bear/Attack.tka");
	m_animationClips[enBearAnimClip_NeleeAttack].SetLoopFlag(false);
	m_animationClips[enBearAnimClip_Covering].Load("Assets/animData/bear/ThrowingTrees.tka");
	m_animationClips[enBearAnimClip_Covering].SetLoopFlag(false);
	m_animationClips[enBearAnimClip_SlowStone].Load("Assets/animData/bear/SlowStone.tka");
	m_animationClips[enBearAnimClip_SlowStone].SetLoopFlag(false);
	m_animationClips[enBearAnimClip_Roar].Load("Assets/animData/bear/Roar.tka");
	m_animationClips[enBearAnimClip_Roar].SetLoopFlag(false);
	m_animationClips[enBearAnimClip_Hit].Load("Assets/animData/bear/Hit.tka");
	m_animationClips[enBearAnimClip_Hit].SetLoopFlag(false);
	m_animationClips[enBearAnimClip_Death].Load("Assets/animData/bear/Death.tka");
	m_animationClips[enBearAnimClip_Death].SetLoopFlag(false);

	//クマモデル読み込み
	m_bearModel.Init("Assets/modelData/bear/bear.tkm", m_animationClips, enBearAnimClip_Num, enModelUpAxisZ);
	//投石の岩モデル読み込み
	m_stoneModel.Init("Assets/modelData/bear/slowStone.tkm");
}

void Bear::Update()
{
	//投石
	StoneThrow();
	//プレイヤーを探す
	FindPlayer();
	//ステート管理
	ManageState();
	//行動実行
	ExecuteAction();	
	//いろいろ更新
	VariousUpdate();
	
}

/// <summary>
/// 投石攻撃
/// </summary>
void Bear::StoneThrow()
{
	//岩が飛んでいないなら実行しない
	if (!m_isStoneSlowing) {
		//岩を未セットにする
		m_isSetStone = false;
		return;
	}

	//岩の準備時間を計算
	m_setStoneTime += g_gameTime->GetFrameDeltaTime();

	//初めにいろいろセット
	if (!m_isSetStone) {
		//岩をクマのちょっと前にセット
		m_newStonePos = m_bearDir * SET_STONE_POS_DIS;
		m_newStonePos += m_bearPos;
		m_stonePos = m_newStonePos;
		//岩の目標位置をセット
		m_toSlowPos = m_player->GetPlayerPos();
		//コリジョンを生成
		StoneCollision();
		//セット済みにする
		m_isSetStone = true;
		//岩を描画する
		m_isStoneDraw = true;
	}

	if (m_setStoneTime > SET_STONE_TIME) {
		
		//投石の開始位置から目標位置への距離を計算
		Vector3 slowVec = m_toSlowPos - m_newStonePos;
		//投石のベクトルを計算（放物線）
		m_flightTime += g_gameTime->GetFrameDeltaTime();
		//岩を投げてからの経過時間を計算
		float elapsedTime = m_flightTime / (slowVec.Length() / VEC_DIVIDE);
		elapsedTime = Clamp(elapsedTime, 0.0f, 1.0f);
		//岩を放物線で動かす
		m_stonePos = CalcStoneVec(m_newStonePos, m_toSlowPos, elapsedTime);

		//5°ずつ回転させる
		m_stoneRot.AddRotationDegX(STONE_ROT_AMOUNT);

		//コリジョンの位置を更新する
		m_stoneCollision->SetPosition(m_stonePos);
		//岩がプレイヤーにヒットしたら
		if (m_stoneCollision->IsHit(m_player->m_playerCharaCon)) {
			//プレイヤーにダメージを与える
			m_player->Hit(STONE_DAMAGE);
		}

		//プレイヤーが岩にぶつかった、
		//もしくは岩が目標位置にたどり着いたら
		if ((m_stoneCollision->IsHit(m_player->m_playerCharaCon)) ||
			(elapsedTime >= 1.0f) ){
			//非投擲状態にする
			m_isStoneSlowing = false;
			//岩の描画をやめる
			m_isStoneDraw = false;
			//コリジョンを消す
			DeleteGO(m_stoneCollision);
			//岩のセット時間をリセット
			m_setStoneTime = 0.0f;
			//経過時間をリセット
			m_flightTime = 0.0f;
		}
	}
	
	//速度で位置を動かす
	const Vector3 moveAmount = m_stoneSpeed * DELTA_TIME;
	m_stonePos.Add(moveAmount);
	//位置更新
	m_stoneModel.SetPosition(m_stonePos);
	//回転更新
	m_stoneModel.SetRotation(m_stoneRot);
	//岩のモデルを更新
	m_stoneModel.Update();
}

/// <summary>
/// 投石を放物線で飛ばす
/// </summary>
/// <param name="start">投石の開始位置</param>
/// <param name="target">投石の目標位置</param>
/// <param name="t">投石が目標位置に到達するまでの時間</param>
/// <returns>岩の位置</returns>
Vector3 Bear::CalcStoneVec(Vector3& start, Vector3& target, const float t)
{
	// 線形補間    
	Vector3 flatPos = start + (target - start) * t;

	// 放物線によるY補正
	float yOffset = SLOW_HIGHT * (1 - 4 * (t - 0.5f) * (t - 0.5f));

	flatPos.y += yOffset;

	return flatPos;
}

/// <summary>
/// 岩のコリジョンを生成
/// </summary>
void Bear::StoneCollision()
{
	m_stoneCollision = NewGO<CollisionObject>(0, "stoneCollision");
	m_stoneCollision->CreateSphere(m_stonePos, Quaternion::Identity, STONE_COL_SIZE);
	m_stoneCollision->SetIsEnableAutoDelete(false);
}

/// <summary>
/// 手下召喚
/// </summary>
void Bear::SummonMinions()
{
	EnemyManager* enemyManager = FindGO<EnemyManager>("enemyManager");
	SnakeEnemy* snake;
	Enemy::PoisonSnake* poisonSnake;
	WildBoar* wildBoar;

	//まず位置を計算
	CalcPos();

	//雑魚を計算した位置に召喚
	for (int i = 0; i < SUMMON_NUM; i++) {
		
		int random = rand() % 3;
		switch (random)
		{
		case 0:
			snake = NewGO<SnakeEnemy>(0, "snake");
			snake->SetSnakePos(m_summonPos.front());
			enemyManager->SetSnake(snake);
			break;

		case 1:
			poisonSnake = NewGO<Enemy::PoisonSnake>(0, "poisonSnake");
			poisonSnake->SetPosition(m_summonPos.front());
			enemyManager->SetPoisonSnake(poisonSnake);
			break;

		case 2:
			wildBoar = NewGO<WildBoar>(0, "wildBoar");
			wildBoar->SetWildBoarPos(m_summonPos.front());
			enemyManager->SetWildBoar(wildBoar);
			break;
		}
	}
	m_summonPos.clear();
}

/// <summary>
/// 召喚する位置を計算
/// </summary>
void Bear::CalcPos()
{
	for (int i = 0; i < SUMMON_NUM; i++) {
		//前方180度に召喚する
		float angle = PI * i / (SUMMON_NUM - 1);

		Vector3 right = Vector3{ -m_bearDir.z, 0.0f, m_bearDir.x };

		float xOffset = std::cos(angle);
		float zOffset = std::sin(angle);
		Vector3 dir = right * xOffset + m_bearDir * zOffset;

		float x = m_bearPos.x + dir.x * RADIUS;
		float z = m_bearPos.z + dir.z * RADIUS;

		m_summonPos.emplace_back(Vector3{ x, SUMMON_YPOS, z });
	}
}

/// <summary>
///	死亡後、地面に沈ませる
/// </summary>
void Bear::SinkIntoGround()
{
	//キャラコンを消す
	m_bearController.RemoveRigidBoby();

	//地面に沈ませる
	m_bearPos.y -= SINKING_TIME;
}

/// <summary>
/// ステート管理
/// </summary>
void Bear::ManageState()
{
	ProjectileManager* projectileManager = FindGO<ProjectileManager>("projectileManager");
	if (projectileManager->IsChargeHit(&m_bearController)) {
		m_bearHP -= 0.1;
		if (m_bearHP <= 0.0f) {
			//死亡
			m_bearState = enBearDeath;
			//死亡の効果音
			m_soundEffect->Play(enBearDeathSE, false);
		}
	}
	//被弾した場合
	if (m_player->m_collision->IsHit(m_bearController)||
		projectileManager->IsHit(&m_bearController)) {

		//HPを減らす。
		m_bearHP -= m_player->m_attackPower;

		//被弾エフェクト
		EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
		m_effect->Init(EnEffectVar::enEnemyHit);
		Vector3 effectPos = m_bearPos;
		effectPos += (m_bearDir * 200.0f);
		effectPos.y += 150.0f;
		m_effect->SetPosition(effectPos);
		m_effect->SetRotation(Quaternion::Identity);
		m_effect->SetScale({ 15.0f,15.0f,15.0f });
		m_effect->Play();

		//HPがまだ残っている。
		if (m_bearHP > 0.0f) {
			//ノックバック
			m_bearState = enBearKnockBack;
			//被弾の効果音
			m_soundEffect->Play(enBearHitSE, false);
		}
		//HPがなくなった
		else {
			//死亡
			m_bearState = enBearDeath;
			//死亡の効果音
			m_soundEffect->Play(enBearDeathSE, false);
		}
		return;
	}

	//ステートを変えてもよいなら
	m_isCanStateChange = m_enemyBase->ChangeFlag();
	if (!m_isCanStateChange) {
		return;
	}

	////召喚
	////クールタイムが終わっていたら
	//if (m_summonCoolTime <= 0.0f) {
	//	m_bearState = enBearSummonMinion;
	//	//クールタイムをセット
	//	m_summonCoolTime = SUMMON_COOLTIME;
	//	return;
	//}

	//拘束攻撃
	//クールタイムが終わっていたら
	if (m_coverCoolTime <= 0.0f && m_toPlayer.Length() < ATK_RANGE) {
		m_bearState = enBearCoverAttack;
		return;
	}

	//近接攻撃
	//近接攻撃範囲まで近づいたら
	if (m_toPlayer.Length() < ATK_RANGE) {
		m_bearState = enBearMeleeAttack;
		return;
	}

	//追従
	//追従範囲外に入っていたら
	if (m_toPlayer.Length() < FOLLOW_RANGE) {
		m_bearState = enBearTrack;
		return;
	}

	//投石攻撃
	//投石攻撃の攻撃範囲まで近づいたら
	if ((m_toPlayer.Length() < SLOW_RANGE) && (m_slowCoolTime <= 0.0f)) {
		m_bearState = enBearSlowStone;
		//投石のクールタイムをセット
		m_slowCoolTime = SLOW_COOLTIME;
		return;
	}

	//待機
	m_bearState = enBearIdle;
}

/// <summary>
/// プレイヤーを探す
/// </summary>
void Bear::FindPlayer()
{
	if (m_isContact) {
		return;
	}
	if (m_toPlayer.Length() < FIND_RANGE) {
		//認識済みにする
		m_isContact = true;
		//クマを認識状態にする
		m_bearState = enbearContact;
		//咆哮の効果音
		m_soundEffect->Play(enBearRoarSE, false);
		//接触時のイベントカメラにする
		m_gameCamera->SetCameraState(EnCameraVar::enBearContact);
		//ステート変更を不可に
		m_enemyBase->SetChangeFlag(false);
	}
}

/// <summary>
/// 行動を実行。
/// </summary>
void Bear::ExecuteAction()
{
	//移動速度を0する
	m_bearSpeed = Vector3::Zero;
	//近接攻撃のクールタイムを計算
	m_ATKCoolTime -= g_gameTime->GetFrameDeltaTime();
	//投石のクールタイムを計算
	m_slowCoolTime -= g_gameTime->GetFrameDeltaTime();
	//召喚のクールタイムを計算
	m_summonCoolTime -= g_gameTime->GetFrameDeltaTime();
	//拘束攻撃のクールタイムを計算
	m_coverCoolTime -= g_gameTime->GetFrameDeltaTime();

	switch (m_bearState) {

		//ノックバック
	case enBearKnockBack:
		m_bearSpeed = m_enemyBase->KnockBack(m_bearDir);
		//被弾アニメーションを再生
		m_bearModel.PlayAnimation(enBearAnimClip_Hit, ANIM_INTERPOLATE_TIME);
		break;

		//死亡
	case enBearDeath:
		//死亡させる
		m_enemyBase->Death();
		//死亡アニメーションを再生（一回だけ）
		if (!m_isPlayDeadAnim) {
			m_bearModel.PlayAnimation(enBearAnimClip_Death, ANIM_INTERPOLATE_TIME);
			m_isPlayDeadAnim = true;
		}

		//アニメーションを再生し終わったら
		if (!m_bearModel.IsPlayAnimation())	{
			//地面に沈ませる
			SinkIntoGround();
			EnemyManager* enemyManager = FindGO<EnemyManager>("enemyManager");
			enemyManager->DeleteBoss();
		}
		break;

		//召喚
	case enBearSummonMinion:

		//まだ召喚していなかったら
		if (!m_isSummonEnd) {
			//雑魚を召喚する
			SummonMinions();
			//咆哮アニメーションを再生
			m_bearModel.PlayAnimation(enBearAnimClip_Roar, ANIM_INTERPOLATE_TIME);
			//召喚済みにする
			m_isSummonEnd = true;
		}

		////アニメーションを再生し終わったらステート変更
		//if (!m_bearModel.IsPlayAnimation()) {
		//	//ステート変更を可能にする
		//	m_enemyBase->SetChangeFlag(true);
		//	//未召喚にする
		//	m_isSummonEnd = false;
		//}
		break;

		//拘束攻撃
	case enBearCoverAttack:

		//覆いかぶさり攻撃のアニメーション
		m_bearModel.PlayAnimation(enBearAnimClip_Covering, ANIM_INTERPOLATE_TIME);

		//ステート変更を不可に
		m_enemyBase->SetChangeFlag(false);

		//攻撃判定を出す時間を計算
		m_coverTime += g_gameTime->GetFrameDeltaTime();

		if (m_coverTime >= COVER_HIT_TIME && !m_isPutCoverCollision) {
			//当たり判定を出す位置を計算
			Vector3 collisionPos = m_bearPos + (m_bearDir * COVER_COLLISION_DIS);
			m_coverCollision = NewGO<CollisionObject>(0, "coverCollision");
			m_coverCollision->CreateSphere(collisionPos, Quaternion::Identity, COVER_COLLISION_SIZE);
			m_coverCollision->SetIsEnableAutoDelete(false);

			if (m_coverCollision->IsHit(m_player->m_playerCharaCon)) {
				m_isCovering = true;
				//クマをプレイヤーの位置に
				Vector3 toGoalDis = m_player->GetPlayerPos() - m_bearPos;
				toGoalDis.y = 0.0f;
				m_bearMoveAmount = toGoalDis / ON_THE_PLAYER_TIME;
				m_bearMoveAmount *= DELTA_TIME;			
			}

			DeleteGO(m_coverCollision);

			m_isPutCoverCollision = true;
		}
		
		if (m_isCovering) {
			if (m_toPlayer.Length() >= ON_THE_PLAYER_DIS) {
				m_bearPos += m_bearMoveAmount;
				m_bearController.SetPosition(m_bearPos);
			}
			
			if (m_isSlowPlayer) {
				m_bearState = enBearSlowPlayer;
				m_coverTime = 0.0f;
				m_isPutCoverCollision = false;
			}
		}

		//攻撃が当たらなかったら
		if (!m_isCovering && !m_bearModel.IsPlayAnimation()) {
			//ステート変更を可にする
			m_enemyBase->SetChangeFlag(true);
			//フラッグをリセット
			m_isPutCoverCollision = false;
			//タイマーをリセット
			m_coverTime = 0.0f;
		}

		break;

		//プレイヤーぶっ飛ばし攻撃
	case enBearSlowPlayer:

		//投石と同じアニメーションを再生
		m_bearModel.PlayAnimation(enBearAnimClip_SlowStone, ANIM_INTERPOLATE_TIME);

		//プレイヤーが着地したらステート変更
		if (!m_isCovering) {
			//フラッグをリセット
			m_isSlowPlayer = false;
			//クールタイムをセット
			m_coverCoolTime = COVER_COOLTIME;
			//ステート変更を可にする
			m_enemyBase->SetChangeFlag(true);
		}

		break;

		//近接攻撃
	case enBearMeleeAttack:
		//クールタイムが0だったら
		if (m_ATKCoolTime <= 0.0f)
		{
			//近接攻撃
			m_enemyBase->MeleeAttack(m_bearPos, m_bearDir, MELEE_ATTACK_DAMAGE);
			//近接攻撃アニメーションを再生
			m_bearModel.PlayAnimation(enBearAnimClip_NeleeAttack, ANIM_INTERPOLATE_TIME);
			//近接攻撃の効果音
			m_soundEffect->Play(enBearAttackSE, false);
			//タイマーをセット
			m_ATKCoolTime = ATK_COOLTIME;
		}
		//攻撃しないときは待機アニメーション
		else if (!m_bearModel.IsPlayAnimation()) {
			m_bearModel.PlayAnimation(enBearAnimClip_Idle, ANIM_INTERPOLATE_TIME);
		}
		break;

		//追従
	case enBearTrack:
		//追従させる
		m_bearSpeed = m_enemyBase->Tracking(m_toPlayer);
		//歩きアニメーションを再生
		m_bearModel.PlayAnimation(enBearAnimClip_Run, ANIM_INTERPOLATE_TIME);
		break;

		//投石攻撃
	case enBearSlowStone:

		//クールタイムが終わっていたら
		if (m_ATKCoolTime <= 0.0f) {
			//岩を飛ばしている状態にする
			m_isStoneSlowing = true;
			//投石アニメーションを再生
			m_bearModel.PlayAnimation(enBearAnimClip_SlowStone, ANIM_INTERPOLATE_TIME);
			//投石の効果音
			m_soundEffect->Play(enBearStoneAttackSE, false);
			//ステート変更を不可にする
			m_enemyBase->SetChangeFlag(false);
			//クールタイムをセット
			m_ATKCoolTime = ATK_COOLTIME;
		}
		//アニメーションが終わったら
		else if (!m_bearModel.IsPlayAnimation()) {
			//ステート変更を可能にする
			m_enemyBase->SetChangeFlag(true);
			//待機アニメーションを再生
			m_bearModel.PlayAnimation(enBearAnimClip_Idle, ANIM_INTERPOLATE_TIME);
		}
		break;

		//待機
	case enBearIdle:
		//待機アニメーションを再生
		m_bearModel.PlayAnimation(enBearAnimClip_Idle, ANIM_INTERPOLATE_TIME);
		break;

		//クマ登場のイベント
	case enbearContact:
		if (!m_isPlayRoar) {
			//咆哮アニメーションを再生
			m_bearModel.PlayAnimation(enBearAnimClip_Roar, ANIM_INTERPOLATE_TIME);

			//咆哮エフェクト
			EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
			m_effect->Init(EnEffectVar::enRoar);
			Vector3 effectPos = m_bearPos;
			effectPos += (m_bearDir * 30.0f); //クマの前方にエフェクトを出す
			effectPos.y = 60.0f;
			m_effect->SetPosition(effectPos);
			m_effect->SetRotation(Quaternion::Identity);
			m_effect->SetScale({ 40.0f,40.0f,40.0f });
			m_effect->Play();

			m_isPlayRoar = true;
		}		
		//アニメーションが再生し終わったら待機アニメーション
		if (!m_bearModel.IsPlayAnimation()) {
			m_bearModel.PlayAnimation(enBearAnimClip_Idle, ANIM_INTERPOLATE_TIME);
		}

		//登場イベントが終わったら
		if (m_gameCamera->GetCameraState() != EnCameraVar::enBearContact) {
			//ボスのHPを生成
			NewGO<BossHPUI>(0, "bossHPUI");
			//ステート変更を可に
			m_enemyBase->SetChangeFlag(true);
		}
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
	//プレイヤーへのベクトルを更新
	m_toPlayer = m_player->GetPlayerPos() - m_bearPos;

	//クマの方向を更新
	DirUpdate();

	//速度を適応。
	ExecuteSpeed();

	//回転の更新。
	m_bearRot.SetRotationYFromDirectionXZ(m_bearDir);
	m_bearModel.SetRotation(m_bearRot);

	//座標の更新。
	m_bearModel.SetPosition(m_bearPos);

	//モデルの更新。
	m_bearModel.Update();
}

/// <summary>
/// クマの方向を更新
/// </summary>
void Bear::DirUpdate()
{
	//クマがノックバックしているときと、死亡しているときは向きを更新しない
	if (m_bearState == enBearKnockBack || m_bearState == enBearDeath) {
		return;
	}

	//基本は、クマはプレイヤーの方向を向く。
	if (m_isContact) {
		m_bearDir = m_toPlayer;
	}

	//方向の情報を正規化
	m_bearDir.Normalize();
}

/// <summary>
/// 速度を適応。
/// </summary>
void Bear::ExecuteSpeed()
{	
	//キャラコンが削除されていないとき
	if (!m_isRemoveController) {
		m_bearPos = m_bearController.Execute(m_bearSpeed, DELTA_TIME);
	}
	//キャラコンが削除されているとき
	else {
		const Vector3 moveAmount = m_bearSpeed * DELTA_TIME;
		m_bearPos.Add(moveAmount);
	}
}

/// <summary>
/// クマの最大HPを取得
/// </summary>
/// <returns>クマの最大HP</returns>
float Bear::GetBearMAXHP()
{
	return MAX_BEAR_HP;
}

/// <summary>
/// 初期の拘束時間を取得
/// </summary>
/// <returns>初期の拘束時間</returns>
float Bear::GetCOVER_TIME()
{
	return COVER_TIME;
}

/// <summary>
/// ぶっ飛ばしの距離を取得
/// </summary>
/// <returns>ぶっ飛ばしの距離</returns>
float Bear::GetBLOW_POS_DIS()
{
	return BLOW_POS_DIS;
}

void Bear::Render(RenderContext& rc)
{
	m_bearModel.Draw(rc);

	//岩描画（岩が飛んでいるときだけ）
	if (m_isStoneDraw) {
		m_stoneModel.Draw(rc);
	}
}