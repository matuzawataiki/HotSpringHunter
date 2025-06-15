#include "stdafx.h"
#include "HitObjectBase.h"

void HitObjectBase::CreatePhysicsObject()
{
	m_physicsStaticObject->CreateFromModel(m_objeModel.GetModel(), m_objeModel.GetModel().GetWorldMatrix());
}
