#include "stdafx.h"
#include "HItBoxFence.h"
#include "FenceManager.h"


HItBoxFence::HItBoxFence()
{
	m_objeModel.Init("Assets/stage/HitBoxFence.tkm");

	m_isFlagMemory = true;
}

void HItBoxFence::Update()
{
	PhysicsManage();
}

/// <summary>
/// 当たり判定フェンスの物理管理を行います。
/// </summary>
void HItBoxFence::PhysicsManage()
{
	m_fenceManager = FindGO<FenceManager>("fenceManager");

	if (m_fenceManager->GetIsHitActive() == m_isFlagMemory) {
		return;
	}

	//if (m_fenceManager->GetIsHitActive()) {
	//	m_physicsStaticObject.CreateFromModel(m_objeModel.GetModel(), m_objeModel.GetModel().GetWorldMatrix());
	//}
	//else {
	//	m_physicsStaticObject.Release();
	//}

	m_isFlagMemory = m_fenceManager->GetIsHitActive();
}
