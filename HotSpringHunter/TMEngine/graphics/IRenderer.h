#pragma once

namespace nsTMEngine {
	class IRenderer : public Noncopyable
	{
	public:
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRenderShadowMap(RenderContext& rc){}
		
		/// <summary>
		/// 2D描画パスから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc){}


	};
}

