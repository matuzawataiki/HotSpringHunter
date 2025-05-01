#pragma once
namespace nsTMEngine {
	class IRenderer;
	class ShadowMapRender:public Noncopyable
	{
	public:
		/// <summary>
		/// ‰Šú‰»
		/// </summary>
		void Init();

		/// <summary>
		/// •`‰æ
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="renderObjects"></param>
		void Render(RenderContext& rc, std::vector<IRenderer*>& renderObjects);	

		/// <summary>
		/// ãƒ†ã‚¯ã‚¹ãƒãƒ£ã®å–å¾—
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
