#pragma once
#include "graphics/postEffect/PostEffect.h"
#include "graphics/preRender/ShadowMapRender.h"
#include "graphics/light/SceneLight.h"

namespace nsTMEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		/// <summary>
		/// ‰Šú‰»
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void Init();

		/// <summary>
		/// XVˆ—
		/// </summary>
		void Update();

		/// <summary>
		/// ƒƒCƒ“ƒŒƒ“ƒ_ƒŠƒ“ƒOƒ^[ƒQƒbƒg‚Ì‰Šú‰»
		/// </summary>
		void InitMainRenderTarget();

		/// <summary>
		/// 2D•`‰æ—p‚ÌƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg‚ğ‰Šú‰»
		/// </summary>
		void Init2DRenderTarget();


		/// <summary>
		/// ƒVƒƒƒhƒEƒ}ƒbƒv‚Ö‚Ì•`‰æˆ—‚ğ‰Šú‰»
		/// </summary>
		void InitShadowMapRender();

		/// <summary>
		/// ƒƒCƒ“ƒŒƒ“ƒ_ƒŠƒ“ƒOƒ^[ƒQƒbƒg‚ÌƒJƒ‰[ƒoƒbƒtƒ@‚Ì“à—e‚ğ
		/// ƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚ÉƒRƒs[‚·‚é‚½‚ß‚ÌƒXƒvƒ‰ƒCƒg‚ğ‰Šú‰»‚·‚é
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();

		/// <summary>
		/// ƒƒCƒ“ƒŒƒ“ƒ_ƒŠƒ“ƒOƒ^[ƒQƒbƒg‚Ì“à—e‚ğƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚ÉƒRƒs[‚·‚é
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBufferSprite(RenderContext& rc);

		/// <summary>
		/// ƒŒƒ“ƒ_ƒŠƒ“ƒOƒpƒCƒvƒ‰ƒCƒ“‚ÌÀs
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// ƒŒƒ“ƒ_ƒŠƒ“ƒOƒ^[ƒQƒbƒg‚É“o˜^
		/// </summary>
		/// <param name="model"></param>
		void RegisterModel(Model* model)
		{
			m_registerModels.push_back(model);
		}

		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// ƒVƒƒƒhƒEƒ}ƒbƒv‚É•`‰æ
		/// </summary>
		/// <param name="rc"></param>
		void RenderToShadowMap(RenderContext& rc);

		/// <summary>
		/// 2DƒIƒuƒWƒFƒNƒg‚Ì•`‰æ
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);

		/// <summary>
		/// ã‚·ãƒ£ãƒ‰ã‚¦ãƒãƒƒãƒ—ã®å–å¾—
		/// </summary>
		/// <returns></returns>
		Texture& GetShadowMap()
		{
			return m_shadowMapRender.GetTexture();
		}

	private:
		Sprite			m_copyMainRtToFrameBufferSprite;
		SceneLight		m_sceneLight;
		RenderTarget	m_mainRenderTarget;
		RenderTarget	m_2DRenderTarget;						// 2D•`‰æ—p‚ÌƒŒƒ“ƒ_\ƒ^[ƒQƒbƒgB
		Sprite			m_2DSprite;								// 2D‡¬—p‚ÌƒXƒvƒ‰ƒCƒgB
		Sprite			m_mainSprite;


		PostEffect		m_postEffect;
		ShadowMapRender m_shadowMapRender;

		std::vector<Model*>		m_registerModels;
		std::vector<IRenderer*> m_renderObjects;				// •`‰æƒIƒuƒWƒFƒNƒg‚ÌƒŠƒXƒgB


	};
}
