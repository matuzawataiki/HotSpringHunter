#include "stdafx.h"
#include "Player.h"
#include "SnakeEnemy.h"
#include "SoundEffect.h"
#include "EnemyManager.h"
#include "GameCamera.h"


namespace Character {
	namespace {

		const Vector3 PLAYER_NEW_POSITION	= { 0.0f,300.0f,0.0f };	//player初期位置。

		const float MAX_PLAYER_HP			= 300.0f;		//最大HP。
		const float ANIM_INTERPOLATE_TIME	= 0.2f;			//アニメーションの補間時間
		const float DELTA_TIME				= 1.0f / 60.0f;	//フレームレート

		const float MOVE_AMOUNT				= 2000.0f;		//移動：移動量。
		const float GRAVITY_AMOUNT			= 10.0f;		//移動：重力量。

		const float WEAK_COLLISION_DIS		= 100.0f;		//弱攻撃：コリジョン位置。
		const float WEAK_COLLISION_SIZE		= 250.0f;		//弱攻撃：コリジョンサイズ。
		const float WEAK_ATTACK_POWER		= 50.0f;		//弱攻撃：攻撃力。
		const float SUCTION_CONDITION_DIS	= 500.0f;		//弱攻撃：吸いつきを行う条件の距離
		const float SUCTION_TARGET_POS_DIS	= 10.0f;		//弱攻撃：攻撃吸いつきの位置の距離
		const float SUCTION_TIME			= 1.0f;			//弱攻撃：吸いつきを行う時間

		const float CHARGE_DECREASE			= 0.5f;			//溜め攻撃：チャージ減少量。
		const float CHARGE_ADD_VALUE		= 2.0f;			//溜め攻撃：チャージ増加量（倍率）。
		const float CHARGE_COLLISION_SIZE	= 3.0f;			//溜め攻撃：コリジョンの大きさの倍率。
		const float COLLISION_SIZE_LOWEST	= 150.0f;		//溜め攻撃：コリジョンの大きさの最低保証。
		const float CHARGE_MAX				= 100.0f;		//溜め攻撃：チャージ最大値。
		const float CHARGE_POWER			= 2.0f;			//溜め攻撃：攻撃力の倍率。
		const float CHANGE_WEAK				= 20.0f;		//溜め攻撃：弱攻撃になるチャージ。
		const float NEW_CHARGE				= 5.0f;			//溜め攻撃：チャージの初期値。

		const float GUARD_TOLERANCE			= 1.0f;			//ガード：ガード可能な角度。

		const float HIT_RIGIDITY_TiME		= 0.5f;			//被弾：硬直時間。

		const float DEATH_MOTION_TIME		= 3.0f;			//死亡：ゲームオーバーに移行するまでの時間。

		/// <summary>
		///	Rスティックが入力されているか
		/// </summary>
		/// <returns>入力中ならならtrue,非入力中ならfalse</returns>
		inline bool IsInputStickR()
		{
			if ((fabsf(g_pad[0]->GetRStickXF()) >= FLT_EPSILON) || (fabsf(g_pad[0]->GetRStickYF()) >= FLT_EPSILON)) {
				return true;
			}

			return false;
		}

		/// <summary>
		/// Lスティックが入力されているか
		/// </summary>
		/// <returns>入力中ならならtrue,非入力中ならfalse</returns>
		inline bool IsInputStickL()
		{
			if ((fabsf(g_pad[0]->GetLStickXF()) >= FLT_EPSILON) || (fabsf(g_pad[0]->GetLStickYF()) >= FLT_EPSILON)) {
				return true;
			}

			return false;
		}
	}

	/*********************************************************************************/
	//player基底クラス。
	/*********************************************************************************/

	Player::Player()
	{
	}

	Player::~Player()
	{
		DeleteList();
		DeleteGO(m_stateMachine);
	}

	/// <summary>
	/// リスト削除。
	/// </summary>
	void Player::DeleteList()
	{
		//stateにインスタンスをいれて削除する。
		for (int i = 0; i < m_stateList.size(); ++i) {
			auto* state = m_stateList[i];
			delete state;
			state = nullptr;
		}
		m_stateList.clear();
	}

