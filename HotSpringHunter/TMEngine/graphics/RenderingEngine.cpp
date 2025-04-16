#include "TMEnginePreCompile.h"
#include "RenderingEngine.h"
namespace nsTMEngine
{
	RenderingEngine::RenderingEngine()
	{
		g_sceneLight = &m_sceneLight;
	}
	RenderingEngine::~RenderingEngine()
	{
		g_sceneLight = nullptr;
	}

	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
		m_postEffect.Init(m_mainRenderTarget);
	}

	void RenderingEngine::Update()
	{
		g_sceneLight->Update();
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		spriteInitData.m_fxFilePath = "Asset/shader/sprite.fx";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);


	}

	void RenderingEngine::CopyMainRenderTargetToFrameBufferSprite(RenderContext& rc)
	{
		BeginGPUEvent("CopyMainRenderTargetToFrameBuffer");

		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);

		EndGPUEvent();
	}
	void RenderingEngine::Execute(RenderContext& rc)
	{
		m_postEffect.Render(rc, m_mainRenderTarget);
		CopyMainRenderTargetToFrameBufferSprite(rc);
	}
}