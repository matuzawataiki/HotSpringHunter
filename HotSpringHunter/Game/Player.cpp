#include "stdafx.h"
#include "Player.h"
#include "Towel.h"
#include "Bucket.h"

#include "SnakeEnemy.h"

namespace {
	const float MOVE_AMOUNT = 120.0f;			//移動：移動量。
	const float GRAVITY_AMOUNT = 10.0f;			//移動：重力量。

	const float JUMP_AMOUNT = 700.0f;			//ジャンプ：ジャンプ力。

	const float DASH_AMOUNT = 4.0f;				//ダッシュ：ダッシュ倍率。

	const float WEAK_COLLISION_DIS = 100.0f;	//弱攻撃：コリジョン位置。
	const float WEAK_COLLISION_SIZE = 150.0f;	//弱攻撃：コリジョンサイズ。

	const float CHARGE_DECREASE = 1.3f;			//溜め攻撃：チャージ減少量。
	const float CHARGE_COLLISION_SIZE = 2.0f;	//溜め攻撃：コリジョン大きさの倍率。

	const Vector3 PLAYER_NEW_POSITION = Vector3{ 0.0f,300.0f,0.0f };	//player初期値。
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
}

/// <summary>
/// リスト削除。
/// </summary>
void Player::DeleteList()
{
	//stateにインスタンスをいれて、削除する。
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
	m_playerPosition = PLAYER_NEW_POSITION;
	//playerキャラコン初期化
	m_playerCharaCon.Init(25.0f, 75.0f, m_playerPosition);

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
	m_stateList.push_back(new PlayerMove);
	m_stateList.push_back(new PlayerJump);
	m_stateList.push_back(new PlayerDash);
	m_stateList.push_back(new PlayerWeakAttack);
	m_stateList.push_back(new PlayerChargeAttack);
	m_stateList.push_back(new PlayerGuard);
	m_stateList.push_back(new PlayerHealth);
	m_stateList.push_back(new PlayerHit);
	m_stateList.push_back(new PlayerDeath);
}

/// <summary>
/// Assetsロード。
/// </summary>
void Player::LoadAssets()
{
	//アニメーションロード。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/player/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/player/jump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	m_animationClips[enAnimationClip_GuardStart].Load("Assets/animData/player/guardStart.tka");
	m_animationClips[enAnimationClip_GuardStart].SetLoopFlag(false);
	m_animationClips[enAnimationClip_GuardEnd].Load("Assets/animData/player/guardEnd.tka");
	m_animationClips[enAnimationClip_GuardEnd].SetLoopFlag(false);
	m_animationClips[enAnimationClip_WeakAttack].Load("Assets/animData/player/weakAttack.tka");
	m_animationClips[enAnimationClip_WeakAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ChargeAttack].Load("Assets/animData/player/chargeAttack.tka");
	m_animationClips[enAnimationClip_ChargeAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Charging].Load("Assets/animData/player/charging.tka");
	m_animationClips[enAnimationClip_Charging].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Hit].Load("Assets/animData/player/hit.tka");
	m_animationClips[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/player/death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	//モデルロード。
	m_playerModel.Init("Assets/ModelData/player/playerModel.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);
}

void Player::Update()
{
	StateManage();	

	m_playerModel.Update();
}

/// <summary>
/// ステート管理。
/// </summary>
void Player::StateManage()
{
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

void Player::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
}

/*********************************************************************************/
//移動用クラス。
/*********************************************************************************/

PlayerMove::PlayerMove(Player* player)
	: IState(player)
{

}

PlayerMove::~PlayerMove()
{

}

void PlayerMove::Enter()
{

}

void PlayerMove::Update()
{
	Move();
	Rotation();
}

/// <summary>
/// 移動。
/// </summary>
void PlayerMove::Move()
{
	//地面についていたら。
	if (m_player->m_playerCharaCon.IsOnGround() == false) {
		return;
	}
	if (g_pad[0]->GetLStickXF() < 0.01f) {
		if (g_pad[0]->GetLStickYF() < 0.01f) {
			return;
		}
	}

	//移動速度を0.0fにする。
	m_player->m_playerSpeed.x = 0.0f;
	m_player->m_playerSpeed.z = 0.0f;

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

	//前方向ベクトルを正規化。
	//カメラの上下方向でキャラの移動速度が変わらないようにする。
	forwardDir.Normalize();

	//移動方向を計算。
	forwardDir *= stickL.y;
	rightDir *= stickL.x;

	//playerの向きを更新。
	GetDirection(forwardDir, rightDir);

	//移動速度を計算。
	//移動速度 = 歩行速度 * ダッシュ状態 * ガード状態。
	MoveAdjust();
	forwardDir *= MOVE_AMOUNT;
	rightDir *= MOVE_AMOUNT;

	//移動速度に加算。
	m_player->m_playerSpeed += forwardDir + rightDir;

	//重力をなくす。
	m_player->m_playerSpeed.y = 0.0f;

	//ジャンプ。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_player->m_playerSpeed.y = JUMP_AMOUNT;
	}
	//地面についていなかったら。
	else {
		//重力を発生させる。
		m_player->m_playerSpeed.y -= GRAVITY_AMOUNT;
	}

	m_player->m_playerModel.PlayAnimation(enAnimationClip_Walk);

	m_player->m_playerPosition = m_player->m_playerCharaCon.Execute(m_player->m_playerSpeed, 1.0f / 60.0f);
	m_player->m_playerModel.SetPosition(m_player->m_playerPosition);
}