	bool Player::Start()
	{
		//player座標初期化
		m_playerPos = PLAYER_NEW_POSITION;
		//playerキャラコン初期化
		m_playerCharaCon.Init(25.0f, 75.0f, m_playerPos);
		//playerのHPをセット
		m_playerHP = MAX_PLAYER_HP;
		//ステートマシン生成
		m_stateMachine = NewGO<StateMachine>(0, "stateMachine");

		m_soundEffect = FindGO<SoundEffect>("soundEffect");

		AddList();
		LoadAssets();

		return true;
	}

	/// <summary>
	/// リスト追加。
	/// </summary>
	void Player::AddList()
	{
		//m_stateListを初期化。
		m_stateList.clear();
		//リストにインスタンスを追加していく。
		m_stateList.push_back(new PlayerIdle(this));
		m_stateList.push_back(new PlayerMove(this));
		m_stateList.push_back(new PlayerWeakAttack(this));
		m_stateList.push_back(new PlayerChargeAttack(this));
		m_stateList.push_back(new PlayerGuard(this));
		m_stateList.push_back(new PlayerHit(this));
		m_stateList.push_back(new PlayerDeath(this));
	}

	/// <summary>
	/// Assetsロード。
	/// </summary>
	void Player::LoadAssets()
	{
		//アニメーションロード。
		m_animationClips[enPlayerAnimClip_Idle].Load("Assets/animData/player/idle.tka");
		m_animationClips[enPlayerAnimClip_Idle].SetLoopFlag(true);
		m_animationClips[enPlayerAnimClip_Walk].Load("Assets/animData/player/walk.tka");
		m_animationClips[enPlayerAnimClip_Walk].SetLoopFlag(true);
		m_animationClips[enPlayerAnimClip_Run].Load("Assets/animData/player/run.tka");
		m_animationClips[enPlayerAnimClip_Run].SetLoopFlag(true);
		m_animationClips[enPlayerAnimClip_GuardStart].Load("Assets/animData/player/guardStart.tka");
		m_animationClips[enPlayerAnimClip_GuardStart].SetLoopFlag(false);
		m_animationClips[enPlayerAnimClip_WeakAttack].Load("Assets/animData/player/weakAttack.tka");
		m_animationClips[enPlayerAnimClip_WeakAttack].SetLoopFlag(false);
		m_animationClips[enPlayerAnimClip_ChargeAttack].Load("Assets/animData/player/chargeAttack.tka");
		m_animationClips[enPlayerAnimClip_ChargeAttack].SetLoopFlag(false);
		m_animationClips[enPLayerAnimClip_Charging].Load("Assets/animData/player/charging.tka");
		m_animationClips[enPLayerAnimClip_Charging].SetLoopFlag(false);
		m_animationClips[enPlayerAnimClip_Hit].Load("Assets/animData/player/hit.tka");
		m_animationClips[enPlayerAnimClip_Hit].SetLoopFlag(false);
		m_animationClips[enPlayerAnimClip_Death].Load("Assets/animData/player/death.tka");
		m_animationClips[enPlayerAnimClip_Death].SetLoopFlag(false);
		//モデルロード。
		m_playerModel.Init("Assets/ModelData/player/playerModel.tkm", m_animationClips, enPlayerAnimClip_Num, enModelUpAxisZ);
	}

	void Player::Update()
	{
		StateManage();
		BasicBehavior();
		DisplayCharge();
		PositionDraw();
	}

	/// <summary>
	/// playerの基本的な挙動。
	/// </summary>
	void Player::BasicBehavior()
	{
		//重力を発生させる。
		if (!m_playerCharaCon.IsOnGround()) {
			m_playerSpeed.y -= GRAVITY_AMOUNT;
		}

		//ポジションの更新。
		m_playerPos = m_playerCharaCon.Execute(m_playerSpeed, DELTA_TIME);
		m_playerModel.SetPosition(m_playerPos);
		//向きの更新。
		if (m_currentState == enPlayerWalk) {
			DirectionUpdate();
		}
		//回転の更新。
		m_playerRot.SetRotationYFromDirectionXZ(m_playerDir);
		m_playerModel.SetRotation(m_playerRot);
		//モデルの更新。
		m_playerModel.Update();
	}

