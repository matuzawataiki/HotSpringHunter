#pragma oncen
#include "TMEnginePreCompile.h"

namespace nsTMEngine {
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// モデルの初期化用関数
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void Init(
			const char* filePath,
			AnimationClip* animationeClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxiz = enModelUpAxisZ
			);

		/// <summary>
		/// アニメーションの初期化用変数
		/// </summary>
		/// <param name="animtionClips"></param>
		/// <param name="numAnimationClips"></param>
		/// <param name="enModelUpAxis"></param>
		void InitAnimation(
			AnimationClip* animtionClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);
		/// <summary>
		/// スケルトンの初期化用関数
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// 描画処理。
		/// </summary>
		void Draw(RenderContext& rc);

	};
}