/// <summary>
/// playerの向きを更新。
/// </summary>
/// <param name="forward"></param>縦の移動量。
/// <param name="right"></param>横の移動量。
void PlayerMove::GetDirection(Vector3 forward, Vector3 right)
{
	//スティックが倒されているなら。
	if (fabsf(forward.x) >= 0.01f || fabsf(forward.z) >= 0.01f ||
		fabsf(right.x) >= 0.01f || fabsf(right.z) >= 0.01f) {
		//playerの向きを更新する。
		m_player->m_playerDirection = forward + right;
		m_player->m_playerDirection.Normalize();
	}
}

/// <summary>
/// 移動速度を加味。
/// </summary>
void PlayerMove::MoveAdjust()
{
	//ガード中。
	if (g_pad[0]->IsPress(enButtonX)) {
		m_player->m_guardState = 0.0f;
	}
	else {
		m_player->m_guardState = 1.0f;
	}

	//ダッシュ。
	if (g_pad[0]->IsPress(enButtonB)) {
		m_dashState = DASH_AMOUNT;
	}
	else {
		m_dashState = 1.0f;
	}
}

/// <summary>
/// 回転を更新。
/// </summary>
void PlayerMove::Rotation()
{
	m_player->m_playerRotation.SetRotationYFromDirectionXZ(m_player->m_playerDirection);
	m_player->m_playerModel.SetRotation(m_player->m_playerRotation);
}

void PlayerMove::Exit()
{

}

/*********************************************************************************/
//ダッシュ用クラス。
/*********************************************************************************/

PlayerDash::PlayerDash(Player* player)
	: IState(player)
{

}

PlayerDash::~PlayerDash()
{

}

void PlayerDash::Enter()
{

}

void PlayerDash::Update()
{
	Dash();
}

/// <summary>
/// ダッシュ。
/// </summary>
void PlayerDash::Dash()
{
	if (g_pad[0]->IsPress(enButtonB)) {
		m_player->m_dashState = DASH_AMOUNT;
	}
	else {
		m_player->m_dashState = 1.0f;
	}
}

void PlayerDash::Exit()
{

}

/*********************************************************************************/
//ジャンプ用クラス。
/*********************************************************************************/
PlayerJump::PlayerJump(Player* player)
	: IState(player)
{

}

PlayerJump::~PlayerJump()
{

}

void PlayerJump::Enter()
{
}

void PlayerJump::Update()
{

}

/// <summary>
/// ジャンプ。
/// </summary>
void PlayerJump::Jump()
{

}

void PlayerJump::Exit()
{

}

/*********************************************************************************/
//弱攻撃用クラス。
/*********************************************************************************/
PlayerWeakAttack::PlayerWeakAttack(Player* player)
	: IState(player)
{
	
}

PlayerWeakAttack::~PlayerWeakAttack()
{

}

void PlayerWeakAttack::Enter()
{
}

void PlayerWeakAttack::Update()
{
	WeakAttack();
}

