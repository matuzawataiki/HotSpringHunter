#pragma once
namespace Enemy
{
	class EnemyBase : public IGameObject
	{
	public:
		bool Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;

	private:
		Vector3 m_position;		//位置
		Vector3 m_scale;		//拡大率

		Quaternion m_rotation;	//回転

		ModelRender m_enemyModel;

	};
}