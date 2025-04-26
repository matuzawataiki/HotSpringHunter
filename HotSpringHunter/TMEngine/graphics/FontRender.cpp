#include "TMEnginePreCompile.h"
#include "FontRender.h"

namespace nsTMEngine {
	void FontRender::Draw(RenderContext& rc)
	{
		if (m_text == nullptr)
		{
			return;
		}
		g_renderingEngine->AddRenderObject(this);
	}
}