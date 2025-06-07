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
		/// 蛻晄悄蛹・
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void Init();

		/// <summary>
		/// 譖ｴ譁ｰ蜃ｦ逅・
		/// </summary>
		void Update();

		/// <summary>
		/// 繝｡繧､繝ｳ繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ蛻晄悄蛹・
		/// </summary>
		void InitMainRenderTarget();

		/// <summary>
		/// 2D謠冗判逕ｨ縺ｮ繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繧貞・譛溷喧
		/// </summary>
		void Init2DRenderTarget();


		/// <summary>
		/// 繧ｷ繝｣繝峨え繝槭ャ繝励∈縺ｮ謠冗判蜃ｦ逅・ｒ蛻晄悄蛹・
		/// </summary>
		void InitShadowMapRender();

		/// <summary>
		/// 繝｡繧､繝ｳ繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ繧ｫ繝ｩ繝ｼ繝舌ャ繝輔ぃ縺ｮ蜀・ｮｹ繧・
		/// 繝輔Ξ繝ｼ繝繝舌ャ繝輔ぃ縺ｫ繧ｳ繝斐・縺吶ｋ縺溘ａ縺ｮ繧ｹ繝励Λ繧､繝医ｒ蛻晄悄蛹悶☆繧・
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();

		/// <summary>
		/// 繝｡繧､繝ｳ繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ蜀・ｮｹ繧偵ヵ繝ｬ繝ｼ繝繝舌ャ繝輔ぃ縺ｫ繧ｳ繝斐・縺吶ｋ
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBufferSprite(RenderContext& rc);

		/// <summary>
		/// 繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ繝代う繝励Λ繧､繝ｳ縺ｮ螳溯｡・
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// 繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｫ逋ｻ骭ｲ
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
		void RemoveRenderObject(IRenderer* renderObject)
		{
			
		}

		/// <summary>
		/// 繧ｷ繝｣繝峨え繝槭ャ繝励↓謠冗判
		/// </summary>
		/// <param name="rc"></param>
		void RenderToShadowMap(RenderContext& rc);

		/// <summary>
		/// 2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・謠冗判
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);

		/// <summary>
		/// シャドウマップの取得
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
		RenderTarget	m_2DRenderTarget;						// 2D謠冗判逕ｨ縺ｮ繝ｬ繝ｳ繝窶輔ち繝ｼ繧ｲ繝・ヨ縲・
		Sprite			m_2DSprite;								// 2D蜷域・逕ｨ縺ｮ繧ｹ繝励Λ繧､繝医・
		Sprite			m_mainSprite;


		PostEffect		m_postEffect;
		ShadowMapRender m_shadowMapRender;

		std::vector<Model*>		m_registerModels;
		std::vector<IRenderer*> m_renderObjects;				// 謠冗判繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝ｪ繧ｹ繝医・


	};
}
