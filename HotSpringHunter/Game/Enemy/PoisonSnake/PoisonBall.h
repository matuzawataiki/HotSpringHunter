#pragma once
namespace nsK2Engine
{
	class CollisionObject;
}

namespace Enemy{
	class PoisonBall:public IGameObject
	{
	public:
		PoisonBall(Vector3& position, Vector3& targetPosition);
		~PoisonBall();
		void Update();
		void Render(RenderContext& rc);

	private:
		CollisionObject		m_sphereCollision;		//玉のコライダー
		ModelRender			m_sphereModel;			//玉のモデル

		Vector3 m_position			= Vector3::Zero;	//玉の位置
		Vector3 m_targetPosition	= Vector3::Zero;	//ターゲットの位置
		Vector3 m_moveDirection		= Vector3::Zero;	//玉の移動する方向

	};
}

