#pragma once
#include "BackGround/HitObjectBase.h"

class FenceManager;
class HItBoxFence:public HitObjectBase
{
public:
	HItBoxFence();
	//当たり判定を管理
	void PhysicsManage();
	void Update()override;

private:
	FenceManager*	m_fenceManager		= nullptr;

	bool			m_isFlagMemory		= false;
};

