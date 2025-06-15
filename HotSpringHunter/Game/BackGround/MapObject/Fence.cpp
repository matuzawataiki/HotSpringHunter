#include "stdafx.h"
#include "Fence.h"
#include "FenceManager.h"

Fence::Fence()
{
	m_objeModel.Init("Assets/stage/fence.tkm");
}

void Fence::Render(RenderContext& rc)
{
	m_fenceManager = FindGO<FenceManager>("fenceManager");

	//配置されているときだけ描画
	/*if (m_fenceManager->GetIsDrawFence()) {
		m_objeModel.Draw(rc);
	}*/
}
