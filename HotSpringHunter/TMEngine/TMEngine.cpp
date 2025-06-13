#include "TMEnginePreCompile.h"
#include "TMEngine.h"


namespace nsTMEngine
{
	TMEngine* TMEngine::m_instance = nullptr;
	TMEngine*			g_tmEngine = nullptr;
	RenderingEngine*	g_renderingEngine = nullptr;
	SceneLight*			g_sceneLight = nullptr;
	CollisionObjectManager* g_collisionObjectManager = nullptr;

	void TMEngine::Init(const InitData& initData)
	{
		g_tmEngine = this;
		g_engine = &m_k2EngineLow;
		g_renderingEngine = &m_renderingEngine;
		g_collisionObjectManager = &m_collisionObjectManager;

		m_k2EngineLow.Init(
			initData.hwnd,
			initData.fremeBufferWidth,
			initData.frameBufferHeight
		);

		g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
		g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

		m_renderingEngine.Init();

	}

	void TMEngine::Execute()
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();
		g_engine->BeginFrame();

		g_engine->ExecuteUpdate();

		m_renderingEngine.Update();
		
		g_engine->ExecuteRender();

		m_renderingEngine.Execute(renderContext);

		g_engine->DebubDrawWorld();

		g_engine->EndFrame();
	}

	TMEngine::~TMEngine()
	{
		g_renderingEngine = nullptr;
		g_engine = nullptr;
	}

}