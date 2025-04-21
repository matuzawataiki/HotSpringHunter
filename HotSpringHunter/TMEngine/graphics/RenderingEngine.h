#pragma once
#include "graphics/postEffect/PostEffect.h"
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

	private:
		Sprite m_mainSprite;
		Sprite m_copyMainRtToFrameBufferSprite;
		SceneLight m_sceneLight;
		RenderTarget m_mainRenderTarget;
		PostEffect m_postEffect;
		std::vector<Model*> m_registerModels;

	};
}
