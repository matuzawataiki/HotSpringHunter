#include "stdafx.h"
#include "InOutHitBox.h"

void InOutHitBox::init(Vector3 positon, Quaternion rotation, Vector3 scale)
{
	m_objectModel.Init("Assets/stage/InOutBox.tkm", nullptr, 0, enModelUpAxisY);
	m_objectModel.SetTRS(positon, rotation, scale);
	m_objectModel.Update();

	Quaternion rot = rotation;
	rot.x = -1.0f;

	m_collision->CreateMesh(
		positon,
		rot,
		m_objectModel.GetModel(),
		g_matIdentity
	);

}
