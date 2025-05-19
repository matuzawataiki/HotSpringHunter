#pragma once
class Fence : public IGameObject
{
public:
	Fence();
	~Fence();

	void Update();
	void Render(RenderContext& rc);

	Vector3 m_fencePosition;

	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;
};