	/// <summary>
	/// 向きを更新。
	/// </summary>
	void Player::DirectionUpdate()
	{
		//左スティックの入力量を取得。
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();

		//カメラの前方向と右方向のベクトルを取得。
		Vector3 forwardDir = g_camera3D->GetForward();
		Vector3 rightDir = g_camera3D->GetRight();

		//y方向を0にする。
		forwardDir.y = 0.0f;
		rightDir.y = 0.0f;

		//移動方向を計算。
		forwardDir *= stickL.y;
		rightDir *= stickL.x;

		//スティックが倒されているなら。
		if (IsInputStickL()) {
			//playerの向きを更新する。
			m_playerDir = forwardDir + rightDir;
			m_playerDir.Normalize();
		}
	}

	/// <summary>
	/// 被弾。
	/// </summary>
	/// <param name="reduce"></param>体力減少量。
	void Player::Hit(float reduce)
	{
		//被弾SE
		m_soundEffect->Play(enPlayerHitSE, false);
		//ガードができていないなら。
		if (m_guardFlag == true) {
			return;
		}

		//HPを減らす。
		m_playerHP -= reduce;

		//攻撃フラッグが立っていたら降ろす
		if (m_weakAtFlag) {
			m_weakAtFlag = false;
		}

		//HPを0以下にしない。
		if (m_playerHP < 0.0) {
			m_playerHP = 0.0f;
		}
		//HPの残量でステートを変える。
		if (m_playerHP > 0.0f) {
			m_hitFlag = true;
		}
		else {
			m_isDead = true;
		}
	}

	/// <summary>
	/// ステート管理。
	/// </summary>
	void Player::StateManage()
	{
		//ステートマシンのUpdate()は常に実行。
		m_stateMachine->Update();

		//ステートが変わったら。
		if (m_requestState != m_currentState) {
			m_stateList[m_currentState]->Exit();
			//現在のステートを切り替える。
			m_currentState = m_requestState;
			m_stateList[m_currentState]->Enter();
		}
		//現在のステートのUpdate()しか実行しない。
		m_stateList[m_currentState]->Update();
	}

	/// <summary>
	/// チャージ量表示（仮）。
	/// </summary>
	void Player::DisplayCharge()
	{
		m_chargeRender.SetScale(1.2);
		m_chargeRender.SetPosition({ 425.0f,475.0f,0.0f });
		m_chargeRender.SetColor(g_vec4Red);

		swprintf_s(m_chargeText, 100, L"チャージ %.1f", float(m_charge));
		m_chargeRender.SetText(m_chargeText);
	}

	/// <summary>
	/// プレイヤーの位置を描画（仮）。
	/// </summary>
	void Player::PositionDraw()
	{
		m_posRender.SetScale(1.2);
		m_posRender.SetPosition({ -900.0f,425.0f,0.0f });
		m_posRender.SetColor(g_vec4Red);

		swprintf_s(m_posText, 100, L"Pos %.1f %.1f %.1f", float(m_playerPos.x), float(m_playerPos.y), float(m_playerPos.z));
		m_posRender.SetText(m_posText);
	}

	void Player::Render(RenderContext& rc)
	{
		m_playerModel.Draw(rc);
		m_chargeRender.Draw(rc);
		m_posRender.Draw(rc);
	}

	/// <summary>
	/// プレイヤーの最大HPを取得。
	/// </summary>
	/// <returns>最大HP</returns>
	float Player::GetPlayerMAXHP()
	{
		return MAX_PLAYER_HP;
	}

	/*********************************************************************************/
	//ステートマシン（仮）。
	/*********************************************************************************/
	StateMachine::StateMachine()
	{
	}

	StateMachine::~StateMachine()
	{
	}

	bool StateMachine::Start()
	{
		m_player = FindGO<Player>("player");

		return true;
	}

