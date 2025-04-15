#include "TMEnginePreCompile.h"
#include "SpriteRender.h"

namespace nsTMEngine {
	SpriteRender::SpriteRender()
	{
	}

	SpriteRender::~SpriteRender()
	{
	}

	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
	{
		SpriteInitData initData;
		//DDSファイル(画像データ)のファイルパスの指定
		initData.m_ddsFilePath[0] = filePath;
		//シェーダーのファイルパスの指定
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		//スプライトのサイズの指定
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		initData.m_alphaBlendMode = alphaBlendMode;

		m_sprite.Init(initData);

	}

	void SpriteRender::Drow(RenderContext& rc)
	{
		m_sprite.Draw(rc);
	}
}