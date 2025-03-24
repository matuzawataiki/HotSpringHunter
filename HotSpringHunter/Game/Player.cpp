#include "stdafx.h"
#include "Player.h"

namespace {
	const float MOVE_AMOUNT = 120.0f;
	const float JUMP_AMOUNT = 700.0f;
	const float DASH_AMOUNT = 2.5f;
	const float GRAVITY_AMOUNT = 10.0f;

	const Vector3 PLAYER_NEW_POSITION = Vector3{ 0.0f,300.0f,0.0f };
}

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	m_playerModelRender.Init("Assets/ModelData/UnityChan.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);

	//player座標初期化
	m_playerPosition = PLAYER_NEW_POSITION;
	//playerキャラコン初期化
	m_playerCharaCon.Init(25.0f, 75.0f, m_playerPosition);

	return true;
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

		//左スティックの入力量に120.0fを乗算。
		forwardDir *= stickL.y * MOVE_AMOUNT * m_runState;
		rightDir *= stickL.x * MOVE_AMOUNT * m_runState;

		//移動速度に加算。
		m_playerSpeed += rightDir + forwardDir;

		//ダッシュ(地面についているときだけ入力可）。
		if (g_pad[0]->IsPress(enButtonX)) {
			m_runState = DASH_AMOUNT;
		}
		else if (g_pad[0]->IsPress(enButtonX) == false) {
			m_runState = 1.0f;
		}

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

/// <summary>
/// ステート管理。
/// </summary>
void Player::StateManage()
{
	//ジャンプ中。
	if (m_playerCharaCon.IsOnGround() == false) {
		m_animationState = EnPlayerAnimVar::jump;
	}
	else {
		//歩行中。
		if (fabsf(m_playerSpeed.x) >= 0.001f || fabsf(m_playerSpeed.z) >= 0.001f) {
			m_animationState = EnPlayerAnimVar::walk;
			//Xボタン入力中なら。
			if (g_pad[0]->IsPress(enButtonX)) {
				m_animationState = EnPlayerAnimVar::run;
			}
		}
		//待機中。
		else {
			m_animationState = EnPlayerAnimVar::idle;
		}
	}
}

void Player::Rotation()
{
	if (fabsf(m_playerSpeed.x) >= 0.01f || fabsf(m_playerSpeed.z) >= 0.01f) {
		m_playerRotation.SetRotationYFromDirectionXZ(m_playerSpeed);
		m_playerModelRender.SetRotation(m_playerRotation);
	}
}

/// <summary>
/// アニメーション管理。
/// </summary>
void Player::AnimationManage()
{
	switch (m_animationState) {
	case EnPlayerAnimVar::idle:
		m_playerModelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case EnPlayerAnimVar::walk:
		m_playerModelRender.PlayAnimation(enAnimationClip_Walk);
		break;
	case EnPlayerAnimVar::jump:
		m_playerModelRender.PlayAnimation(enAnimationClip_Jump);
		break;
	case EnPlayerAnimVar::run:
		m_playerModelRender.PlayAnimation(enAnimationClip_Run);
		break;
	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}