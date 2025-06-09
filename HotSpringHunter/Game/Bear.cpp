#include "stdafx.h"
#include "Bear.h"
#include "Player.h"
#include "SnakeEnemy.h"
#include "EnemySpawn.h"
#include "EnemyBase.h"
#include "GameCamera.h"
#include "collision/CollisionObject.h"
#include "EnemyHPBar.h"
#include "SoundEffect.h"


namespace {
	const float BEAR_MAX_HP				= 500.0f;		//クマの最大HP
	const float MAX_BOSS_HP				= 500.0f;		//ボスの最大HP
	const float FIND_RANGE				= 1500.0f;		//プレイヤーを捉える距離
	const float PI						= 3.14;			//円周率
	const float ANIM_INTERPOLATE_TIME	= 0.2f;			//アニメーションの補間時間
	const float CONTACT_TIME			= 4.0f;			//クマ登場のイベント時間
	const float DELTA_TIME				= 1.0f / 60.0f;	//フレームレート
	const Vector3 NEW_POSITION			= { -100.0f,0.0f,3000.0f };	//初期位置

	const float ATK_RANGE				= 300.0f;		//近接攻撃：リーチ
	const float MELEE_ATTACK_DAMAGE		= 20.0f;		//近接攻撃：攻撃力
	const float ATK_COOLTIME			= 3.0f;			//近接攻撃：クールタイム

	const float FOLLOW_RANGE			= 1000.0f;		//追従：追従する距離

	const float SLOW_RANGE				= 1500.0f;		//投石：投石攻撃ができる距離
	const float SET_STONE_POS_DIS		= 160.0f;		//投石：岩をセットする位置（どれだけ前に出すか）
	const float STONE_YPOS				= 50.0f;		//投石：岩の高さ
	const float SET_STONE_TIME			= 0.3f;			//投石：岩をセットする時間
	const float SLOW_SPEED				= 1000.0f;		//投石：投球速度
	const float STONE_COL_SIZE			= 60.0f;		//投石：岩の当たり判定の大きさ
	const float STONE_REMOVE_DIS		= 10.0f;		//投石：岩を非投擲状態とする距離
	const float STONE_DAMAGE			= 30.0f;		//投石：投石のダメージ
	const float STONE_ROT_AMOUNT		= 20.0f;		//投石：岩が回転する量
	const float SLOW_COOLTIME			= 15.0f;		//投石：投石のクールタイム

	const float GO_NEW_POS_SPEED		= 500.0f;		//召喚：初期位置に移動する速さ
	const float MOVE_STOP_DIS			= 100.0f;		//召喚：初期位置への移動をやめる距離
	const float SUMMON_PCT				= 0.5f;			//召喚：召喚を行うHPの割合
	const float RADIUS					= 300.0f;		//召喚：召喚する円の半径
	const float SUMMON_YPOS				= 20.0f;		//召喚：召喚したときの高さ
	const int SUMMON_NUM				= 4;			//召喚：雑魚を召喚する数

	const float SINKING_TIME			= 0.05f;		//死亡：下に沈む速さ
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
	m_player		= FindGO<Player>("player");
	m_enemySpawn	= FindGO<EnemySpawn>("enemySpawn");
	m_gameCamera	= FindGO<GameCamera>("gameCamera");

	//クマを初期位置に
	m_bearPos = NEW_POSITION;

	//キャラクターコントローラー
	m_bearController.Init(80.0f, 80.0f, m_bearPos);

	//クマのHPをセット
	m_bearHP = BEAR_MAX_HP;

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
	//スポーンしているなら
	if (m_isSpawn) {
		//投石
		StoneThrow();
		//ステート管理
		ManageState();
		//行動実行
		ExecuteAction();
	}
	//いろいろ更新
	VariousUpdate();
	//プレイヤーを探す
	FindPlayer();
}

/// <summary>
/// 投石攻撃
/// </summary>
void Bear::StoneThrow()
{
	Vector3 slowVec		= Vector3::Zero;			//投石のベクトル
	Vector3 newStonePos = Vector3::Zero;			//岩の初期位置

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
		newStonePos = m_bearDir * SET_STONE_POS_DIS;
		newStonePos += m_bearPos;
		newStonePos.y = STONE_YPOS;
		m_stonePos = newStonePos;
		//岩の目標位置をセット
		m_toSlowPos = m_player->GetPlayerPos();
		//ちょっと高さを付ける
		m_toSlowPos.y = STONE_YPOS;
		//コリジョンを生成
		StoneCollision();
		//セット済みにする
		m_isSetStone = true;
	}

	if (m_setStoneTime > SET_STONE_TIME) {
		//投石のベクトルを計算
		slowVec = m_toSlowPos - m_stonePos;
		//このベクトルを岩の移動速度にする
		m_stoneSpeed = slowVec;
		//ベクトルを正規化
		m_stoneSpeed.Normalize();
		//ベクトルに速さを付ける
		m_stoneSpeed *= SLOW_SPEED;

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
			(slowVec.Length() <= STONE_REMOVE_DIS)) {
			//非投擲状態にする
			m_isStoneSlowing = false;
			//コリジョンを消す
			DeleteGO(m_stoneCollision);
			//岩のセット時間をリセット
			m_setStoneTime = 0.0f;
		}
	}
	//岩のモデルを更新
	m_stoneModel.Update();
	//速度で位置を動かす
	m_stoneSpeed = m_stoneSpeed * DELTA_TIME;
	m_stonePos.Add(m_stoneSpeed);
	//位置更新
	m_stoneModel.SetPosition(m_stonePos);
	//回転更新
	m_stoneModel.SetRotation(m_stoneRot);
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
/// クマを初期位置に移動させる（召喚用）
/// </summary>
void Bear::GoNewPos()
{
	Vector3 toNewPos = NEW_POSITION - m_bearPos;

	//ステートを変更不能に
	m_enemyBase->m_isCanChange = false;
	//クマの位置を初期位置に移動させる
	m_bearSpeed = toNewPos;
	m_bearSpeed.Normalize();
	m_bearSpeed *= GO_NEW_POS_SPEED;

	//ある程度初期位置に近づいたら
	if (toNewPos.Length() < MOVE_STOP_DIS) {
		//クマを手前側に向かせる
		m_bearDir = Vector3{ 0.0f,0.0f,1.0f };
		//召喚する
		m_bearState = enBearSummonMinion;
	}
}

