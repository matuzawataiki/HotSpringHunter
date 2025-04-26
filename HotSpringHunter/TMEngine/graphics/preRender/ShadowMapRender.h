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
	private:
		IRenderer m_renderer;
		RenderTarget m_shadowMap;
	};
}
