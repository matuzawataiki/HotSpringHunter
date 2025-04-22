#pragma once
using namespace nsK2Engine;

namespace nsTMEngine {
	class SceneLight;
	class RenderingEngine;
	class TMEngine
	{
	private:
		TMEngine() {}
		~TMEngine();
	public:
		/// <summary>
		/// 初期化データ
		/// </summary>
		struct InitData {
			HWND hwnd;					//ウィンドウハンドル
			UINT fremeBufferWidth;		//フレームバッファの幅
			UINT frameBufferHeight;	//フレームバッファの高さ
		};

		/// <summary>
		/// インスタンスの作成
		/// </summary>
		/// <param name="initData"></param>
		static void CreateInstance(const InitData& initData)
		{
			m_instance = new TMEngine;
			m_instance->Init(initData);
		}
		/// <summary>
		/// 初期化
		/// </summary>
		void Init(const InitData& initData);

		/// <summary>
		/// エンジン処理の実行
		/// </summary>
		void Execute();

		/// <summary>
		/// インスタンスの破棄。
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
		}
		/// <summary>
		/// インスタンスを取得。
		/// </summary>
		/// <returns></returns>
		static TMEngine* GetInstance()
		{
			return m_instance;
		}

	private:

		K2EngineLow m_k2EngineLow;
		RenderingEngine m_renderingEngine;
		CollisionObjectManager m_collisionObjectManager;


		static TMEngine* m_instance;

	};

	extern CollisionObjectManager* g_collisionObjectManager;
	extern TMEngine*		g_tmEngine;
	extern SceneLight*		g_sceneLight;
	extern RenderingEngine* g_renderingEngine;

}