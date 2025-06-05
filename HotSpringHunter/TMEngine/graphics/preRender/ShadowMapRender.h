#pragma once
namespace nsTMEngine {
	class IRenderer;
	class ShadowMapRender:public Noncopyable
	{
	public:
		/// <summary>
		/// 蛻晄悄蛹・
		/// </summary>
		void Init();

		/// <summary>
		/// 謠冗判
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="renderObjects"></param>
		void Render(RenderContext& rc, std::vector<IRenderer*>& renderObjects);	

		/// <summary>
		/// テクスチャの取得
		/// </summary>
		/// <returns></returns>
		Texture& GetTexture()
		{
			return m_shadowMap.GetRenderTargetTexture();
		}
	
	private:
		IRenderer m_renderer;
		RenderTarget m_shadowMap;
	};
}
