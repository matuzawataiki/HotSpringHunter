#include "stdafx.h"
#include "IState.h"
#include "Player.h"
#include "collision/CollisionObject.h"
#include "Enemy/PoisonSnake/PoisonBall.h"


namespace Enemy
{
	PoisonSnakeStateMachine::PoisonSnakeStateMachine()
	{
	}

	PoisonSnakeStateMachine::~PoisonSnakeStateMachine()
	{
	}

	void PoisonSnakeStateMachine::Update()
	{
		K2_ASSERT(m_currentState != nullptr, "カレントステートがNULL");
		if (m_currentState) {
			uint32_t request;
			if (m_currentState->RequestState(request)) {
				m_currentState->Enter();
				m_currentState = FindState(request);
				m_currentState->Enter();
			}
			m_currentState->Update();
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// 待機ステート
	//////////////////////////////////////////////////////////////////////////////

	namespace {
		const float MOVE_SPEED_OFFSET = 10.0f;
	}

	void PoisonSnakeIdleState::Enter()
	{
		m_moveTime = rand() % 4 + 1;
		if (rand() % 2 == 1) {
			m_moveDirection = -1;
		}
		else {
			m_moveDirection = 1;
		}

	}

	void PoisonSnakeIdleState::Update()
	{
		if (m_moveTime < 0) {
			m_moveTime = rand() % 4 + 1;
			if (rand() % 2 == 1) {
				m_moveDirection = -1;
			}
			else {
				m_moveDirection = 1;
			}
		}

		//ターゲットの方向に向かせる
		Vector3 enemyToTarget = m_owner->GetTarget()->GetPlayerPos() - m_owner->GetPosition();
		enemyToTarget.y = 0;
		enemyToTarget.Normalize();
		enemyToTarget.Dot(Vector3::AxisX);
		Quaternion enemyDirection;
		enemyDirection.SetRotationYFromDirectionXZ(enemyToTarget);
		m_owner->SetOwnerRotetion(enemyDirection);
		
		//待機状態なら止まる
		if(m_isIdle)
		{
			if (m_isIdleAnim) {
				m_owner->GetModelRender()->PlayAnimation(enAnimClip_Idle);
			}
			m_owner->SetOwnerMoveSpeed(Vector3::Zero);

			m_isIdleAnim = false;
			m_isMoveAnim = true;
		}
		else
		{
			if (m_isMoveAnim) {
				m_owner->GetModelRender()->PlayAnimation(enAnimClip_Idle);
			}
			//移動方向の設定
			Vector3 moveDirection = Vector3::Zero;
			moveDirection.x = MOVE_SPEED_OFFSET * m_moveDirection;
			m_owner->SetOwnerMoveSpeed(moveDirection);

			m_isIdleAnim = true;
			m_isMoveAnim = false;
		}

		m_moveTime -= g_gameTime->GetFrameDeltaTime();

	}

	void PoisonSnakeIdleState::Exit()
	{
		m_owner->SetOwnerMoveSpeed(Vector3::Zero);
	}

	bool PoisonSnakeIdleState::RequestState(uint32_t& request)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////
	// 攻撃ステート
	//////////////////////////////////////////////////////////////////////////////

	void PoisonSnakeAtkState::Enter()
	{
		ModelRender* ownerModel = m_owner->GetModelRender();
		ownerModel->PlayAnimation(enAnimClip_Attack);
	}

	void PoisonSnakeAtkState::Update()
	{
		if (m_isAttack) {
			return;
		}
		else {
			if (!m_owner->GetIsAttack()) {
				return;
			}

			m_isAttack = true;

			PoisonBall* poisonBall = new PoisonBall(m_owner->GetPosition(), m_owner->GetTarget()->GetPlayerPos());
			AddGo(poisonBall, 0, "poisonBall");

		}
	}

	void PoisonSnakeAtkState::Exit()
	{
		m_owner->AttackOff();
	}

	bool PoisonSnakeAtkState::RequestState(uint32_t& request)
	{

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	// 追従ステート
	//////////////////////////////////////////////////////////////////////////////
	namespace {
		const float MOVEPOINT_FAR = 600.0f;
		const float MOVE_SPEED = 50.0f;
	}

	void PoisonSnakeTrackState::Enter()
	{
		m_owner->GetModelRender()->PlayAnimation(enAnimClip_Run);

		Vector3 moveDirection = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		float length =  moveDirection.Length();

		if (length >= MOVEPOINT_FAR) {
			m_isMove = 1.0f;
		}
		else
		{
			m_isMove = -1.0f;
		}

		Vector3 moveSpeed = moveDirection * MOVE_SPEED * m_isMove;
		m_owner->SetOwnerMoveSpeed(moveSpeed);
	}

	void PoisonSnakeTrackState::Update()
	{

	}

	void PoisonSnakeTrackState::Exit()
	{
		m_owner->SetOwnerMoveSpeed(Vector3::Zero);
	}

	bool PoisonSnakeTrackState::RequestState(uint32_t& request)
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	// ノックバックステート
	//////////////////////////////////////////////////////////////////////////////

	namespace {
		const float KNOCK_BACK_SPEED = 60.0f;
		const float KNOCK_BACK_DECREASE = 0.02;
	}

	void PoisonSnakeKnockBackState::Enter()
	{
		m_owner->GetModelRender()->PlayAnimation(enAnimClip_Hit);

		m_moveSpeed = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		m_moveSpeed.Normalize();
		m_moveSpeed *= KNOCK_BACK_SPEED;
		m_owner->SetOwnerMoveSpeed(m_moveSpeed);
	}

	void PoisonSnakeKnockBackState::Update()
	{
		m_knockDecreased -= KNOCK_BACK_DECREASE;
		m_moveSpeed *= m_knockDecreased;

		m_owner->SetOwnerMoveSpeed(m_moveSpeed);
	}

	void PoisonSnakeKnockBackState::Exit()
	{
		m_owner->SetOwnerMoveSpeed(Vector3::Zero);
	}

	bool PoisonSnakeKnockBackState::RequestState(uint32_t& request)
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	//デスステート
	//////////////////////////////////////////////////////////////////////////////


	void PoisonSnakeDeathState::Enter()
	{
		m_owner->GetModelRender()->PlayAnimation(enAnimClip_Death);

		Vector3 moveDirection = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		moveDirection.y = 0;
		moveDirection.Normalize();

		Vector3 moveSpeed = moveDirection * 200;
		moveSpeed.y = 500;
		m_owner->SetOwnerMoveSpeed(moveSpeed);
	}

	void PoisonSnakeDeathState::Update()
	{
	}

	void PoisonSnakeDeathState::Exit()
	{
		m_owner->SetOwnerMoveSpeed(Vector3::Zero);
	}

	bool PoisonSnakeDeathState::RequestState(uint32_t& request)
	{
		return false;
	}

}
