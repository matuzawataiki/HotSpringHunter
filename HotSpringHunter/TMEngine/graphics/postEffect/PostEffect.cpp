#include "TMEnginePreCompile.h"
#include "PostEffect.h"
namespace nsTMEngine
{
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		m_bloom.Init(mainRenderTarget);
	}
	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		m_bloom.Render(rc, mainRenderTarget);
	}
}