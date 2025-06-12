#pragma once
#include "BackGround/ObjectBase.h"

class FenceManager;
class Fence :public ObjectBase
{
public:
	Fence();
	void Render(RenderContext& rc)override;

private:
	FenceManager* m_fenceManager = nullptr;
};

