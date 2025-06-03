#include "stdafx.h"
#include "PoisonBall.h"
#include "collision/CollisionObject.h"

namespace {
	const float SPHERE_COLLISION_SIZE = 18.0f;
	const float MOVE_SPEED_OFFSET = 20.0f;
	const float DELETE_TIME = 4.0f;
}

namespace Enemy {
	PoisonBall::PoisonBall(Vector3& position, Vector3& targetPosition)
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

	}



	void PoisonBall::Render(RenderContext& rc)
	{
		m_sphereModel.Draw(rc);
	}
}
