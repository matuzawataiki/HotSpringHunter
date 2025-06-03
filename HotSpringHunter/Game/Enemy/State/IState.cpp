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

	///////////////////////////////////
	// 待機ステート
	///////////////////////////////////

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
			m_owner->SetOwnerMoveSpeed(Vector3::Zero);
		}
		else
		{
			//移動方向の設定
			Vector3 moveDirection = Vector3::Zero;
			moveDirection.x = MOVE_SPEED_OFFSET * m_moveDirection;
			m_owner->SetOwnerMoveSpeed(moveDirection);
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

	///////////////////////////////////
	// 攻撃ステート
	///////////////////////////////////

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

			NewGO<PoisonBall>(0, "poisonBall");

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

}
