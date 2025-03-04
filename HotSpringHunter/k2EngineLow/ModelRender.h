#pragma oncen
#include "TMEnginePreCompile.h"

namespace nsTMEngine {
	/// <summary>
	/// スキンモデルレンダー
	/// </summary>
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// モデルの初期化用関数
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationeClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションの数</param>
		/// <param name="enModelUpAxiz">モデルの上方向</param>
		void Init(
			const char* filePath,
			AnimationClip* animationeClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxiz = enModelUpAxisZ
			);
	private:
		/// <summary>
		/// スケルトンの初期化用関数
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);
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
		/// アニメーション済み頂点バッファの計算処理を初期化
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitComputeAnimatoinVertexBuffer(
			const char* tkmFilePath,
			EnModelUpAxis enModelUpAxis);
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// 描画処理。
		/// </summary>
		void Draw(RenderContext& rc);

	private:
		Model* m_model = nullptr;	//モデル
		Skeleton m_sukeleton;		//ボーン

	};
}
