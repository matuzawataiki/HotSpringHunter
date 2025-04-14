#pragma once
namespace nsTMEngine {
	class PostEffectComponentBase:public Noncopyable
	{
	public:
		void Init(RenderTarget& mainRenderTarget);
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
		
	private:
		virtual void OnInit(RenderTarget& mainRenderTarget) = 0;

		virtual void OnRender(RenderContext&rc, RenderTarget& mainRenderTarget) = 0;
		
		virtual Texture& GetResultTexture()
		{
			static Texture nullTexture;
			return nullTexture;
		}
		
		virtual bool IsCopyResultTextureToMainRenderTarget() const
		{
			return false;
		}

		Sprite m_copyMainRtSprite;
	
	};
}