/// <summary>
/// 弱攻撃。
/// </summary>
void PlayerWeakAttack::WeakAttack()
{
	if (g_pad[0]->IsTrigger(enButtonY)) {
		m_player->m_animationState = enWeakAttack;
		SnakeEnemy* snakeEnemy = FindGO<SnakeEnemy>("snakeEnemy");
		if (snakeEnemy == nullptr) {
			return;
		}
		MakeCollision();
		if (m_player->m_collision->IsHit(snakeEnemy->m_characterController)) {
			snakeEnemy->Hit();
		}
		DeleteGO(m_player->m_collision);
	}
}

/// <summary>
/// コリジョン作成。
/// </summary>
void PlayerWeakAttack::MakeCollision()
{
	//コリジョンオブジェクトを作成
	m_player->m_collision = NewGO<CollisionObject>(0, "weakAttack");
	Vector3 collisionPosition = m_player->m_playerPosition;
	//座標をプレイヤーの少し前に設定
	collisionPosition += m_player->m_playerDirection * WEAK_COLLISION_DIS;
	//球状のコリジョンを作成
	m_player->m_collision->CreateSphere(collisionPosition,
		Quaternion::Identity,
		WEAK_COLLISION_SIZE);
}
void PlayerWeakAttack::Exit()
{

}

/*********************************************************************************/
//溜め攻撃用クラス。
/*********************************************************************************/

PlayerChargeAttack::PlayerChargeAttack(Player* player)
	: IState(player)
{

}

PlayerChargeAttack::~PlayerChargeAttack()
{

}

void PlayerChargeAttack::Enter()
{

}

void PlayerChargeAttack::Update()
{
	StickCharge();
	ChargeAttack();
	DisplayCharge();
}

/// <summary>
/// チャージを溜める。
/// </summary>
void PlayerChargeAttack::StickCharge()
{
	Vector3 RStick = Vector3::Zero;				//Rスティック入力量。	
	float movePower = 0.0f;						//パワー（入力変動量）。

	//Rスティックの入力があったら。
	if ((fabsf(g_pad[0]->GetRStickXF()) >= 0.001f) || (fabsf(g_pad[0]->GetRStickYF()) >= 0.001f)) {
		//アニメーションステート切り替え。
		//m_player->m_animationState = m_player->enCharging;

		//Rスティックの入力量をとる。		
		RStick.x = g_pad[0]->GetRStickXF();
		RStick.y = g_pad[0]->GetRStickYF();
		//チャージ量にたすパワーを計算する（スティックの変更後、変更前の内積）。
		movePower = Dot(RStick, m_RStickOld);
		//スティックが動いていないなら、パワーを0にする。
		if ((RStick.x == m_RStickOld.x) && (RStick.y == m_RStickOld.y)) {
			movePower = 0.0f;
		}
		//パワーの値がマイナスならプラスにする。
		if (movePower < 0.0f) {
			movePower *= -1.0f;
		}
		//パワーをチャージに足す。
		m_charge += movePower;
	}
	//パワーリセット。
	movePower = 0.0f;

	//チャージを減少させる。
	m_charge -= CHARGE_DECREASE;

	//チャージを0以下にさせない。
	if (m_charge <= 0.0f) {
		m_charge = 0.0f;
	}

	if (m_charge >= 100.0f) {
		m_charge = 100.0f;
	}

	//スティック入力量を更新。
	m_RStickOld = RStick;

}

/// <summary>
/// 溜め攻撃。
/// </summary>
void PlayerChargeAttack::ChargeAttack()
{
	//チャージが20以上でスティックの入力がなかったら。
	if ((fabsf(g_pad[0]->GetRStickXF()) <= 0.001f) || (fabsf(g_pad[0]->GetRStickYF()) <= 0.001f)) {
		if (m_charge >= 20.0f) {
			//playerのアニメーションステート切り替え。
			m_player->m_animationState = enChargeAttack;
			SnakeEnemy* snakeEnemy = FindGO<SnakeEnemy>("enemy");
			/*if (snakeEnemy == nullptr) {
				return;
			}*/
			MakeCollision();
			//if (m_collision->IsHit(snakeEnemy->m_characterController)) {
			//	//snakeEnemy->Hit(m_charge * 2.0f);
			//}
			DeleteGO(m_player->m_collision);
		}
	}
}

/// <summary>
/// コリジョンを作成。
/// </summary>
void PlayerChargeAttack::MakeCollision()
{
	//コリジョンオブジェクトを作成
	m_player->m_collision = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_player->m_playerPosition;
	m_collisionSize = CHARGE_COLLISION_SIZE * m_charge + 100.0f;
	//球状のコリジョンを作成
	m_player->m_collision->CreateSphere(collisionPosition,
		Quaternion::Identity,
		m_collisionSize);
	//名前をつける
	m_player->m_collision->SetName("chargeAttack");
	//チャージリセット。
	m_charge = 0.0f;
}

