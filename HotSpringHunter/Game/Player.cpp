#include "stdafx.h"
#include "Player.h"
#include "PlayerHealth.h"
#include "PlayerAttack.h"
#include "PlayerGuard.h"
#include "PlayerChargeAttack.h"
#include "Towel.h"
#include "Bucket.h"

namespace {
	const float MOVE_AMOUNT = 120.0f;
	const float JUMP_AMOUNT = 700.0f;
	const float DASH_AMOUNT = 4.0f;
	const float GRAVITY_AMOUNT = 10.0f;

	const Vector3 PLAYER_NEW_POSITION = Vector3{ 0.0f,300.0f,0.0f };
}

Player::Player()
{
}

Player::~Player()
{
	DeleteGO(m_playerAttack);
	DeleteGO(m_playerGuard);
	DeleteGO(m_playerChaAt);
	DeleteGO(m_playerHealth);
}

bool Player::Start()
{
	//player座標初期化
	m_playerPosition = PLAYER_NEW_POSITION;
	//playerキャラコン初期化
	m_playerCharaCon.Init(25.0f, 75.0f, m_playerPosition);

	LoadModel();
	GenerateMinions();

	return true;
}

/// <summary>
/// Assetsロード。
/// </summary>
void Player::LoadModel()
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
	m_playerModelRender.Init("Assets/ModelData/player/playerModel.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);
}

/// <summary>
/// 子クラスNewGO。
/// </summary>
void Player::GenerateMinions()
{
	m_playerHealth = NewGO<PlayerHealth>(0, "playerHealth");
	m_playerAttack = NewGO<PlayerAttack>(0, "playerAttack");
	m_playerGuard = NewGO<PlayerGuard>(0, "playerGuard");
	m_playerChaAt = NewGO<PlayerChargeAttack>(0, "playerChargeAttack");

	//NewGO<Towel>(0, "towel");
	//NewGO<Bucket>(0, "bucket");
}

void Player::Update()
{
	Move();
	Rotation();
	StateManage();
	AnimationManage();

	m_playerModelRender.Update();
}

/// <summary>
/// playerの移動。
/// </summary>
void Player::Move()
{
	//地面についていたら。
	if (m_playerCharaCon.IsOnGround() == true) {

		//移動速度を0.0fにする。
		m_playerSpeed.x = 0.0f;
		m_playerSpeed.z = 0.0f;

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

		//playerの向きを取得。
		GetDirection(forwardDir, rightDir);

		//移動速度を計算。
		//移動速度 = 歩行速度 * ダッシュ状態 * ガード状態。
		MoveAdjust();
		forwardDir *= MOVE_AMOUNT * m_runState * m_guardState;
		rightDir *= MOVE_AMOUNT * m_runState * m_guardState;

		//移動速度に加算。
		m_playerSpeed += forwardDir + rightDir;		

		//重力をなくす。
		m_playerSpeed.y = 0.0f;

		//ジャンプ。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_playerSpeed.y = JUMP_AMOUNT;
		}
	}
	//地面についていなかったら。
	else {
		//重力を発生させる。
		m_playerSpeed.y -= GRAVITY_AMOUNT;
	}
	
	m_playerPosition = m_playerCharaCon.Execute(m_playerSpeed, 1.0f / 60.0f);
	m_playerModelRender.SetPosition(m_playerPosition);
}

void Player::MoveAdjust()
{
	//ガード中。
	if (g_pad[0]->IsPress(enButtonX)) {
		m_guardState = 0.0f;
	}
	else {
		m_guardState = 1.0f;
	}

	//ダッシュ。
	if (g_pad[0]->IsPress(enButtonB)) {
		m_runState = DASH_AMOUNT;
	}
	else{
		m_runState = 1.0f;
	}
}

/// <summary>
/// playerの向きを更新。
/// </summary>
/// <param name="forward"></param>スティックの縦方向。
/// <param name="right"></param>スティックの横方向。
void Player::GetDirection(Vector3 forward,Vector3 right)
{
	//スティックが倒されているなら。
	if (fabsf(forward.x) >= 0.01f || fabsf(forward.z) >= 0.01f ||
		fabsf(right.x) >= 0.01f || fabsf(right.z) >= 0.01f) {
		//playerの向きを更新する。
		m_playerDirection = forward + right;
		m_playerDirection.Normalize();
	}
}

/// <summary>
/// アニメーションステート管理。
/// </summary>
void Player::StateManage()
{
	//ジャンプ中。
	if (m_playerCharaCon.IsOnGround() == false) {
		m_animationState = EnPlayerAnimVar::enJump;
	}
	else {
		//歩行中。
		if (fabsf(m_playerSpeed.x) >= 0.01f || fabsf(m_playerSpeed.z) >= 0.01f) {
			m_animationState = EnPlayerAnimVar::enWalk;
			//Xボタン入力中なら。
			if (g_pad[0]->IsPress(enButtonB)) {
				m_animationState = EnPlayerAnimVar::enRun;
			}
		}
		//待機中。
		/*else {
			m_animationState = EnPlayerAnimVar::enIdle;
		}*/
	}
}

void Player::Rotation()
{
	m_playerRotation.SetRotationYFromDirectionXZ(m_playerDirection);
	m_playerModelRender.SetRotation(m_playerRotation);
}

/// <summary>
/// アニメーション管理。
/// </summary>
void Player::AnimationManage()
{
	switch (m_animationState) {
	case EnPlayerAnimVar::enIdle:
		m_playerModelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case EnPlayerAnimVar::enWalk:
		m_playerModelRender.PlayAnimation(enAnimationClip_Walk);
		break;
	case EnPlayerAnimVar::enJump:
		m_playerModelRender.PlayAnimation(enAnimationClip_Jump);
		break;
	case EnPlayerAnimVar::enRun:
		m_playerModelRender.PlayAnimation(enAnimationClip_Run);
		break;
	case EnPlayerAnimVar::enGuardStart:
		m_playerModelRender.PlayAnimation(enAnimationClip_GuardStart);
		break;
	case EnPlayerAnimVar::enWeakAttack:
		m_playerModelRender.PlayAnimation(enAnimationClip_WeakAttack);
		break;
	case EnPlayerAnimVar::enChargeAttack:
		m_playerModelRender.PlayAnimation(enAnimationClip_ChargeAttack);
		break;
	case EnPlayerAnimVar::enCharging:
		m_playerModelRender.PlayAnimation(enAnimationClip_Charging);
		break;
	case EnPlayerAnimVar::enHit:
		m_playerModelRender.PlayAnimation(enAnimationClip_Hit);
		break;
	case EnPlayerAnimVar::enDeath:
		m_playerModelRender.PlayAnimation(enAnimationClip_Death);
	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}