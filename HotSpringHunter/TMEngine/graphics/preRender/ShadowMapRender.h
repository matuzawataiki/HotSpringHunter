#pragma once
namespace nsTMEngine {
	class IRenderer;
	class ShadowMapRender:public Noncopyable
	{
	public:
		/// <summary>
		/// åˆæœŸåŒ–
		/// </summary>
		void Init();

		/// <summary>
		/// æç”»
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="renderObjects"></param>
		void Render(RenderContext& rc, std::vector<IRenderer*>& renderObjects);	

		/// <summary>
		/// ƒeƒNƒXƒ`ƒƒ‚Ìæ“¾
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
