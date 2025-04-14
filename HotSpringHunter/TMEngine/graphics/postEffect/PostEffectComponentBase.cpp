#include "TMEnginePreCompile.h"
#include "PostEffectComponentBase.h"
namespace nsTMEngine
{
	void PostEffectComponentBase::Init(RenderTarget& mainRenderTarget)
	{
		OnInit(mainRenderTarget);

		if (IsCopyResultTextureToMainRenderTarget())
		{
			SpriteInitData initData;
			initData.m_width = mainRenderTarget.GetWidth();
			initData.m_height = mainRenderTarget.GetHeight();
			initData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
			initData.m_fxFilePath = "Assets/shader/sprite.fx";
			initData.m_textures[0] = &GetResultTexture();
			m_copyMainRtSprite.Init(initData);
		}
	}

	void PostEffectComponentBase::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		OnRender(rc, mainRenderTarget);

		if (IsCopyResultTextureToMainRenderTarget())
		{
			rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
			rc.SetRenderTargetAndViewport(mainRenderTarget);
			m_copyMainRtSprite.Draw(rc);
			rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		}

	}
}