	void StateMachine::Update()
	{
		//イベントカメラ中なら実行しない
		m_gameCamera = FindGO<GameCamera>("gameCamera");
		if (m_gameCamera->GetCameraState() == EnCameraVar::enBearContact) {
			m_player->m_requestState = enPlayerIdle;
			return;
		}

		StateManage();
	}

	/// <summary>
	/// ステート遷移（仮）。
	/// </summary>
	void StateMachine::StateManage()
	{
		//死亡。
		//・死亡していたら。
		if (m_player->m_isDead) {
			m_player->m_requestState = enPlayerDeath;
			return;
		}

		//被弾。
		//・被弾の硬直中なら。
		if (m_player->m_hitFlag) {
			m_player->m_requestState = enPlayerHit;
			return;
		}

		//弱攻撃。
		//Yボタンが押されたら。
		if ((g_pad[0]->IsTrigger(enButtonY)) || (m_player->m_weakAtFlag)) {
			m_player->m_requestState = enPlayerWeakAttack;
			return;
		}

		//溜め攻撃。
		//右スティックでチャージ中なら。
		if (IsInputStickR() || m_player->m_chargeAtFlag) {
			m_player->m_requestState = enPlayerChargeAttack;
			return;
		}

		//ガード。
		//Xボタンが入力されているなら。
		if (g_pad[0]->IsPress(enButtonX)) {
			m_player->m_requestState = enPlayerGuard;
			return;
		}

		//移動。
		//・スティックが入力されている。
		if (IsInputStickL()) {
			m_player->m_requestState = enPlayerWalk;
			return;
		}

		//待機。
		//・ボタンとスティックが何も入力されていない。
		if ((!g_pad[0]->IsPressAnyKey()) && (!IsInputStickL()) && (!IsInputStickR())) {
			m_player->m_requestState = enPlayerIdle;
		}
	}

	/*********************************************************************************/
	//待機用クラス。
	/*********************************************************************************/

	PlayerIdle::~PlayerIdle()
	{

	}

