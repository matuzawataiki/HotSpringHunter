#pragma once

class BackGround;
class Player;

class FallenTrees : public IGameObject
{
public:
	FallenTrees();
	~FallenTrees();

	bool Start() override;
	void Update() override;

	void Render(RenderContext& rc);

	ModelRender m_treesModel;  //ƒ‚ƒfƒ‹
	PhysicsStaticObject physicsStaticObject;  //“–‚½‚è”»’è
};

