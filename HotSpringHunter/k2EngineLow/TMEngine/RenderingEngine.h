#pragma once
#include "TMEnginePreCompile.h"
namespace nsTMEngine
{
	class RendringEngine : public Noncopyable
	{
	public:
		RendringEngine();
		~RendringEngine();

		/// <summary>
		/// 描画オブジェクトを追加。
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}
	private:

		std::vector< IRenderer* > m_renderObjects;			// 描画オブジェクトのリスト。

	};

}