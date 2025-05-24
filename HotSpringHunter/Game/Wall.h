#pragma once
class Wall : public IGameObject
{
public:
	Wall();
	~Wall();

	void Render(RenderContext& rc);

	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;
};

