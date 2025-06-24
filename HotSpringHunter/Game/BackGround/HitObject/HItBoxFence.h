#pragma once
#include "BackGround/HitObjectBase.h"

class FenceManager;
class HItBoxFence:public HitObjectBase
{
public:
	HItBoxFence();
	void Update()override;

private:
	bool			m_isFlagMemory		= false;
};

