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
		/// 初期化
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void Init();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// メインレンダリングターゲットの初期化
		/// </summary>
		void InitMainRenderTarget();

		/// <summary>
		/// 2D描画用のレンダーターゲットを初期化
		/// </summary>
		void Init2DRenderTarget();


		/// <summary>
		/// シャドウマップへの描画処理を初期化
		/// </summary>
		void InitShadowMapRender();

		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容を
		/// フレームバッファにコピーするためのスプライトを初期化する
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();

		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピーする
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBufferSprite(RenderContext& rc);

		/// <summary>
		/// レンダリングパイプラインの実行
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// レンダリングターゲットに登録
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
		/// シャドウマップに描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderToShadowMap(RenderContext& rc);

		/// <summary>
		/// 2Dオブジェクトの描画
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);

	private:
		Sprite			m_copyMainRtToFrameBufferSprite;
		SceneLight		m_sceneLight;
		RenderTarget	m_mainRenderTarget;
		RenderTarget	m_2DRenderTarget;						// 2D描画用のレンダ―ターゲット。
		Sprite			m_2DSprite;								// 2D合成用のスプライト。
		Sprite			m_mainSprite;


		PostEffect		m_postEffect;
		ShadowMapRender m_shadowMapRender;

		std::vector<Model*>		m_registerModels;
		std::vector<IRenderer*> m_renderObjects;				// 描画オブジェクトのリスト。


	};
}
