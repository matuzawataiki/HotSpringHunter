#include "stdafx.h"
#include "PoisonBall.h"
#include "Player.h"
#include "collision/CollisionObject.h"

namespace Enemy {
	PoisonBall::PoisonBall(const Vector3& position, const Vector3& targetPosition, Character::Player& target)
	{
		SetupStatus();

		m_position = position;
		m_targetPosition = targetPosition;
		m_sphereModel.Init("Assets/modelData/snake/poisonSnake/poisonBall.tkm");

		m_position.y += 50.0f;
		m_moveDirection = m_targetPosition - m_position;
		m_moveDirection.Normalize();
		m_moveDirection.y = 0.0f;
		m_position += m_moveDirection * 30.0f;

		m_sphereCollision.CreateSphere(m_position, Quaternion::Identity, m_status->m_sphereCollisionSize);
		m_sphereCollision.SetTimeLimit(m_status->m_deleteTime);
		m_sphereCollision.SetPosition(m_position);

		m_sphereModel.SetPosition(m_position);

		m_target = &target;
	}

	PoisonBall::~PoisonBall()
	{
	}

	void PoisonBall::SetupStatus()
	{
		m_status = new PoisonBallStatus;

		nlohmann::json j = LoadScene("Assets/Json/poisonBall.json");
		auto status = j["Status"];

		m_status->m_sphereCollisionSize = status["SphereCollisionSize"];
		m_status->m_moveSpeed = status["MoveSpeed"];
		m_status->m_deleteTime = status["DeleteTime"];
		m_status->m_damage= status["Damage"];
	}

	void PoisonBall::Update()
	{
		if (m_sphereCollision.IsDead()) {
			DeleteGO(this);
			return;
		}

		m_position += m_moveDirection * m_status->m_moveSpeed;

		m_sphereCollision.SetPosition(m_position);
		m_sphereCollision.Update();
		m_sphereModel.SetPosition(m_position);
		m_sphereModel.Update();

		//playerがnullなら実行しない
		m_target = FindGO<Character::Player>("player");
		if(m_target == nullptr) {
			return;
		}
		if (m_sphereCollision.IsHit(m_target->m_playerCharaCon)) {
			m_target->Hit(m_status->m_damage);
			DeleteGO(this);
		}
	}



	void PoisonBall::Render(RenderContext& rc)
	{
		m_sphereModel.Draw(rc);
	}
}