/// <summary>
/// チャージ量表示（仮）。
/// </summary>
void PlayerChargeAttack::DisplayCharge()
{
	m_chargeRender.SetScale(1.2);
	m_chargeRender.SetPosition({ 425.0f,475.0f,0.0f });
	m_chargeRender.SetColor(g_vec4Black);

	swprintf_s(m_chargeText, 100, L"チャージ %.1f", float(m_charge));
	m_chargeRender.SetText(m_chargeText);
}

void PlayerChargeAttack::Exit()
{

}

/*********************************************************************************/
//ガード用クラス。
/*********************************************************************************/

PlayerGuard::PlayerGuard(Player* player)
	: IState(player)
{

}

PlayerGuard::~PlayerGuard()
{

}

void PlayerGuard::Exit()
{
}

void PlayerGuard::Update()
{
	if (g_pad[0]->IsPress(enButtonX)) {
		GuardDirection();
	}
}

/// <summary>
/// ガード方向を加味。
/// </summary>
void PlayerGuard::GuardDirection()
{
	/*
	Vector3 enemyPos = m_enemy->GetPosition();
	Vector3 playerPos = m_player->GetPosition();
	Vector3 toEnemyDirection = playerPos - enemyPos;
	toEnemyDirection.Normalize();
	Vector3 playerDirection = m_player->GetPlayerDirection();
	Vector3 guard = Dot(playerDirection,toEnemyDirection)

	if(guard <= GUARD_TOLERANCE){
		m_guardFlag = true;
	}
	else {
		m_guardFlag = false;
	}
	*/
	m_player->m_animationState = enGuardStart;
}

void PlayerGuard::Exit()
{

}

/*********************************************************************************/
//体力管理用クラス。
/*********************************************************************************/

PlayerHealth::PlayerHealth(Player* player)
	: IState(player)
{

}

PlayerHealth::~PlayerHealth()
{

}

void PlayerHealth::Exit()
{
}

void PlayerHealth::Update()
{
	TakeDamage();
	DisplayHP();
}

/// <summary>
/// 攻撃を受ける。
/// </summary>
void PlayerHealth::TakeDamage()
{
	//まだ生きているとき。
	if (m_player->m_playerHP >= 1.0f) {
		m_player->m_animationState = enHit;
	}
	//HPが0になったとき。
	else {
	}
}

/// <summary>
/// HP表示（仮）。
/// </summary>
void PlayerHealth::DisplayHP()
{
	m_HPRender.SetScale(1.2);
	m_HPRender.SetPosition({ 800.0f,-400.0f, 0.0f });
	m_HPRender.SetColor(g_vec4Black);

	swprintf_s(m_HPText, 100, L"HP %f", float(m_playerHP));
	m_HPRender.SetText(m_HPText);
}

void PlayerHealth::Exit()
{
}
/*********************************************************************************/
//被弾用クラス。
/*********************************************************************************/

PlayerHit::PlayerHit(Player* player)
	: IState(player)
{

}

PlayerHit::~PlayerHit()
{

}

void PlayerHit::Exit()
{
	
}

void PlayerHit::Update()
{
	
}

/// <summary>
/// 被弾。
/// </summary>
/// <param name="reduce"></param>体力減少量。
void PlayerHit::Hit(float reduce)
{
	//ガードができていないなら。
	if (m_player->m_guardFlag == true) {
		return;
	}

	//HPを減らす。
	m_player->m_playerHP -= reduce;
}

void PlayerHit::Exit()
{


}
/*********************************************************************************/
//死亡用クラス。
/*********************************************************************************/

PlayerDeath::PlayerDeath(Player* player)
	: IState(player)
{

}

PlayerDeath::~PlayerDeath()
{

}

void PlayerDeath::Enter()
{
}

void PlayerDeath::Update()
{
	DeathJudge();
}

/// <summary>
/// 死亡判定。
/// </summary>
void PlayerDeath::DeathJudge()
{
	if (m_player->m_playerHP <= 0.0f) {
		m_player->m_animationState = enDeath;
	}
}

void PlayerDeath::Exit()
{

}