/// <summary>
/// 手下召喚
/// </summary>
void Bear::SummonMinions()
{
	//まず位置を計算
	CalcPos();

	//雑魚を計算した位置に召喚
	for (int i = 0; i < SUMMON_NUM; i++) {
		//雑魚を生成
		m_snakeEnemy[i] = NewGO<SnakeEnemy>(0, "snakeEnemy");
		//雑魚の初期位置をセット
		m_snakeEnemy[i]->m_position = m_summonPos[0];
		//配列の頭から消す
		m_summonPos.erase(m_summonPos.begin());
	}
}

/// <summary>
/// 召喚する位置を計算
/// </summary>
void Bear::CalcPos()
{
	for (int i = 0; i < SUMMON_NUM; i++) {
		float angle = 2.0f * PI * i / SUMMON_NUM;
		float x = m_bearPos.x + RADIUS * std::cos(angle);
		float z = m_bearPos.z + RADIUS * std::sin(angle);
		m_summonPos.emplace_back(Vector3{ x,SUMMON_YPOS,z });
	}
}

/// <summary>
///	死亡後、地面に沈ませる
/// </summary>
void Bear::SinkIntoGround()
{
	//キャラコンを消す
	m_bearController.RemoveRigidBoby();

	m_bearPos.y -= SINKING_TIME;
}

/// <summary>
/// ステート管理
/// </summary>
void Bear::ManageState()
{
	//被弾した場合
	if (m_player->m_collision->IsHit(m_bearController)) {

		//HPを減らす。
		m_bearHP -= m_player->m_attackPower;

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

	//召喚
	//クマのHPが30％以下、
	//且つ召喚をまだしていないなら
	if (m_bearHP < BEAR_MAX_HP * SUMMON_PCT && !m_isSummon) {
		//まずは召喚位置へ移動
		m_bearState = enBearGoNewPos;
		//召喚を行った状態にする
		m_isSummon = true;
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
		//ステート変更を不可に
		m_enemyBase->m_isCanChange = false;
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
		}
		break;

		//召喚位置へ移動
	case enBearGoNewPos:
		//移動する
		GoNewPos();
		//移動アニメーションを再生
		m_bearModel.PlayAnimation(enBearAnimClip_Run, ANIM_INTERPOLATE_TIME);

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

		//アニメーションを再生し終わったらステート変更
		if (!m_bearModel.IsPlayAnimation()) {
			//ステート変更を可能にする
			m_enemyBase->m_isCanChange = true;
			//未召喚にする
			m_isSummonEnd = false;
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
			m_enemyBase->m_isCanChange = false;
			//クールタイムをセット
			m_ATKCoolTime = ATK_COOLTIME;
		}
		//アニメーションが終わったら
		else if (!m_bearModel.IsPlayAnimation()) {
			//ステート変更を可能にする
			m_enemyBase->m_isCanChange = true;
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

		//クマがプレイヤーを認識したら
		//イベントタイムを数える
		m_contactTime += g_gameTime->GetFrameDeltaTime();

		//咆哮アニメーションを再生
		m_bearModel.PlayAnimation(enBearAnimClip_Roar, ANIM_INTERPOLATE_TIME);
		

		////登場イベントが終わっていないなら
		if (m_contactTime <= CONTACT_TIME) {
			//カメラをイベントカメラにする
			//m_gameCamera->SetCameraState(enLookDown);
		}
		//イベント時間が終わったら
		else {
			//イベントカメラをやめる
			//m_gameCamera->SetCameraState(enLookDown);
			//召喚状態へ
			m_bearState = enBearGoNewPos;
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

	//召喚時の移動の時は移動方向に向かせる
	if (m_bearState == enBearGoNewPos) {
		m_bearDir = m_bearSpeed;
	}

	//方向の情報を正規化
	m_bearDir.Normalize();
}

/// <summary>
/// 速度を適応。
/// </summary>
void Bear::ExecuteSpeed()
{
	//敵が吹っ飛ぶときにキャラコンを消して透明な壁をすり抜ける
	//if (m_isAlive) {
	//	// キャラクターコントローラーがある時は移動処理の結果を受け取るだけ
	//	m_bearPos = m_bearController.Execute(m_bearSpeed, 1.0f / 60.0f);
	//}
	//else {
		// ないときは自分で移動結果を計算
		const Vector3 move = m_bearSpeed * DELTA_TIME;
		m_bearPos.Add(move);
	//}
}

/// <summary>
/// クマの最大HPを取得
/// </summary>
/// <returns></returns>
float Bear::GetBearMAXHP()
{
	return MAX_BOSS_HP;
}

void Bear::Render(RenderContext& rc)
{
	//クマ描画
	m_bearModel.Draw(rc);

	//岩描画（岩が飛んでいるときだけ）
	if (m_isStoneSlowing) {
		m_stoneModel.Draw(rc);
	}
}