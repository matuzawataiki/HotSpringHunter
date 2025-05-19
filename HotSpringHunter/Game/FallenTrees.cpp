#include "stdafx.h"
#include "FallenTrees.h"

#include "BackGround.h"
#include "Player.h"

FallenTrees::FallenTrees()
{
	m_treesModel.Init("Assets/modelData/");
	m_treesModel.Update();

	physicsStaticObject.CreateFromModel(m_treesModel.GetModel(), m_treesModel.GetModel().GetWorldMatrix());
}

FallenTrees::~FallenTrees()
{
}

bool FallenTrees::Start()
{
	return false;
}

void FallenTrees::Update()
{
}

void FallenTrees::Render(RenderContext& rc)
{
	m_treesModel.Draw(rc);
}
