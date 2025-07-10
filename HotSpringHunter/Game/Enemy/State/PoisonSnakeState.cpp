#include "stdafx.h"
#include "PoisonSnakeState.h"

#include "collision/CollisionObject.h"
#include "Enemy/PoisonSnake/PoisonBall.h"

#include "Player.h"

namespace Enemy {
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
				m_currentState->Exit();
				m_currentState = FindState(request);
				m_currentState->Enter();
			}
			m_currentState->Update();
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// 待機ステート
	//////////////////////////////////////////////////////////////////////////////

	void PoisonSnakeIdleState::SetupStatus()
	{
		// デストラクタで消してね
		m_status = new IdelStatus();

		nlohmann::json j = LoadScene("Assets/Json/poisonSnake.json");
		auto status = j["IdelStatus"];
		m_status->m_IdelMoveSpeed = status["IdelMoveSpeed"];
		m_status->m_IdelRangeFar = status["IdelRangeFar"];
		m_status->m_IdelRangeNiar = status["IdelRangeNiar"];
	}

	void PoisonSnakeIdleState::Enter()
	{
		m_moveTime = rand() % 2 + 1;
		if (rand() % 2 == 1) {
			m_sideDirection = -1;
		}
		else {
			m_sideDirection = 1;
		}

		m_isIdleAnim = false;
		m_isMoveAnim = true;

	}

	void PoisonSnakeIdleState::Update()
	{
		if (m_moveTime < 0) {
			m_moveTime = rand() % 4 + 1;
			if (rand() % 2 == 1) {
				m_sideDirection = -1;
			}
			else {
				m_sideDirection = 1;
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
		if (m_isIdle)
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

			Vector3 direction = m_owner->GetTarget()->GetPlayerPos() - m_owner->GetPosition();
			direction.y = 0.0f;
			direction = { -direction.z, 0.0f, direction.x };
			direction.Normalize();
			direction *= m_status->m_IdelMoveSpeed * m_sideDirection;

			m_owner->SetOwnerMoveSpeed(direction);

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
		if (m_owner->IsDeath()) {
			request = PoisonSnakeDeathState::ID();
			return true;
		}

		if (m_owner->IsAttackCooldown()) {
			request = PoisonSnakeAtkState::ID();
			return true;
		}

		if (m_owner->IsHit()) {
			request = PoisonSnakeKnockBackState::ID();
			return true;
		}

		Vector3 enemyToPlayer = m_owner->GetTarget()->GetPlayerPos() - m_owner->GetPosition();
		float length = enemyToPlayer.Length();

		if (length > m_status->m_IdelRangeFar || m_status->m_IdelRangeNiar > length) {
			request = PoisonSnakeTrackState::ID();
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	// 攻撃ステート
	//////////////////////////////////////////////////////////////////////////////

	void PoisonSnakeAtkState::Enter()
	{
		ModelRender* ownerModel = m_owner->GetModelRender();
		ownerModel->PlayAnimation(enAnimClip_Attack);
		m_isCreateBall = true;

	}

	void PoisonSnakeAtkState::Update()
	{
		if (m_isCreateBall) {

			m_isCreateBall = false;

			PoisonBall* poisonBall = new PoisonBall(m_owner->GetPosition(), m_owner->GetTarget()->GetPlayerPos(), *m_owner->GetTarget());
			AddGo(poisonBall, 0, "poisonBall");
		}

	}

	void PoisonSnakeAtkState::Exit()
	{
		m_owner->AttackOff();
	}

	bool PoisonSnakeAtkState::RequestState(uint32_t& request)
	{
		if (!m_owner->GetModelRender()->IsPlayAnimation()) {
			request = PoisonSnakeIdleState::ID();
			return true;
		}

		if (m_owner->IsDeath()) {
			request = PoisonSnakeDeathState::ID();
			return true;
		}

		if (m_owner->IsHit()) {
			request = PoisonSnakeKnockBackState::ID();
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	// 追従ステート
	//////////////////////////////////////////////////////////////////////////////

	void PoisonSnakeTrackState::SetupStatus()
	{
		// デストラクタで消してね
		m_status = new TrackStatus();

		nlohmann::json j = LoadScene("Assets/Json/poisonSnake.json");
		auto status = j["TrackStatus"];

		m_status->m_movePointFar = status["MovePointFar"];
		m_status->m_movePointNiar = status["MovePointNiar"];
		m_status->m_moveSpeed = status["TrackMoveSpeed"];
	}

	void PoisonSnakeTrackState::Enter()
	{
		m_owner->GetModelRender()->PlayAnimation(enAnimClip_Run);


		Vector3 moveDirection = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		float length = moveDirection.Length();
		moveDirection.Normalize();

		if (length >= m_status->m_movePointFar) {
			m_isMove = -1.0f;
		}
		else
		{
			m_isMove = 1.0f;
		}


	}

	void PoisonSnakeTrackState::Update()
	{

		Vector3 moveDirection = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		float length = moveDirection.Length();
		moveDirection.Normalize();
		moveDirection.y = 0.0f;

		Quaternion enemyDirection;
		enemyDirection.SetRotationYFromDirectionXZ((moveDirection * m_isMove));
		m_owner->SetOwnerRotetion(enemyDirection);

		Vector3 moveSpeed = moveDirection * m_status->m_moveSpeed * m_isMove;
		m_owner->SetOwnerMoveSpeed(moveSpeed);
	}

	void PoisonSnakeTrackState::Exit()
	{
		m_owner->SetOwnerMoveSpeed(Vector3::Zero);
	}

	bool PoisonSnakeTrackState::RequestState(uint32_t& request)
	{
		if (m_owner->IsDeath()) {
			request = PoisonSnakeDeathState::ID();
			return true;
		}

		if (m_owner->IsHit()) {
			request = PoisonSnakeKnockBackState::ID();
			return true;
		}

		Vector3 toEnemyPlayer = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		float	lenghth = toEnemyPlayer.Length();

		if (m_isMove == -1.0f) {
			if (m_status->m_movePointFar > lenghth) {
				request = PoisonSnakeIdleState::ID();
				return true;
			}
		}
		else {
			if (m_status->m_movePointNiar < lenghth) {
				request = PoisonSnakeIdleState::ID();
				return true;
			}
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	// ノックバックステート
	//////////////////////////////////////////////////////////////////////////////

	void PoisonSnakeKnockBackState::SetupStatus()
	{
		// デストラクタで消してね
		m_status = new KnockBackStatus();

		nlohmann::json j = LoadScene("Assets/Json/poisonSnake.json");
		auto status = j["KnockBackStatus"];

		m_status->m_knockBackSpeed = status["KnockBackSpeed"];
		m_status->m_knockBackDecrease = status["KnockBackDecrease"];
	}

	void PoisonSnakeKnockBackState::Enter()
	{
		m_knockDecreased = 1.0f;

		m_owner->GetModelRender()->PlayAnimation(enAnimClip_Hit);

		m_moveSpeed = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		m_moveSpeed.Normalize();
		m_moveSpeed *= m_status->m_knockBackSpeed;
		m_owner->SetOwnerMoveSpeed(m_moveSpeed);
	}

	void PoisonSnakeKnockBackState::Update()
	{
		m_knockDecreased -= m_status->m_knockBackDecrease;
		m_moveSpeed *= m_knockDecreased;

		m_owner->SetOwnerMoveSpeed(m_moveSpeed);
	}

	void PoisonSnakeKnockBackState::Exit()
	{
		m_owner->SetOwnerMoveSpeed(Vector3::Zero);
		m_owner->ChangeHitFlag();
	}

	bool PoisonSnakeKnockBackState::RequestState(uint32_t& request)
	{
		if (m_knockDecreased < 0.7f) {
			request = PoisonSnakeIdleState::ID();
			return true;
		}

		if (m_owner->IsDeath()) {
			request = PoisonSnakeDeathState::ID();
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	//デスステート
	//////////////////////////////////////////////////////////////////////////////

	namespace {
		float DEATH_TIME = 3.0f;
		float MOVE_OFFSET = 40.0f;
	}

	void PoisonSnakeDeathState::SetupStatus()
	{
		// デストラクタで消してね
		m_status = new DeathStatus();

		nlohmann::json j = LoadScene("Assets/Json/poisonSnake.json");
		auto status = j["DeathStatus"];

		m_status->m_deathTime = status["DeathTime"];
		m_status->m_moveSpeed = status["DeathMoveSpeed"];
	}

	void PoisonSnakeDeathState::Enter()
	{
		m_owner->GetModelRender()->PlayAnimation(enAnimClip_Death);

		Vector3 moveDirection = m_owner->GetPosition() - m_owner->GetTarget()->GetPlayerPos();
		moveDirection.y = 0;
		moveDirection.Normalize();

		Vector3 moveSpeed = moveDirection;
		moveSpeed.y += 0.25f;
		moveSpeed *= MOVE_OFFSET;
		m_owner->SetOwnerMoveSpeed(moveSpeed);

		m_lifeTime = DEATH_TIME;
	}

	void PoisonSnakeDeathState::Update()
	{
		m_lifeTime -= g_gameTime->GetFrameDeltaTime();
	}

	void PoisonSnakeDeathState::Exit()
	{
		m_owner->SetOwnerMoveSpeed(Vector3::Zero);
	}

	bool PoisonSnakeDeathState::RequestState(uint32_t& request)
	{
		if (m_lifeTime < 0.0f) {
			m_owner->Deash();
		}
		return false;
	}
}