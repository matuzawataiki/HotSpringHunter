#pragma once
namespace nsK2Engine
{
	class CollisionObject;
}

namespace Character {
	class Player;
}

namespace Enemy{
	struct PoisonBallStatus
	{
		float m_sphereCollisionSize;
		float m_moveSpeed;
		float m_deleteTime;
		float m_damage;

		PoisonBallStatus()
			:m_sphereCollisionSize(0.0f)
			, m_moveSpeed(0.0f)
			, m_deleteTime(0.0f)
			, m_damage(0.0f)
		{}
	};

	class PoisonBall:public IGameObject
	{
	public:
		PoisonBall(const Vector3& position, const Vector3& targetPosition, Character::Player& target);
		~PoisonBall();
		void SetupStatus();

		void Update();
		void Render(RenderContext& rc);

	private:
		Character::Player*	m_target = nullptr;
		PoisonBallStatus* m_status = nullptr;

		CollisionObject		m_sphereCollision;		//玉のコライダー
		ModelRender			m_sphereModel;			//玉のモデル
		
		Vector3 m_position			= Vector3::Zero;	//玉の位置
		Vector3 m_targetPosition	= Vector3::Zero;	//ターゲットの位置
		Vector3 m_moveDirection		= Vector3::Zero;	//玉の移動する方向

	};
}