	void PlayerIdle::Enter()
	{
		m_soundEffect = FindGO<SoundEffect>("soundEffect");
		//待機アニメーションを再生。
		m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_Idle, ANIM_INTERPOLATE_TIME);
	}

	void PlayerIdle::Update()
	{
		idle();
	}

	/// <summary>
	/// 待機。
	/// </summary>
	void PlayerIdle::idle()
	{

	}

	void PlayerIdle::Exit()
	{

	}



	/*********************************************************************************/
	//移動用クラス。
	/*********************************************************************************/

	PlayerMove::~PlayerMove()
	{

	}

	void PlayerMove::Enter()
	{
		m_soundEffect = FindGO<SoundEffect>("soundEffect");

		m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_Run, ANIM_INTERPOLATE_TIME);
	}

	void PlayerMove::Update()
	{
		if (m_player->m_playerCharaCon.IsOnGround()) {
			Dash();
		}
	}

	/// <summary>
	/// ダッシュ。
	/// </summary>
	void PlayerMove::Dash()
	{
		m_player->m_playerSpeed.x = m_player->m_playerDir.x * MOVE_AMOUNT;
		m_player->m_playerSpeed.z = m_player->m_playerDir.z * MOVE_AMOUNT;
	}

	void PlayerMove::Exit()
	{
		m_player->m_playerSpeed = Vector3::Zero;
	}

	/*********************************************************************************/
	//弱攻撃用クラス。
	/*********************************************************************************/

	PlayerWeakAttack::~PlayerWeakAttack()
	{

	}

	void PlayerWeakAttack::Enter()
	{
		m_soundEffect	= FindGO<SoundEffect>("soundEffect");
		m_enemyManager	= FindGO<EnemyManager>("enemyManager");

		//弱攻撃のフラッグを立てる。
		m_player->m_weakAtFlag = true;
		//攻撃力を設定。
		m_player->m_attackPower = WEAK_ATTACK_POWER;
		//弱攻撃。
		WeakAttack();
		//弱攻撃アニメーションを再生。
		m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_WeakAttack, ANIM_INTERPOLATE_TIME);
		//サウンドソース
		m_soundEffect->Play(enPlayerAttackSE, false);
	}

	/// <summary>
	/// 弱攻撃。
	/// </summary>
	void PlayerWeakAttack::WeakAttack()
	{
		//敵をロックオン
		LockOnEnemy();
		//コリジョン生成。
		MakeCollision();
		//コリジョン削除。
		DeleteGO(m_player->m_collision);
	}

	/// <summary>
	/// 近くの敵をロックオン
	/// </summary>
	void PlayerWeakAttack::LockOnEnemy()
	{
		//最寄りの敵への方向を計算
		Vector3 nearEnemyPos = m_enemyManager->CalcToNearestEnemyVec(m_player->m_playerPos);
		//ゼロベクトルが帰っているなら実行しない（仮）
		if (nearEnemyPos.Length() <= 5.0f) {
			return;
		}
		Vector3 toNearEnemy = nearEnemyPos - m_player->m_playerPos;
		toNearEnemy.y = 0.0f;

		//敵との距離が離れすぎていたら実行しない
		if (toNearEnemy.Length() >= SUCTION_CONDITION_DIS) {
			return;
		}
		toNearEnemy.Normalize();
		//プレイヤーの向きを変える
		m_player->m_playerDir = toNearEnemy;

		//敵への吸いつきを行うかを判断
		//敵への方向の一定距離に吸いつきの目標位置を設定
		Vector3 targetPos = nearEnemyPos + (toNearEnemy * -1 * SUCTION_TARGET_POS_DIS);
		targetPos.y = 0.0f;
		//目標位置への方向と敵への方向が一緒なら
		//（目標位置よりも近い場所にいるなら方向は違うはず）
		m_toSuctionTarget = targetPos - m_player->m_playerPos;
		m_toSuctionTarget.y = 0.0f;
		Vector3 toSuctionTargetDir = m_toSuctionTarget;
		toSuctionTargetDir.Normalize();
		float dirGap = Dot(toNearEnemy, toSuctionTargetDir);
		if (dirGap >= 0.9f) {
			//吸いつき処理を行う
			m_isSuctionDecide = true;
		}
	}

	void PlayerWeakAttack::Update()
	{
		if (m_isSuctionDecide) {
			SuctionEnemy();
		}
		
		ChangeState();

		//弱攻撃アニメーション再生。
		m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_WeakAttack, ANIM_INTERPOLATE_TIME);
	}

	/// <summary>
	/// 敵に吸いつくようにプレイヤーを動かす
	/// </summary>
	void PlayerWeakAttack::SuctionEnemy()
	{
		//吸いつきの経過時間を計算
		m_suctionElapsedTime += g_gameTime->GetFrameDeltaTime();

		//1フレームで進む距離を計算
		Vector3 moveAmount = m_toSuctionTarget / SUCTION_TIME * DELTA_TIME;

		m_player->m_playerPos += moveAmount;
		m_player->m_playerCharaCon.SetPosition(m_player->m_playerPos);
	}

	/// <summary>
	/// コリジョン作成。
	/// </summary>
	void PlayerWeakAttack::MakeCollision()
	{
		//コリジョンオブジェクトを作成
		m_player->m_collision = NewGO<CollisionObject>(0, "weakAttack");
		Vector3 collisionPosition = m_player->m_playerPos;
		//座標をプレイヤーの少し前に設定
		collisionPosition += m_player->m_playerDir * WEAK_COLLISION_DIS;
		//球状のコリジョンを作成
		m_player->m_collision->CreateSphere(collisionPosition,
			Quaternion::Identity,
			WEAK_COLLISION_SIZE);
	}

	/// <summary>
	//ステート切り替え。
	/// </summary>
	void PlayerWeakAttack::ChangeState()
	{
		//アニメーションが再生し終わったらステート変更。
		if (!m_player->m_playerModel.IsPlayAnimation()) {
			m_player->m_weakAtFlag = false;
		}
	}

	void PlayerWeakAttack::Exit()
	{
		//攻撃力をリセット。
		m_player->m_attackPower = 0.0f;
		//吸いつきフラッグをリセット
		m_isSuctionDecide = false;
	}

	/*********************************************************************************/
	//溜め攻撃用クラス。
	/*********************************************************************************/

	PlayerChargeAttack::~PlayerChargeAttack()
	{

	}

	void PlayerChargeAttack::Enter()
	{
		m_soundEffect = FindGO<SoundEffect>("soundEffect");

		//溜め攻撃のフラッグを立てる。
		m_player->m_chargeAtFlag = true;
		//チャージ中。
		m_isCharging = true;
		//まだステートを切り替えていない。
		m_isStateChange = false;
		//チャージの初期値。
		m_player->m_charge = NEW_CHARGE;
	}

	void PlayerChargeAttack::Update()
	{
		Charging();
		ChangeState();
	}

	/// <summary>
	/// チャージを溜める。
	/// </summary>
	void PlayerChargeAttack::Charging()
	{
		//チャージ中ではないなら実行しない。
		if (!m_isCharging) {
			return;
		}


		//チャージ中アニメーション再生。
		m_player->m_playerModel.PlayAnimation(enPLayerAnimClip_Charging, ANIM_INTERPOLATE_TIME);

		Vector3 RStick = Vector3::Zero;				//Rスティック入力量。
		float movePower = 0.0f;						//パワー（入力変動量）。

		//Rスティックの入力量をとる。
		RStick.x = g_pad[0]->GetRStickXF();
		RStick.y = g_pad[0]->GetRStickYF();
		//チャージ量にたすパワーを計算する（スティックの変更後、変更前の内積）。
		movePower = Dot(RStick, m_RStickOld);
		//スティックが動いていないなら、パワーを0にする。
		if ((RStick.x == m_RStickOld.x) && (RStick.y == m_RStickOld.y)) {
			movePower = 0.0f;
		}
		//movePowerを絶対値にする
		movePower = fabsf(movePower);
		//movePowerを増加させる
		movePower *= CHARGE_ADD_VALUE;
		//パワーをチャージに足す。
		m_player->m_charge += movePower;

		//チャージが増加しているなら、溜め攻撃1アニメーションを再生。
		if (m_player->m_charge >= 30.0f)
		{
			m_soundEffect->Play(enPlayerCharge1SE, false);
		}
		if (m_player->m_charge >= 60.0f)
		{
			m_soundEffect->Play(enPlayerCharge2SE, false);
		}
		if (m_player->m_charge >= 100.0f)
		{
			m_soundEffect->Play(enPlayerCharge3SE, false);
		}

		//チャージを減少させる。
		m_player->m_charge -= CHARGE_DECREASE;

		//チャージを0以下にさせない。
		if (m_player->m_charge < 0.0f) {
			m_player->m_charge = 0.0f;
		}

		//チャージのMAX値。
		if (m_player->m_charge >= CHARGE_MAX) {
			m_player->m_charge = CHARGE_MAX;
		}

		//スティック入力量を更新。
		m_RStickOld = RStick;

		//スティックの入力をやめていたら。
		if (!IsInputStickR()) {
			//チャージが少なかったら弱攻撃に。
			if (m_player->m_charge <= CHANGE_WEAK) {
				m_player->m_weakAtFlag = true;
				m_player->m_chargeAtFlag = false;
				return;
			}
			if (!m_isStateChange) {
				ChargeAttack();
				//溜め攻撃アニメーションを再生。
				m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_ChargeAttack, ANIM_INTERPOLATE_TIME);
				//溜め攻撃をした。
				m_isStateChange = true;
			}

			m_isCharging = false;
		}
	}

	/// <summary>
	/// 溜め攻撃実行。
	/// </summary>
	void PlayerChargeAttack::ChargeAttack()
	{
		SnakeEnemy* snakeEnemy = FindGO<SnakeEnemy>("enemy");
		//コリジョン生成。
		MakeCollision();
		//コリジョン削除。
		DeleteGO(m_player->m_collision);
	}

	/// <summary>
	/// コリジョンを作成。
	/// </summary>
	void PlayerChargeAttack::MakeCollision()
	{
		//コリジョンオブジェクトを作成
		m_player->m_collision = NewGO<CollisionObject>(0, "chargeAttack");
		Vector3 collisionPosition = m_player->m_playerPos;
		m_collisionSize = CHARGE_COLLISION_SIZE * m_player->m_charge + COLLISION_SIZE_LOWEST;
		//球状のコリジョンを作成
		m_player->m_collision->CreateSphere(collisionPosition,
			Quaternion::Identity,
			m_collisionSize);

		//攻撃力を設定。
		m_player->m_attackPower = m_player->m_charge * CHARGE_POWER;
	}

	/// <summary>
	/// ステートを変える条件。
	/// </summary>
	void PlayerChargeAttack::ChangeState()
	{
		//チャージ中の場合。
		//チャージが0になったら。
		if ((m_isCharging) && (m_player->m_charge <= 0.0f)) {
			m_player->m_chargeAtFlag = false;
		}
		//攻撃実行中の場合。
		//アニメーションが再生し終わったら。
		if ((!m_isCharging) && (!m_player->m_playerModel.IsPlayAnimation())) {
			m_player->m_chargeAtFlag = false;
		}
	}

	void PlayerChargeAttack::Exit()
	{
		//チャージをリセット。
		m_player->m_charge = 0.0f;
	}

	/*********************************************************************************/
	//ガード用クラス。
	/*********************************************************************************/

	PlayerGuard::~PlayerGuard()
	{

	}

	void PlayerGuard::Enter()
	{
		m_soundEffect = FindGO<SoundEffect>("soundEffect");

		//ガードアニメーションを再生。
		m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_GuardStart, ANIM_INTERPOLATE_TIME);
		//ガードフラッグを立てる。
		m_player->m_guardFlag = true;
	}

	void PlayerGuard::Update()
	{
		GuardDirection();
	}

	/// <summary>
	/// ガード方向を加味。
	/// </summary>
	void PlayerGuard::GuardDirection()
	{
		//Vector3 toEnemyDirection = m_player->m_playerPos - enemyPos;
		//toEnemyDirection.Normalize();
		//float guard = Dot(m_player->m_playerDir, toEnemyDirection);

		//if(guard <= GUARD_TOLERANCE){
		//	m_player->m_guardFlag = true;
		//}
		//else {
		//	m_player->m_guardFlag = false;
		//}
	}

	void PlayerGuard::Exit()
	{
		//ガードフラッグを下ろす。
		m_player->m_guardFlag = false;
	}

	/*********************************************************************************/
	//被弾用クラス。
	/*********************************************************************************/

	PlayerHit::~PlayerHit()
	{

	}

	void PlayerHit::Enter()
	{
		m_soundEffect = FindGO<SoundEffect>("soundEffect");

		//被弾アニメーションを再生。
		m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_Hit, ANIM_INTERPOLATE_TIME);
		//被弾のSE
		m_soundEffect->Play(enPlayerHitSE, false);
	}

	void PlayerHit::Update()
	{
		ChangeState();
	}

	/// <summary>
	/// 硬直時間を計算。
	/// </summary>
	void PlayerHit::ChangeState()
	{
		//アニメーションが再生し終わったらステート変更。
		if (!m_player->m_playerModel.IsPlayAnimation()) {
			m_player->m_hitFlag = false;
		}
	}

	void PlayerHit::Exit()
	{

	}
	/*********************************************************************************/
	//死亡用クラス。
	/*********************************************************************************/

	PlayerDeath::~PlayerDeath()
	{

	}

	void PlayerDeath::Enter()
	{
		m_soundEffect = FindGO<SoundEffect>("soundEffect");

		//死亡アニメーションを再生。
		m_player->m_playerModel.PlayAnimation(enPlayerAnimClip_Death, ANIM_INTERPOLATE_TIME);
		//死亡SE
		m_soundEffect->Play(enPlayerDeathSE, false);
	}

	void PlayerDeath::Update()
	{
	}

	void PlayerDeath::Exit()
	{

	}
}