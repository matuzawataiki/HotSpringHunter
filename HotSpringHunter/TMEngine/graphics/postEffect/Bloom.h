#pragma once
#include "graphics/postEffect/PostEffectComponentBase.h"
namespace nsTMEngine {
	class Bloom:public PostEffectComponentBase
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void OnInit(RenderTarget& mainRenderTarget);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

		/// <summary>
		/// ポストエフェクトを実行した結果となるテクスチャを取得
		/// </summary>
		/// <returns></returns>
		Texture& GetResultTexture() override
		{
			return m_luminanceRenderTarget.GetRenderTargetTexture();
		}

		/// <summary>
		/// ブルームが発生する閾値を設定
		/// </summary>
		/// <param name="value"></param>
		void SetThreshold(float value)
		{
			m_samplingLuminanceCB1.threshold = value;
		}

		void GetThreshold() const
		{
			m_samplingLuminanceCB1.threshold;
		}


	private:
		/// <summary>
		/// 高度抽出パスの定数バッファ１に送る内容
		/// </summary>
		struct SSampleingLuminanceCB1
		{
			float threshold = 1.0f;
		};
		RenderTarget m_luminanceRenderTarget;	//光度抽出用のレンダリングターゲット
		Sprite m_luminanceSprite;				//光度抽出用のスプライト
		GaussianBlur m_gaussianBlur[4];			//ガウシアンブラー
		Sprite m_finalSprite;					//最終合成用のスプライト
		SSampleingLuminanceCB1 m_samplingLuminanceCB1;
	};
}

