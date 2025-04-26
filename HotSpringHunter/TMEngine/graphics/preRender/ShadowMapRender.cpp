#include "TMEnginePreCompile.h"
#include "ShadowMapRender.h"

void nsTMEngine::ShadowMapRender::Init()
{
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	m_shadowMap.Create(
		1024, 1024, 1, 1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

}

void nsTMEngine::ShadowMapRender::Render(RenderContext& rc, std::vector<IRenderer*>& renderObjects)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);
	for (auto& renderer : renderObjects) {
		renderer->OnRenderShadowMap(rc);
	}
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
}
