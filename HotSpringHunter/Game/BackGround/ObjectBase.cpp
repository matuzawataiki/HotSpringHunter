#include "stdafx.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase()
{
}

ObjectBase::~ObjectBase()
{
}

void ObjectBase::Render(RenderContext& rc)
{
	m_objeModel.Draw(rc);
}
