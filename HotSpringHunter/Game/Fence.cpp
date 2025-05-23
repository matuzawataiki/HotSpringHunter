#include "stdafx.h"
#include "Fence.h"

Fence::Fence()
{
	modelRender.Init("Assets/modelData/BackGround/fence/saku.tkm");
	modelRender.SetPosition(Vector3{ 0.0f,0.0f,5250.0f });
	modelRender.Update();

	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
}

Fence::~Fence()
{
}

void Fence::Update()
{
}

void Fence::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}
