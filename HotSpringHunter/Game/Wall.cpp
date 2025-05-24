#include "stdafx.h"
#include "Wall.h"

Wall::Wall()
{
	//“§–¾‚È•Ç
	modelRender.Init("Assets/ModelData/BackGround/kabe.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
}

Wall::~Wall()
{
}

void Wall::Render(RenderContext& rc)
{
}
