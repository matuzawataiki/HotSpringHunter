#include "stdafx.h"
#include "PoisonBall.h"
#include "Player.h"
#include "collision/CollisionObject.h"

namespace {
	static const float SPHERE_COLLISION_SIZE = 18.0f;
	static const float MOVE_SPEED_OFFSET = 20.0f;
	static const float DELETE_TIME = 4.0f;
	static const float DAMAGE = 20.0F;
}

namespace Enemy {
	PoisonBall::PoisonBall(const Vector3& position, const Vector3& targetPosition, Character::Player& target)
	{
		m_position = position;
		m_targetPosition = targetPosition;
		m_sphereModel.Init("Assets/modelData/snake/poisonSnake/poisonBall.tkm");

		m_position.y += 50.0f;
		m_moveDirection = m_targetPosition - m_position;
		m_moveDirection.Normalize();
		m_moveDirection.y = 0.0f;
		m_position += m_moveDirection * 30.0f;

		m_sphereCollision.CreateSphere(m_position, Quaternion::Identity, SPHERE_COLLISION_SIZE);
		m_sphereCollision.SetTimeLimit(DELETE_TIME);
		m_sphereCollision.SetPosition(m_position);

		m_sphereModel.SetPosition(m_position);

		m_target = &target;
	}

	PoisonBall::~PoisonBall()
	{
	}

	void PoisonBall::Update()
	{
		if (m_sphereCollision.IsDead()) {
			DeleteGO(this);
			return;
		}

		m_position += m_moveDirection * MOVE_SPEED_OFFSET;

		m_sphereCollision.SetPosition(m_position);
		m_sphereModel.SetPosition(m_position);

		m_sphereModel.Update();

		if (m_sphereCollision.IsHit(m_target->m_playerCharaCon)) {
			m_target->Hit(DAMAGE);
			DeleteGO(this);
		}
	}



	void PoisonBall::Render(RenderContext& rc)
	{
		m_sphereModel.Draw(rc);
	}
}
